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
 * File:   FleXdWatchdog.cpp
 * Author: Branislav Podkonicky
 *
 * Created on July 29, 2018, 09:45 AM
 */

#include "FleXdWatchdog.h"
#include <FleXdIPCMsgTypes.h>
#include <FleXdLogger.h>

namespace flexd {
    namespace icl {
        namespace ipc {
            
            FleXdWatchdog::FleXdWatchdog(FleXdEpoll& poller, IPCInterface& ipc, std::string path, int timeout, int coreAppIpcId)
                : m_poller(poller),
                  m_timer(m_poller, 1, 0, true, [&]{ this->onTimer(); }),
                  m_ipc(ipc),
                  m_path(path),
                  m_appName(""),
                  m_defaultTimeout(timeout),
                  m_actualTimeout(timeout),
                  m_coreAppIpcId(coreAppIpcId),
                  m_valid(true) {
                std::string pidFileName = m_path.substr(m_path.rfind("/")+1);
                m_appName = pidFileName.substr(0, pidFileName.length() - 4);
                m_timer.start(); 
            }
            
            FleXdWatchdog::~FleXdWatchdog() {
                m_timer.stop();
            }

            std::string FleXdWatchdog::getPath() {
                return m_path;
            }
            
            std::string FleXdWatchdog::getAppName() {
                return m_appName;
            }
            
            int FleXdWatchdog::getDefaultTimeoutValue() {
                return m_defaultTimeout;
            }
            
            void FleXdWatchdog::setDefaultTimeoutValue(int value) {
                m_defaultTimeout = value;
            }
            
            int FleXdWatchdog::getActualTimeoutValue() {
                return m_actualTimeout;
            }
            
            void FleXdWatchdog::setActualTimeoutValue(int value) {
                m_actualTimeout = value;
            }
            
            void FleXdWatchdog::resetTimer() {
                m_actualTimeout = m_defaultTimeout;
            }
            
            bool FleXdWatchdog::isValid() {
                return m_valid;
            }
            
            void FleXdWatchdog::onTimer() {
                this->setActualTimeoutValue(this->getActualTimeoutValue() - 1);
                if (this->m_actualTimeout == 0){
                    sendRestartMsg();
                    this->m_timer.stop();
                }
                FLEX_LOG_INFO("FleX-d Watchdog : ", m_appName, " >> restart in = ", m_actualTimeout, " seconds.\n");
            }
            
            void FleXdWatchdog::sendRestartMsg() {
                uint8_t msgType(flexd::icl::ipc::FleXdIPCMsgTypes::IPCMsg);
                std::string payloadString{m_appName + " restart triggered!"};
                std::vector<uint8_t> payload(payloadString.begin(), payloadString.end());
                auto msgPtr = std::make_shared<flexd::icl::ipc::FleXdIPCMsg>(msgType, std::move(payload));
                m_ipc.send(m_coreAppIpcId, msgPtr);
                m_valid = false;
            }
            
        } // namespace ipc
    } // namespace icl
} // namespace flexd
