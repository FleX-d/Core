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
 * File:   FleXdWatchdogEvent.cpp
 * Author: Branislav Podkonicky
 *
 * Created on July 29, 2018, 09:45 AM
 */

#include "FleXdWatchdogEvent.h"
#include "IPCInterface.h"
#include <FleXdLogger.h>
#include <sys/eventfd.h>

namespace flexd {
    namespace icl {
        namespace ipc {
            
            FleXdWatchdogEvent::FleXdWatchdogEvent(FleXdEpoll& poller, IPCInterface& ipc, std::vector<std::string> folders, int fd, int * wdp, WatchdogEventFunction onEvent)
            : m_poller(poller),
              m_ipc(ipc), 
              m_folders(folders),
              m_fd(fd),
              m_wd(wdp), 
              m_onEvent(onEvent)
            {
                fflush(stdout);
            }
            
            FleXdWatchdogEvent::~FleXdWatchdogEvent() {
                fflush(stdout);
                if (m_fd != -1) {
                    //m_poller.rmEvent(m_fd);
                }
            }
            
            bool FleXdWatchdogEvent::init() {
                FLEX_LOG_INFO("FleX-d Watchdog >> Event init was entered: ", m_fd, " \n");
                fflush(stdout);
                if (m_fd > -1) {
                    m_poller.addEvent(m_fd, [this](FleXdEpoll::Event e) {
                        onEvent(e);
                    });
                    return true;
                }
                return false;
            }
            
            bool FleXdWatchdogEvent::uninit() {
                fflush(stdout);
                if (m_fd == -1) {
                    return true;
                }
                if (m_poller.rmEvent(m_fd)) {
                    m_fd = -1;
                    return true;
                };
                return false;
            }
            
            bool FleXdWatchdogEvent::trigger() {
                fflush(stdout);
                if (m_fd != -1) {
                    if (::eventfd_write(m_fd, 1) == 0) {
                        return true;
                    }
                }
                return false;
            }
            
            int FleXdWatchdogEvent::getFd() const {
                fflush(stdout);
                return m_fd;
            }
            
            void FleXdWatchdogEvent::setOnEvent(WatchdogEventFunction onEvent) {
                fflush(stdout);
                m_onEvent = onEvent;
            }
            
            void FleXdWatchdogEvent::onEvent(FleXdEpoll::Event e) {
                FLEX_LOG_INFO("---------------FleXdWatchdogEvent::onEvent triggered --------------- \n");
                fflush(stdout);
                if (e.type == EpollEvent::EpollIn && e.fd == m_fd) {
                    fflush(stdout);
                    m_onEvent(m_poller, m_ipc, m_fd, m_wd, m_folders);
                }
            }
        } // namespace ipc
    } // namespace icl
} // namespace flexd
