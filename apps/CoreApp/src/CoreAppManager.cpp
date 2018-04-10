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


namespace flexd {
    namespace core {

        CoreAppManager::CoreAppManager(const std::string& dbPath)
        : m_exe(),
        m_dbName("CoreAppDb"),
        m_db(dbPath, m_dbName) {
        }

        bool CoreAppManager::tryProcesRequest(InstallRequest& rqst) {

            std::string command, key = "Install";
            m_db.getRecord(m_dbName, rqst.getName(), rqst.getVersion(), command, key);

            if(appExecute(command)){
                rqst.onSccess(iCoreAppAck(RqstAck::Enum::succes, rqst.getName(), rqst.getVersion()));
                return true;
            }
            rqst.onError(iCoreAppAck(RqstAck::Enum::fail, rqst.getName(), rqst.getVersion()));
            return false;
        }

        bool CoreAppManager::tryProcesRequest(UninstallRequest& rqst) {

            return true;
        }

        bool CoreAppManager::tryProcesRequest(StartRequest& rqst) {

            return true;
        }

        bool CoreAppManager::tryProcesRequest(StopRequest& rqst) {

            return true;
        }

        bool CoreAppManager::tryProcesRequest(FreezRequest& rqst) {

            return true;
        }

        bool CoreAppManager::tryProcesRequest(UnfreezRequest& rqst) {

            return true;
        }

        bool CoreAppManager::tryProcesRequest(UpdateRequest& rqst) {

            return true;
        }

        bool CoreAppManager::appExecute(const std::string cmd) {

            /*TODO chceck if cmd run back true*/

            m_exe.runOsCmd(cmd);


            return true;
        }



    }
}