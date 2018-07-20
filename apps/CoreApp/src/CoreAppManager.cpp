/*
Copyright (c) 2018, Globallogic s.r.o.
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
 * File:   CoreAppManager.cpp
 * Author: Peter Kocity
 *
 * Created on February 8, 2018, 10:48 AM
 */

#include <map>
#include <bits/stl_map.h>

#include "CoreAppManager.h"
#include "InstallRequest.h"
#include "UninstallRequest.h"
#include "StartRequest.h"
#include "StopRequest.h"
#include "FreezeRequest.h"
#include "UnfreezeRequest.h"
#include "UpdateRequest.h"
#include "InvalidRequest.h"
#include "CoreAppRequestValidator.h"
#include "iCoreAppAck.h"
#include <FleXdLogger.h>


namespace flexd {
    namespace core {

        CoreAppManager::CoreAppManager(const std::string& dbPath, const std::string& dbName)
        : m_rqstPoller(COREAPP_POLLER_MAXEVENTH),
          m_exe(),
          m_db(dbPath, dbName),
          m_requests(),
          m_apps(),
          m_rqstPollerThread(&flexd::icl::ipc::FleXdEpoll::loop, &m_rqstPoller),
          m_workerThread(&CoreAppManager::performRequests, this),
          m_safeStop(false) {
        }

        CoreAppManager::~CoreAppManager() {
            m_rqstPoller.endLoop();
            m_safeStop.store(true);
            m_requests.stopActions();
            m_rqstPollerThread.join();
            m_workerThread.join();
        }

        bool CoreAppManager::onRequest(pSharediCoreAppRequest_t rqst) {
            if (rqst) {
                FLEX_LOG_TRACE("CoreAppManager::onRequest(): ", rqst->getName(), rqst->getType());
                if (rqst->validate(*this)) {
                    FLEX_LOG_TRACE("CoreAppManager::onRequest(): adding request to queue");
                    rqst->setOnRequestDone(std::bind(&CoreAppManager::onRequestDone, this, std::placeholders::_1));
                    if (m_requests.push(std::move(rqst))) {
                        return true;
                    }
                }
            } else {
                FLEX_LOG_DEBUG("CoreAppManager::onRequest() invalid parameter");
            }
            return false;
        }

        const flexd::icl::ipc::FleXdEpoll& CoreAppManager::getRqstPoller() const {
            return m_rqstPoller;
        }

        flexd::icl::ipc::FleXdEpoll& CoreAppManager::getRqstPoller() {
            return m_rqstPoller;
        }

        void CoreAppManager::visit(InstallRequest_t rqst) {
            if (!rqst) {
                FLEX_LOG_ERROR("CoreAppManager::visit(InstallRequest) parameter is nullptr");
                return;
            }

            if (!prepareRequest(*rqst)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(InstallRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            std::string command;
            if (!m_db.getRecord(rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(InstallRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(InstallRequest):", command);

            if (!findInList(rqst->getName()))
                addToList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(InstallRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            std::string msg;
            if (appExecute(command, msg, rqst->getPath())) {
                changeStateInList(rqst->getName(), RqstType::Enum::stop);
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(InstallRequest): sending ack(success)");
//                 rqst->onAck(iCoreAppAck(RqstAck::Enum::success, rqst->getName(), rqst->getVersion(), msg));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(InstallRequest): sending ack(fail)");
            rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion(), msg));
        }

        bool CoreAppManager::validate(InstallRequest_t rqst) {
            if (!rqst) {
                FLEX_LOG_ERROR("CoreAppManager::validate(InstallRequest) parameter is nullptr");
                return false;
            }
            return CoreAppRequestValidator::validateRequestInstall(*rqst);
        }

        void CoreAppManager::visit(UninstallRequest_t rqst) {
            if (!rqst) {
                FLEX_LOG_ERROR("CoreAppManager::visit(UninstallRequest) parameter is nullptr");
                return;
            }

            if (!prepareRequest(*rqst)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(InstallRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            std::string command;
            if (!m_db.getRecord(rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(UninstallRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(UninstallRequest):", command);

            if (!findInList(rqst->getName()))
                addToList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(UninstallRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            std::string msg;
            if (appExecute(command, msg)) {
                eraseFromList(rqst->getName());
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(UninstallRequest): sending ack(success)");
//                 rqst->onAck(iCoreAppAck(RqstAck::Enum::success, rqst->getName(), rqst->getVersion(), msg));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(UninstallRequest): sending ack(fail)");
            rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion(), msg));
        }

        bool CoreAppManager::validate(UninstallRequest_t rqst) {
            if (!rqst) {
                FLEX_LOG_ERROR("CoreAppManager::validate(UninstallRequest) parameter is nullptr");
                return false;
            }
            return CoreAppRequestValidator::validateRequestUninstall(*rqst);
        }

        void CoreAppManager::visit(StartRequest_t rqst) {
            if (!rqst) {
                FLEX_LOG_ERROR("CoreAppManager::visit(StartRequest) parameter is nullptr");
                return;
            }

            if (!prepareRequest(*rqst)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(InstallRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            std::string command;
            if (!m_db.getRecord(rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(StartRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(StartRequest):", command);

            if (!findInList(rqst->getName()))
                addToList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(StartRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            if (appExecute(command)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(StartRequest): sending ack(success)");
//                 rqst->onAck(iCoreAppAck(RqstAck::Enum::success, rqst->getName(), rqst->getVersion()));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(StartRequest): sending ack(fail)");
            rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
        }

        bool CoreAppManager::validate(StartRequest_t rqst) {
            if (!rqst) {
                FLEX_LOG_ERROR("CoreAppManager::validate(StartRequest) parameter is nullptr");
                return false;
            }
            return CoreAppRequestValidator::validateRequestStart(*rqst);
        }

        void CoreAppManager::visit(StopRequest_t rqst) {
            if (!rqst) {
                FLEX_LOG_ERROR("CoreAppManager::visit(StopRequest) parameter is nullptr");
                return;
            }

            if (!prepareRequest(*rqst)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(InstallRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            std::string command;
            if (!m_db.getRecord(rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(StopRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(StopRequest):", command);

            if (!findInList(rqst->getName()))
                addToList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(StopRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            if (appExecute(command)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(StopRequest): sending ack(success)");
//                 rqst->onAck(iCoreAppAck(RqstAck::Enum::success, rqst->getName(), rqst->getVersion()));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(StopRequest): sending ack(fail)");
            rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
        }

        bool CoreAppManager::validate(StopRequest_t rqst) {
            if (!rqst) {
                FLEX_LOG_ERROR("CoreAppManager::validate(StopRequest) parameter is nullptr");
                return false;
            }
            return CoreAppRequestValidator::validateRequestStop(*rqst);
        }

        void CoreAppManager::visit(FreezeRequest_t rqst) {
            if (!rqst) {
                FLEX_LOG_ERROR("CoreAppManager::visit(FreezeRequest) parameter is nullptr");
                return;
            }

            if (!prepareRequest(*rqst)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(InstallRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            std::string command;
            if (!m_db.getRecord(rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(FreezeRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(FreezeRequest):", command);

            if (!findInList(rqst->getName()))
                addToList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(FreezeRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            if (appExecute(command)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(FreezeRequest): sending ack(success)");
//                 rqst->onAck(iCoreAppAck(RqstAck::Enum::success, rqst->getName(), rqst->getVersion()));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(FreezeRequest): sending ack(fail)");
            rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
        }

        bool CoreAppManager::validate(FreezeRequest_t rqst) {
            if (!rqst) {
                FLEX_LOG_ERROR("CoreAppManager::validate(FreezeRequest) parameter is nullptr");
                return false;
            }
            return CoreAppRequestValidator::validateRequestFreeze(*rqst);
        }

        void CoreAppManager::visit(UnfreezeRequest_t rqst) {
            if (!rqst) {
                FLEX_LOG_ERROR("CoreAppManager::visit(UnfreezeRequest) parameter is nullptr");
                return;
            }

            if (!prepareRequest(*rqst)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(InstallRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            std::string command;
            if (!m_db.getRecord(rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(UnfreezeRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(UnfreezeRequest):", command);

            if (!findInList(rqst->getName()))
                addToList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(UnfreezeRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            if (appExecute(command)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(UnfreezeRequest): sending ack(success)");
//                 rqst->onAck(iCoreAppAck(RqstAck::Enum::success, rqst->getName(), rqst->getVersion()));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(UnfreezeRequest): sending ack(fail)");
            rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
        }

        bool CoreAppManager::validate(UnfreezeRequest_t rqst) {
            if (!rqst) {
                FLEX_LOG_ERROR("CoreAppManager::validate(UnfreezeRequest) parameter is nullptr");
                return false;
            }
            return CoreAppRequestValidator::validateRequestUnfreeze(*rqst);
        }

        void CoreAppManager::visit(UpdateRequest_t rqst) {
            if (!rqst) {
                FLEX_LOG_ERROR("CoreAppManager::visit(UpdateRequest) parameter is nullptr");
                return;
            }

            if (!prepareRequest(*rqst)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(InstallRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            std::string command;
            if (!m_db.getRecord(rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(UpdateRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(UpdateRequest):", command);

            if (!findInList(rqst->getName()))
                addToList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(UpdateRequest): sending ack(fail)");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            std::string msg;
            if (appExecute(command, msg, rqst->getPath())) {
                changeStateInList(rqst->getName(), RqstType::Enum::stop);
                FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(UpdateRequest): sending ack(success)");
//                 rqst->onAck(iCoreAppAck(RqstAck::Enum::success, rqst->getName(), rqst->getVersion(), msg));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcessRequest(UpdateRequest): sending ack(fail)");
            rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion(), msg));
        }

        bool CoreAppManager::validate(UpdateRequest_t rqst) {
            if (!rqst) {
                FLEX_LOG_ERROR("CoreAppManager::validate(UpdateRequest) parameter is nullptr");
                return false;
            }
            return CoreAppRequestValidator::validateRequestUpdate(*rqst);
        }

        bool CoreAppManager::prepareRequest(iCoreAppRequest& rqst) {
            time_t timeout;
            if (!m_db.getTimeout(rqst.getName(), rqst.getVersion(), timeout)) {
                FLEX_LOG_TRACE("CoreAppManager::prepareRequest(): sending ack(fail)");
                rqst.onAck(iCoreAppAck(RqstAck::Enum::fail, rqst.getName(), rqst.getVersion()));
                return false;
            }
            if (!rqst.setTimeout(m_rqstPoller, timeout)) {
                FLEX_LOG_TRACE("CoreAppManager::prepareRequest(): sending ack(fail)");
                rqst.onAck(iCoreAppAck(RqstAck::Enum::fail, rqst.getName(), rqst.getVersion()));
                return false;
            }
            if (!rqst.prepareRqst()) {
                FLEX_LOG_ERROR("CoreAppManager::prepareRequest(): preparing request failed");
                rqst.onAck(iCoreAppAck(RqstAck::Enum::fail, rqst.getName(), rqst.getVersion()));
                return false;
            }
            return true;
        }

        void CoreAppManager::performRequests() {
            FLEX_LOG_TRACE("CoreAppManager::performRequests()");
            pSharediCoreAppRequest_t rqst;
            //TODO this is test version, always takes only the first request
            while (!m_safeStop.load()) {
                rqst = m_requests.front();
                if (rqst && !rqst->isDone()) {
                    //using visitor design patern to check type of the request
                    rqst->accept(*this);
                    rqst->trigger();
                    //TODO possible deadlock
                    while (!rqst->isDone()) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    }
                }
                rqst.reset();
            }
        }

        void CoreAppManager::onRequestDone(bool result) {
            if (result) {
                //TODO request done with success
            } else {
                //TODO timer expired, terminate actions
            }
            FLEX_LOG_TRACE("CoreAppManager::onRequestDone(): deleting request from queue");
            m_requests.pop();
        }

        bool CoreAppManager::appExecute(const std::string& cmd) {

            FLEX_LOG_TRACE("CoreAppManager::appExecute():");
            /*TODO chceck if cmd run back true*/
            try {
                m_exe.runOsCmd(cmd);
            }
            catch (std::exception& e) {
                return false;
            }
            return true;
        }

        bool CoreAppManager::appExecute(const std::string& cmd, std::string& message, const std::string& path){
            FLEX_LOG_TRACE("CoreAppManager::appExecute():");
            /*TODO check if cmd run back true*/
            try {
                message=m_exe.runOsCmdWithResult(cmd, path);
            }
            catch (std::exception& e) {
                return false;
            }
            return true;
        }

        std::string CoreAppManager::getDbKey(RqstType::Enum e) {
            switch (e) {
                case RqstType::Enum::install: return "Install";
                case RqstType::Enum::uninstall: return "Uninstall";
                case RqstType::Enum::start: return "Start";
                case RqstType::Enum::stop: return "Stop";
                case RqstType::Enum::freeze: return "Freeze";
                case RqstType::Enum::unfreeze: return "Unfreeze";
                case RqstType::Enum::update: return "UpdateDB";
                default: return "";
            }
        }

        bool CoreAppManager::addToList(std::string appName) {
            iApp a;
            return m_apps.insert(std::make_pair(appName, a)).second;
        }

        bool CoreAppManager::eraseFromList(std::string appName) {
            return m_apps.erase(appName);
        }

        bool CoreAppManager::findInList(std::string appName) {
            if (m_apps.find(appName) != m_apps.end())
                return true;
            else {
                return false;
            }
        }

        bool CoreAppManager::changeStateInList(std::string appName, RqstType::Enum rqstType) {
            switch (rqstType) {
                case RqstType::install:
                case RqstType::uninstall:
                case RqstType::update:
                {
                    return m_apps.at(appName).update();
                }
                case RqstType::start:
                {
                    return m_apps.at(appName).start();
                }
                case RqstType::stop:
                {
                    return m_apps.at(appName).stop();
                }
                case RqstType::freeze:
                {
                    return m_apps.at(appName).freeze();
                }
                case RqstType::unfreeze:
                {
                    return m_apps.at(appName).unfreeze();
                }
                default:
                {
                    return false;
                }
            }
        }
    }
}
