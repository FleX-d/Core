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
        
        void CoreAppManager::Lambda(iCoreAppRequest& rqst){
            FLEX_LOG_TRACE("CoreAppManager::Lambda(iCoreAppRequest& rqst):");
            rqst.accept(*this);
            //using visitor design patern for check type of request
        }        

        void CoreAppManager::visit(InstallRequest_t rqst) {

            std::string command;
            if (!m_db.getRecord(m_dbName, rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install):", command);

            if (!findInList(rqst->getName()))
                addInList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            if (appExecute(command)) {
                changeStateInList(rqst->getName(), RqstType::Enum::stop);
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::succes, rqst->getName(), rqst->getVersion()));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending error");
            rqst->onError(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
        }

        void CoreAppManager::visit(UninstallRequest_t rqst) {

            std::string command;
            if (!m_db.getRecord(m_dbName, rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install):", command);

            if (!findInList(rqst->getName()))
                addInList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            if (appExecute(command)) {
                eraseInList(rqst->getName());
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::succes, rqst->getName(), rqst->getVersion()));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending error");
            rqst->onError(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
        }

        void CoreAppManager::visit(StartRequest_t rqst) {

            std::string command;
            if (!m_db.getRecord(m_dbName, rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install):", command);

            if (!findInList(rqst->getName()))
                addInList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            if (appExecute(command)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::succes, rqst->getName(), rqst->getVersion()));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending error");
            rqst->onError(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
        }

        void CoreAppManager::visit(StopRequest_t rqst) {

            std::string command;
            if (!m_db.getRecord(m_dbName, rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install):", command);

            if (!findInList(rqst->getName()))
                addInList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            if (appExecute(command)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::succes, rqst->getName(), rqst->getVersion()));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending error");
            rqst->onError(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
        }

        void CoreAppManager::visit(FreezRequest_t rqst) {

            std::string command;
            if (!m_db.getRecord(m_dbName, rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install):", command);

            if (!findInList(rqst->getName()))
                addInList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            if (appExecute(command)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::succes, rqst->getName(), rqst->getVersion()));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending error");
            rqst->onError(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
        }

        void CoreAppManager::visit(UnfreezRequest_t rqst) {

            std::string command;
            if (!m_db.getRecord(m_dbName, rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install):", command);

            if (!findInList(rqst->getName()))
                addInList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            if (appExecute(command)) {
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::succes, rqst->getName(), rqst->getVersion()));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending error");
            rqst->onError(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
        }

        void CoreAppManager::visit(UpdateRequest_t rqst) {

            std::string command;
            if (!m_db.getRecord(m_dbName, rqst->getName(), rqst->getVersion(), command, getDbKey(rqst->getType()))){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install):", command);

            if (!findInList(rqst->getName()))
                addInList(rqst->getName());
            if (!changeStateInList(rqst->getName(), rqst->getType())){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
                return;
            }

            if (appExecute(command)) {
                changeStateInList(rqst->getName(), RqstType::Enum::stop);
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst->onSccess(iCoreAppAck(RqstAck::Enum::succes, rqst->getName(), rqst->getVersion()));
                return;
            }
            changeStateInList(rqst->getName(), RqstType::Enum::stop);
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending error");
            rqst->onError(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
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

        std::string CoreAppManager::getDbKey(RqstType::Enum e) {
            switch (e) {
                case 0: return "Install";
                case 1: return "Uninstall";
                case 2: return "Start";
                case 3: return "Stop";
                case 4: return "Freez";
                case 5: return "Unfreez";
                case 6: return "UpdateDB";
                default: return " ";
            }
        }

        bool CoreAppManager::addInList(std::string mapName) {
            iApp a;
            return m_listOfApp.insert(std::make_pair(mapName, a)).second;
        }

        bool CoreAppManager::eraseInList(std::string mapName) {
            return m_listOfApp.erase(mapName);
        }

        bool CoreAppManager::findInList(std::string mapName) {
            if (m_listOfApp.find(mapName) != m_listOfApp.end())
                return true;
            else {
                return false;
            }
        }

        bool CoreAppManager::changeStateInList(std::string mapName, RqstType::Enum e) {
            switch (e) {
                case 0:
                case 1:
                case 6:
                {
                    return m_listOfApp.at(mapName).updating();
                }
                case 2:
                {
                    return m_listOfApp.at(mapName).starting();
                }
                case 3:
                {
                    return m_listOfApp.at(mapName).stoping();
                }
                case 4:
                {
                    return m_listOfApp.at(mapName).freezing();
                }
                case 5:
                {
                    return m_listOfApp.at(mapName).unfreezing();
                }
                default:
                {
                    return false;
                }
            }
        }
    }
}