/*
Copyright (c) 2017, Globallogic s.r.o.
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
 * File:   main.cpp
 * Author: Branislav Podkonicky
 *
 * Created on July 29, 2018, 09:45 AM
 */

#ifndef IPCINTERFACE_H
#define IPCINTERFACE_H

#include <FleXdEpoll.h>
#include <FleXdIPCMsg.h>
#include <FleXdIPCConnector.h>
#include <FleXdLogger.h>

class IPCInterface : public flexd::icl::ipc::IPCConnector {
public:
    IPCInterface(uint32_t id, flexd::icl::ipc::FleXdEpoll& poller);
    virtual ~IPCInterface();
    void send(uint32_t peerID, std::shared_ptr<flexd::icl::ipc::FleXdIPCMsg> msg);
    virtual void receiveMsg(flexd::icl::ipc::pSharedFleXdIPCMsg msg) override {}
    virtual void onBeforeRcvMsg(std::shared_ptr<const flexd::icl::ipc::FleXdIPCMsg> msg);
    uint32_t getTimestamp();
    virtual void onConnectPeer(uint32_t peerID, bool genericPeer) override {
        std::cout << "onConnectPeer() -> " << peerID << ", generic = " << genericPeer << "\n";
        uint8_t msgType(1);
        std::vector<uint8_t> payload;
        unsigned char myChar(1);
        payload.push_back(myChar);
        auto msgPtr = std::make_shared<flexd::icl::ipc::FleXdIPCMsg>(msgType, std::move(payload));
        send(peerID, msgPtr);
    }

private:
    flexd::icl::ipc::FleXdEpoll& m_poller;
    uint8_t m_counter;
};
#endif //IPCINTERFACE_H
