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
#include <FleXdLogger.h>
#include <sys/inotify.h>
#include <unistd.h>

namespace flexd {
    namespace icl {
        namespace ipc {
            
            FleXdWatchdog::FleXdWatchdog(FleXdEpoll& poller, IPCInterface& ipc, int fd, std::string path)
                : m_poller(poller),
                  m_timer(m_poller, 1, 0, true, [&]{ this->onTimer(); }),
                  m_ipc(ipc),
                  m_path(path),
                  m_appName(""),
                  m_fd(fd),
                  m_timeout(60),
                  m_valid(true) {
                std::vector<std::string> pathVector;
                pathVector.push_back(path);
                int position =  m_path.rfind("/");
                std::string pidFileName = m_path.substr(position+1);
                m_appName = pidFileName.substr(0, pidFileName.length() - 4);
                m_timer.start(); 
                FLEX_LOG_INFO("FleX-d Watchdog : ", m_appName, " >> started.\n");
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
            
            int FleXdWatchdog::getTimeoutValue() {
                return m_timeout;
            }
            
            void FleXdWatchdog::setTimeoutValue(int value) {
                m_timeout = value;
            }
            
            void FleXdWatchdog::resetTimer() {
                m_timeout = 60;
            }
            
            bool FleXdWatchdog::isValid() {
                return m_valid;
            }
            
            void FleXdWatchdog::onTimer() {
                this->setTimeoutValue(this->getTimeoutValue() - 1);
                if (this->m_timeout == 0){
                    sendRestartMsg();
                    this->m_timer.stop();
                }
                FLEX_LOG_INFO("FleX-d Watchdog : ", m_appName, " >> restart in = ", m_timeout, " seconds.\n");
            }
            
            void FleXdWatchdog::sendRestartMsg() {
                std::string payloadString{"FleX-d Watchdog : "};
                uint8_t msgType(flexd::icl::ipc::FleXdIPCMsgTypes::IPCMsg);
                payloadString += m_appName + " RESTART TRIGGERED!";
                std::vector<uint8_t> payload(payloadString.begin(), payloadString.end());
                auto msgPtr = std::make_shared<flexd::icl::ipc::FleXdIPCMsg>(msgType, std::move(payload));
                m_ipc.send(111, msgPtr);
                FLEX_LOG_INFO("FleX-d Watchdog : ", m_appName, " >> RESTART TRIGGERED!\n");
                m_valid = false;
            }
        } // namespace ipc
    } // namespace icl
} // namespace flexd
