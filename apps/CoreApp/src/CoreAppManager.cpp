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
 * File:   CoreAppManager.cppF
 * Author: Peter Kocity
 *
 * Created on February 8, 2018, 10:48 AM
 */

#include "CoreAppManager.h"
#include "iCoreAppAck.h"
#include "FleXdLogger.h"


namespace flexd {
    namespace core {

        CoreAppManager::CoreAppManager(const std::string& dbPath)
        : m_exe(),
        m_dbName("CoreAppDb"),
        m_db(dbPath, m_dbName) {
            FLEX_LOG_INIT("CoreAppManager");
        }
        
        bool CoreAppManager::tryProcesRequest(InstallRequest& rqst) {
            
            std::string command;
            m_db.getRecord(m_dbName, rqst.getName(), rqst.getVersion(), command, getDbKey(rqst.getType()));
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install):", command);
            
            /*TODO check install file*/
            
            if(appExecute(command)){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending ack");
                rqst.onSccess(iCoreAppAck(RqstAck::Enum::succes, rqst.getName(), rqst.getVersion()));
                return true;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Install): sending error");
            rqst.onError(iCoreAppAck(RqstAck::Enum::fail, rqst.getName(), rqst.getVersion()));
            return false;
        }

        bool CoreAppManager::tryProcesRequest(UninstallRequest& rqst) {

            std::string command;
            m_db.getRecord(m_dbName, rqst.getName(), rqst.getVersion(), command, getDbKey(rqst.getType()));
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Uninstall):", command);
            
            /*TODO check if running then stop*/

            if(appExecute(command)){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Uninstall): sending ack");
                rqst.onSccess(iCoreAppAck(RqstAck::Enum::succes, rqst.getName(), rqst.getVersion()));
                return true;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Uninstall): sending error");
            rqst.onError(iCoreAppAck(RqstAck::Enum::fail, rqst.getName(), rqst.getVersion()));
            return false;
        }

        bool CoreAppManager::tryProcesRequest(StartRequest& rqst) {

            std::string command;
            m_db.getRecord(m_dbName, rqst.getName(), rqst.getVersion(), command, getDbKey(rqst.getType()));
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Start):", command);

            /*TODO check if is running or freez*/
            
            if(appExecute(command)){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Start): sending ack");
                rqst.onSccess(iCoreAppAck(RqstAck::Enum::succes, rqst.getName(), rqst.getVersion()));
                return true;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Start): sending error");
            rqst.onError(iCoreAppAck(RqstAck::Enum::fail, rqst.getName(), rqst.getVersion()));
            return false;
        }

        bool CoreAppManager::tryProcesRequest(StopRequest& rqst) {

            std::string command;
            m_db.getRecord(m_dbName, rqst.getName(), rqst.getVersion(), command, getDbKey(rqst.getType()));
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Stop):", command);
            
            /*TODO check if is stop*/

            if(appExecute(command)){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Stop): sending ack");
                rqst.onSccess(iCoreAppAck(RqstAck::Enum::succes, rqst.getName(), rqst.getVersion()));
                return true;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Stop): sending error");
            rqst.onError(iCoreAppAck(RqstAck::Enum::fail, rqst.getName(), rqst.getVersion()));
            return false;
        }

        bool CoreAppManager::tryProcesRequest(FreezRequest& rqst) {

            std::string command;
            m_db.getRecord(m_dbName, rqst.getName(), rqst.getVersion(), command, getDbKey(rqst.getType()));
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Freez):", command);
            
            /*TODO check if is running*/

            if(appExecute(command)){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Freez): sending ack");
                rqst.onSccess(iCoreAppAck(RqstAck::Enum::succes, rqst.getName(), rqst.getVersion()));
                return true;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Freez): sending error");
            rqst.onError(iCoreAppAck(RqstAck::Enum::fail, rqst.getName(), rqst.getVersion()));
            return false;
        }

        bool CoreAppManager::tryProcesRequest(UnfreezRequest& rqst) {
            
            /*TODO check if is freez*/

            std::string command;
            m_db.getRecord(m_dbName, rqst.getName(), rqst.getVersion(), command, getDbKey(rqst.getType()));
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Unfreez):", command);

            if(appExecute(command)){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Unfreez): sending ack");
                rqst.onSccess(iCoreAppAck(RqstAck::Enum::succes, rqst.getName(), rqst.getVersion()));
                return true;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Unfreez): sending error");
            rqst.onError(iCoreAppAck(RqstAck::Enum::fail, rqst.getName(), rqst.getVersion()));
            return false;
        }

        bool CoreAppManager::tryProcesRequest(UpdateRequest& rqst) {

            std::string command;
            m_db.getRecord(m_dbName, rqst.getName(), rqst.getVersion(), command, getDbKey(rqst.getType()));
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Update):", command);

            /*TODO*/
            
            if(appExecute(command)){
                FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Update): sending ack");
                rqst.onSccess(iCoreAppAck(RqstAck::Enum::succes, rqst.getName(), rqst.getVersion()));
                return true;
            }
            FLEX_LOG_TRACE("CoreAppManager::tryProcesRequest(Update): sending error");
            rqst.onError(iCoreAppAck(RqstAck::Enum::fail, rqst.getName(), rqst.getVersion()));
            return false;
        }

        bool CoreAppManager::appExecute(const std::string& cmd) {
            
            FLEX_LOG_TRACE("CoreAppManager::appExecute():");
            /*TODO chceck if cmd run back true*/
            
            try{
                m_exe.runOsCmd(cmd);
            }
            catch(std::exception& e){
                return false;
            }
            return true;
        }

        std::string CoreAppManager::getDbKey(RqstType::Enum e){
            switch(e){
                case 0: return "Install";
                case 1: return "Uninstall";
                case 2: return "Start";
                case 3: return "Stop";
                case 4: return "Freez";
                case 5: return "Unfreez";
                case 6: return "UpdateDB";
                default: return "";
            }
        }
        
    }
}