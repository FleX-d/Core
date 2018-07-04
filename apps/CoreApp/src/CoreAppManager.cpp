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
#include "iCoreAppAck.h"
#include "FleXdLogger.h"


namespace flexd {
    namespace core {

        CoreAppManager::CoreAppManager(const std::string& dbPath)
        : m_exe(),
        m_dbName("CoreAppDb"),
        m_db(dbPath, m_dbName)
        {
        }

        void CoreAppManager::onRequest(iCoreAppRequest& rqst){
            FLEX_LOG_TRACE("CoreAppManager::onRequest(iCoreAppRequest& rqst):");
            rqst.accept(*this);
            //using visitor design patern for check type of request
        }

        void CoreAppManager::visit(InstallRequest_t rqst) {

            std::string command;
            if (!m_db.getRecord(m_dbName, rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install):", command);

            if (!findInList(rqst->getName()))
                addToList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            std::string msg;
            if (appExecute(command, msg, rqst->getPath())) {
                changeStateInList(rqst->getName(), RqstType::Enum::stop);
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::success, rqst->getName(), rqst->getVersion(), msg));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending error");
            rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion(), msg));
        }

        void CoreAppManager::visit(UninstallRequest_t rqst) {

            std::string command;
            if (!m_db.getRecord(m_dbName, rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install):", command);

            if (!findInList(rqst->getName()))
                addToList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            std::string msg;
            if (appExecute(command, msg)) {
                eraseFromList(rqst->getName());
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::success, rqst->getName(), rqst->getVersion(), msg));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending error");
            rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion(), msg));
        }

        void CoreAppManager::visit(StartRequest_t rqst) {

            std::string command;
            if (!m_db.getRecord(m_dbName, rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install):", command);

            if (!findInList(rqst->getName()))
                addToList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            if (appExecute(command)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::success, rqst->getName(), rqst->getVersion()));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending error");
            rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
        }

        void CoreAppManager::visit(StopRequest_t rqst) {

            std::string command;
            if (!m_db.getRecord(m_dbName, rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install):", command);

            if (!findInList(rqst->getName()))
                addToList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            if (appExecute(command)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::success, rqst->getName(), rqst->getVersion()));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending error");
            rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
        }

        void CoreAppManager::visit(FreezeRequest_t rqst) {

            std::string command;
            if (!m_db.getRecord(m_dbName, rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install):", command);

            if (!findInList(rqst->getName()))
                addToList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            if (appExecute(command)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::success, rqst->getName(), rqst->getVersion()));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending error");
            rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
        }

        void CoreAppManager::visit(UnfreezeRequest_t rqst) {

            std::string command;
            if (!m_db.getRecord(m_dbName, rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install):", command);

            if (!findInList(rqst->getName()))
                addToList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            if (appExecute(command)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::success, rqst->getName(), rqst->getVersion()));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending error");
            rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
        }

        void CoreAppManager::visit(UpdateRequest_t rqst) {

            std::string command;
            if (!m_db.getRecord(m_dbName, rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install):", command);

            if (!findInList(rqst->getName()))
                addToList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            std::string msg;
            if (appExecute(command, msg, rqst->getPath())) {
                changeStateInList(rqst->getName(), RqstType::Enum::stop);
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::success, rqst->getName(), rqst->getVersion(), msg));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending error");
            rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion(), msg));
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
            /*TODO chceck if cmd run back true*/
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
                case 0: return "Install";
                case 1: return "Uninstall";
                case 2: return "Start";
                case 3: return "Stop";
                case 4: return "Freeze";
                case 5: return "Unfreeze";
                case 6: return "UpdateDB";
                default: return " ";
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
                case RqstType::unintall:
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
