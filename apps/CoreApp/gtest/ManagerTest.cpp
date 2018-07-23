/*
 C opyright (c) 2018, G*loballogic s.r.o.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of the Globallogic s.r.o. nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC S.R.O. BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * File:   ManagerTest.cpp
 * Author: Peter Kocity
 *
 * Created on March 27, 2018, 11:10 AM
 */

#include "CoreAppManager.h"
#include "IPCClient.h"
#include <FleXdIPCConnector.h>
#include <FleXdEpoll.h>
#include <FleXdIPCCommon.h>
#include <FleXdEvent.h>
#include <JsonObj.h>
#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <cstdlib>
#include <iostream>

#define DOCKER_APP_ID           12345
#define CORE_APP_ID             11111
#define SND_RQST_ID             1
#define RCV_RQST_ID             2
#define TEST_COREAPP_DB         std::string(getenv("HOME")) + std::string("/tmp/CoreApp/TestCoreAppDb.db")
#define TEST_COREAPP_DB_NAME    "TestCoreAppDb"
#define TEST_ID                 "Test"
#define TEST_ID_TIMEOUT         "Test_timeout"
#define TEST_ID_TIMEOUT2        "Test_timeout2"
#define TEST_ID_TIMEOUT3        "Test_timeout3"
#define TEST_UNUSED             "unused"
#define TEST_PATH               "/some_path/TEST"

namespace {

    struct RqstAckDummy {
        void reset() { operationAck = false; message.clear(); appID. clear(), valid = false; }
        bool operationAck = false;
        std::string message;
        std::string appID;
        bool valid = false;
    };

    std::atomic_bool rcvMsgCalled(false);

    class DockerAppDummy: public flexd::icl::ipc::IPCConnector {
    public:
        DockerAppDummy(flexd::icl::ipc::FleXdEpoll& poller) : flexd::icl::ipc::IPCConnector(DOCKER_APP_ID, poller) { addPeer(CORE_APP_ID); }
        ~DockerAppDummy() = default;

        RqstAckDummy m_rqstAck;

        void sendRqst(uint8_t Operation, const std::string& Message, const std::string& AppID) {
            flexd::icl::JsonObj json;
            json.add<int>("/id", SND_RQST_ID);
            json.add<uint8_t>("/payload/Operation", Operation);
            json.add<std::string>("/payload/Message", Message);
            json.add<std::string>("/payload/AppID", AppID);

            std::string tmp = json.getJson();
            std::vector<uint8_t> payload(tmp.begin(), tmp.end());

            auto msg = std::make_shared<flexd::icl::ipc::FleXdIPCMsg>(std::move(payload), true);
            sendMsg(msg, CORE_APP_ID);
        }

        void receiveMsg(flexd::icl::ipc::pSharedFleXdIPCMsg msg) override {
            rcvMsgCalled.store(true);
            if(!msg) {
                return;
            }

            flexd::icl::JsonObj json(std::string(msg->getPayload().begin(), msg->getPayload().end()));
            if (json.exist("/id")) {
                int id;
                json.get<int>("/id", id);
                if (id == RCV_RQST_ID) {
                    m_rqstAck.valid = false;
                    if (json.exist("/payload/OperationAck")) {
                        json.get<bool>("/payload/OperationAck", m_rqstAck.operationAck);
                        if(json.exist("/payload/Message")) {
                            json.get<std::string>("/payload/Message", m_rqstAck.message);
                            if(json.exist("/payload/AppID")) {
                                json.get<std::string>("/payload/AppID", m_rqstAck.appID);
                                m_rqstAck.valid = true;
                            }
                        }
                    }
                }
            }
        }

        void onConnectPeer(uint32_t peerID, bool genericPeer) override {
        }
    };

    TEST(Manager, Installrequest_normal) {
        flexd::icl::ipc::FleXdEpoll poller(10);
        flexd::icl::ipc::FleXdEpoll poller2(10);
        std::thread handlerPoller([&poller](void) { poller.loop(); });
        std::thread handlerPoller2([&poller2](void) { poller2.loop(); });

        DockerAppDummy dockerApp(poller);
        flexd::core::CoreAppManager manager(TEST_COREAPP_DB, TEST_COREAPP_DB_NAME);
        flexd::core::IPCClient client(poller2, manager.getRqstPoller());
        client.setOnRequest([&manager](flexd::core::pSharediCoreAppRequest_t rqst) { return manager.onRequest(rqst); });
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::install, TEST_PATH, TEST_ID);

        uint8_t count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_TRUE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, std::string("install\nsh: 1: ") + std::string(TEST_PATH) + std::string(": not found\n"));
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID);

        poller.endLoop();
        poller2.endLoop();
        handlerPoller.join();
        handlerPoller2.join();
    }

    TEST(Manager, Installrequest_timeout) {
        flexd::icl::ipc::FleXdEpoll poller(10);
        flexd::icl::ipc::FleXdEpoll poller2(10);
        std::thread handlerPoller([&poller](void) { poller.loop(); });
        std::thread handlerPoller2([&poller2](void) { poller2.loop(); });

        DockerAppDummy dockerApp(poller);
        flexd::core::CoreAppManager manager(TEST_COREAPP_DB, TEST_COREAPP_DB_NAME);
        flexd::core::IPCClient client(poller2, manager.getRqstPoller());
        client.setOnRequest([&manager](flexd::core::pSharediCoreAppRequest_t rqst) { return manager.onRequest(rqst); });
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::install, TEST_PATH, TEST_ID_TIMEOUT);

        uint8_t count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_FALSE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, COREAPP_RQST_TIMEOUT_MESSAGE);
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID_TIMEOUT);

        poller.endLoop();
        poller2.endLoop();
        handlerPoller.join();
        handlerPoller2.join();
    }

    TEST(Manager, Uninstallrequest_normal) {
        flexd::icl::ipc::FleXdEpoll poller(10);
        flexd::icl::ipc::FleXdEpoll poller2(10);
        std::thread handlerPoller([&poller](void) { poller.loop(); });
        std::thread handlerPoller2([&poller2](void) { poller2.loop(); });

        DockerAppDummy dockerApp(poller);
        flexd::core::CoreAppManager manager(TEST_COREAPP_DB, TEST_COREAPP_DB_NAME);
        flexd::core::IPCClient client(poller2, manager.getRqstPoller());
        client.setOnRequest([&manager](flexd::core::pSharediCoreAppRequest_t rqst) { return manager.onRequest(rqst); });
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::uninstall, TEST_UNUSED, TEST_ID);

        uint8_t count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_TRUE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, "uninstall\n");
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID);

        poller.endLoop();
        poller2.endLoop();
        handlerPoller.join();
        handlerPoller2.join();
    }

    TEST(Manager, Uninstallrequest_timeout) {
        flexd::icl::ipc::FleXdEpoll poller(10);
        flexd::icl::ipc::FleXdEpoll poller2(10);
        std::thread handlerPoller([&poller](void) { poller.loop(); });
        std::thread handlerPoller2([&poller2](void) { poller2.loop(); });

        DockerAppDummy dockerApp(poller);
        flexd::core::CoreAppManager manager(TEST_COREAPP_DB, TEST_COREAPP_DB_NAME);
        flexd::core::IPCClient client(poller2, manager.getRqstPoller());
        client.setOnRequest([&manager](flexd::core::pSharediCoreAppRequest_t rqst) { return manager.onRequest(rqst); });
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::uninstall, TEST_UNUSED, TEST_ID_TIMEOUT);

        uint8_t count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_FALSE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, COREAPP_RQST_TIMEOUT_MESSAGE);
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID_TIMEOUT);

        poller.endLoop();
        poller2.endLoop();
        handlerPoller.join();
        handlerPoller2.join();
    }

    TEST(Manager, Startrequest_normal) {
        flexd::icl::ipc::FleXdEpoll poller(10);
        flexd::icl::ipc::FleXdEpoll poller2(10);
        std::thread handlerPoller([&poller](void) { poller.loop(); });
        std::thread handlerPoller2([&poller2](void) { poller2.loop(); });

        DockerAppDummy dockerApp(poller);
        flexd::core::CoreAppManager manager(TEST_COREAPP_DB, TEST_COREAPP_DB_NAME);
        flexd::core::IPCClient client(poller2, manager.getRqstPoller());
        client.setOnRequest([&manager](flexd::core::pSharediCoreAppRequest_t rqst) { return manager.onRequest(rqst); });
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::start, TEST_UNUSED, TEST_ID);

        uint8_t count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_TRUE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, "");
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID);

        poller.endLoop();
        poller2.endLoop();
        handlerPoller.join();
        handlerPoller2.join();
    }

    TEST(Manager, Startrequest_timeout) {
        flexd::icl::ipc::FleXdEpoll poller(10);
        flexd::icl::ipc::FleXdEpoll poller2(10);
        std::thread handlerPoller([&poller](void) { poller.loop(); });
        std::thread handlerPoller2([&poller2](void) { poller2.loop(); });

        DockerAppDummy dockerApp(poller);
        flexd::core::CoreAppManager manager(TEST_COREAPP_DB, TEST_COREAPP_DB_NAME);
        flexd::core::IPCClient client(poller2, manager.getRqstPoller());
        client.setOnRequest([&manager](flexd::core::pSharediCoreAppRequest_t rqst) { return manager.onRequest(rqst); });
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::start, TEST_UNUSED, TEST_ID_TIMEOUT);

        uint8_t count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_FALSE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, COREAPP_RQST_TIMEOUT_MESSAGE);
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID_TIMEOUT);

        poller.endLoop();
        poller2.endLoop();
        handlerPoller.join();
        handlerPoller2.join();
    }

    TEST(Manager, Stoprequest_normal) {
        flexd::icl::ipc::FleXdEpoll poller(10);
        flexd::icl::ipc::FleXdEpoll poller2(10);
        std::thread handlerPoller([&poller](void) { poller.loop(); });
        std::thread handlerPoller2([&poller2](void) { poller2.loop(); });

        DockerAppDummy dockerApp(poller);
        flexd::core::CoreAppManager manager(TEST_COREAPP_DB, TEST_COREAPP_DB_NAME);
        flexd::core::IPCClient client(poller2, manager.getRqstPoller());
        client.setOnRequest([&manager](flexd::core::pSharediCoreAppRequest_t rqst) { return manager.onRequest(rqst); });
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::start, TEST_UNUSED, TEST_ID);
        uint8_t count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_TRUE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, "");
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID);

        dockerApp.m_rqstAck.reset();
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::stop, TEST_UNUSED, TEST_ID);
        count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_TRUE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, "");
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID);

        poller.endLoop();
        poller2.endLoop();
        handlerPoller.join();
        handlerPoller2.join();
    }

    TEST(Manager, Stoprequest_timeout) {
        flexd::icl::ipc::FleXdEpoll poller(10);
        flexd::icl::ipc::FleXdEpoll poller2(10);
        std::thread handlerPoller([&poller](void) { poller.loop(); });
        std::thread handlerPoller2([&poller2](void) { poller2.loop(); });

        DockerAppDummy dockerApp(poller);
        flexd::core::CoreAppManager manager(TEST_COREAPP_DB, TEST_COREAPP_DB_NAME);
        flexd::core::IPCClient client(poller2, manager.getRqstPoller());
        client.setOnRequest([&manager](flexd::core::pSharediCoreAppRequest_t rqst) { return manager.onRequest(rqst); });
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::start, TEST_UNUSED, TEST_ID_TIMEOUT2);
        uint8_t count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_TRUE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, "");
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID_TIMEOUT2);

        dockerApp.m_rqstAck.reset();
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::stop, TEST_UNUSED, TEST_ID_TIMEOUT2);
        count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_FALSE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, COREAPP_RQST_TIMEOUT_MESSAGE);
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID_TIMEOUT2);

        poller.endLoop();
        poller2.endLoop();
        handlerPoller.join();
        handlerPoller2.join();
    }

    TEST(Manager, Freezerequest_normal) {
        flexd::icl::ipc::FleXdEpoll poller(10);
        flexd::icl::ipc::FleXdEpoll poller2(10);
        std::thread handlerPoller([&poller](void) { poller.loop(); });
        std::thread handlerPoller2([&poller2](void) { poller2.loop(); });

        DockerAppDummy dockerApp(poller);
        flexd::core::CoreAppManager manager(TEST_COREAPP_DB, TEST_COREAPP_DB_NAME);
        flexd::core::IPCClient client(poller2, manager.getRqstPoller());
        client.setOnRequest([&manager](flexd::core::pSharediCoreAppRequest_t rqst) { return manager.onRequest(rqst); });
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::start, TEST_UNUSED, TEST_ID);
        uint8_t count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_TRUE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, "");
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID);

        dockerApp.m_rqstAck.reset();
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::freeze, TEST_UNUSED, TEST_ID);
        count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_TRUE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, "");
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID);

        poller.endLoop();
        poller2.endLoop();
        handlerPoller.join();
        handlerPoller2.join();
    }

    TEST(Manager, Freezerequest_timeout) {
        flexd::icl::ipc::FleXdEpoll poller(10);
        flexd::icl::ipc::FleXdEpoll poller2(10);
        std::thread handlerPoller([&poller](void) { poller.loop(); });
        std::thread handlerPoller2([&poller2](void) { poller2.loop(); });

        DockerAppDummy dockerApp(poller);
        flexd::core::CoreAppManager manager(TEST_COREAPP_DB, TEST_COREAPP_DB_NAME);
        flexd::core::IPCClient client(poller2, manager.getRqstPoller());
        client.setOnRequest([&manager](flexd::core::pSharediCoreAppRequest_t rqst) { return manager.onRequest(rqst); });
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::start, TEST_UNUSED, TEST_ID_TIMEOUT2);
        uint8_t count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_TRUE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, "");
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID_TIMEOUT2);

        dockerApp.m_rqstAck.reset();
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::freeze, TEST_UNUSED, TEST_ID_TIMEOUT2);
        count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_FALSE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, COREAPP_RQST_TIMEOUT_MESSAGE);
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID_TIMEOUT2);

        poller.endLoop();
        poller2.endLoop();
        handlerPoller.join();
        handlerPoller2.join();
    }

    TEST(Manager, Unfreezerequest_normal) {
        flexd::icl::ipc::FleXdEpoll poller(10);
        flexd::icl::ipc::FleXdEpoll poller2(10);
        std::thread handlerPoller([&poller](void) { poller.loop(); });
        std::thread handlerPoller2([&poller2](void) { poller2.loop(); });

        DockerAppDummy dockerApp(poller);
        flexd::core::CoreAppManager manager(TEST_COREAPP_DB, TEST_COREAPP_DB_NAME);
        flexd::core::IPCClient client(poller2, manager.getRqstPoller());
        client.setOnRequest([&manager](flexd::core::pSharediCoreAppRequest_t rqst) { return manager.onRequest(rqst); });
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::start, TEST_UNUSED, TEST_ID);
        uint8_t count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_TRUE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, "");
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID);

        dockerApp.m_rqstAck.reset();
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::freeze, TEST_UNUSED, TEST_ID);
        count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_TRUE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, "");
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID);

        dockerApp.m_rqstAck.reset();
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::unfreeze, TEST_UNUSED, TEST_ID);
        count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_TRUE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, "");
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID);

        poller.endLoop();
        poller2.endLoop();
        handlerPoller.join();
        handlerPoller2.join();
    }

    TEST(Manager, Unfreezerequest_timeout) {
        flexd::icl::ipc::FleXdEpoll poller(10);
        flexd::icl::ipc::FleXdEpoll poller2(10);
        std::thread handlerPoller([&poller](void) { poller.loop(); });
        std::thread handlerPoller2([&poller2](void) { poller2.loop(); });

        DockerAppDummy dockerApp(poller);
        flexd::core::CoreAppManager manager(TEST_COREAPP_DB, TEST_COREAPP_DB_NAME);
        flexd::core::IPCClient client(poller2, manager.getRqstPoller());
        client.setOnRequest([&manager](flexd::core::pSharediCoreAppRequest_t rqst) { return manager.onRequest(rqst); });
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::start, TEST_UNUSED, TEST_ID_TIMEOUT3);
        uint8_t count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_TRUE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, "");
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID_TIMEOUT3);

        dockerApp.m_rqstAck.reset();
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::freeze, TEST_UNUSED, TEST_ID_TIMEOUT3);
        count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_TRUE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, "");
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID_TIMEOUT3);

        dockerApp.m_rqstAck.reset();
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::unfreeze, TEST_UNUSED, TEST_ID_TIMEOUT3);
        count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_FALSE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, COREAPP_RQST_TIMEOUT_MESSAGE);
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID_TIMEOUT3);

        poller.endLoop();
        poller2.endLoop();
        handlerPoller.join();
        handlerPoller2.join();
    }

    TEST(Manager, Updaterequest_normal) {
        flexd::icl::ipc::FleXdEpoll poller(10);
        flexd::icl::ipc::FleXdEpoll poller2(10);
        std::thread handlerPoller([&poller](void) { poller.loop(); });
        std::thread handlerPoller2([&poller2](void) { poller2.loop(); });

        DockerAppDummy dockerApp(poller);
        flexd::core::CoreAppManager manager(TEST_COREAPP_DB, TEST_COREAPP_DB_NAME);
        flexd::core::IPCClient client(poller2, manager.getRqstPoller());
        client.setOnRequest([&manager](flexd::core::pSharediCoreAppRequest_t rqst) { return manager.onRequest(rqst); });
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::update, TEST_PATH, TEST_ID);

        uint8_t count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_TRUE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, std::string("update\nsh: 1: ") + std::string(TEST_PATH) + std::string(": not found\n"));
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID);
        poller.endLoop();
        poller2.endLoop();
        handlerPoller.join();
        handlerPoller2.join();
    }

    TEST(Manager, Updaterequest_timeout) {
        flexd::icl::ipc::FleXdEpoll poller(10);
        flexd::icl::ipc::FleXdEpoll poller2(10);
        std::thread handlerPoller([&poller](void) { poller.loop(); });
        std::thread handlerPoller2([&poller2](void) { poller2.loop(); });

        DockerAppDummy dockerApp(poller);
        flexd::core::CoreAppManager manager(TEST_COREAPP_DB, TEST_COREAPP_DB_NAME);
        flexd::core::IPCClient client(poller2, manager.getRqstPoller());
        client.setOnRequest([&manager](flexd::core::pSharediCoreAppRequest_t rqst) { return manager.onRequest(rqst); });
        rcvMsgCalled.store(false);
        dockerApp.sendRqst(flexd::core::RqstType::Enum::update, TEST_PATH, TEST_ID_TIMEOUT);

        uint8_t count = 0;
        while(!rcvMsgCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if(++count >= 20) {
                poller.endLoop();
                poller2.endLoop();
                handlerPoller.join();
                handlerPoller2.join();
                ASSERT_TRUE(count < 20);
            }
        }
        EXPECT_TRUE(dockerApp.m_rqstAck.valid);
        EXPECT_FALSE(dockerApp.m_rqstAck.operationAck);
        EXPECT_EQ(dockerApp.m_rqstAck.message, COREAPP_RQST_TIMEOUT_MESSAGE);
        EXPECT_EQ(dockerApp.m_rqstAck.appID, TEST_ID_TIMEOUT);
        poller.endLoop();
        poller2.endLoop();
        handlerPoller.join();
        handlerPoller2.join();
    }

}
