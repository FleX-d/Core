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
#include <sys/inotify.h>
#include <unistd.h>

namespace flexd {
    namespace icl {
        namespace ipc {
            
            FleXdWatchdog::FleXdWatchdog(FleXdEpoll& poller, int fd, std::string path)
            : m_poller(poller),
            m_fd(fd),
            m_path(path),
            m_timeout(60),
            m_timer(m_poller, 1, 0, true, [&]{ this->onTimer(); }){
                m_wd = inotify_add_watch(fd, path.c_str(), IN_DELETE);
                if (m_wd == -1) {
                    fprintf(stderr, "Cannot watch '%s'\n", path.c_str());
                    perror("inotify_add_watch");
                    exit(EXIT_FAILURE);
                }
                std::vector<std::string> pathVector;
                pathVector.push_back(path);
                
                using namespace std::placeholders; 
                m_event = std::make_unique<FleXdWatchdogEvent> (m_poller, m_fd, &m_wd, pathVector, 
                        std::bind(&FleXdWatchdog::onEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
                m_event->init();
                m_timer.start(); 
                sendCreateMsg();
            }
            
            FleXdWatchdog::~FleXdWatchdog() {
                //inotify_rm_watch(m_fd, m_wd);
                m_timer.stop();
                sendDeleteMsg();
            }
            
            std::string FleXdWatchdog::getPath(){
                return m_path;
            }
            
            void FleXdWatchdog::onTimer(){
                this->setTimeoutValue(this->getTimeoutValue() - 1);
                printf("FleXdWatchdog::onTimer() timeout value = %d \n", m_timeout);
                if (this->m_timeout == 0){
                    sendRestartMsg();
                    this->m_timer.stop();
                }
            }
            
            int FleXdWatchdog::getTimeoutValue(){
                return m_timeout;
            }
            
            void FleXdWatchdog::setTimeoutValue(int value) {
                m_timeout = value;
            }
            
            void FleXdWatchdog::onEvent(FleXdEpoll& poller, int fd, int * wd, std::vector<std::string> folders) {             
                char buf[4096]
                        __attribute__((aligned(__alignof__(struct inotify_event))));
                const struct inotify_event *event;
                ssize_t len;
                char *ptr;
                
                for (;;) {
                    len = read(fd, buf, sizeof buf);

                    if (len == -1 && errno != EAGAIN) {
                        perror("read");
                        exit(EXIT_FAILURE);
                    }
                    
                    if (len <= 0)
                        break;
                    
                    for (ptr = buf; ptr < buf + len;
                            ptr += sizeof (struct inotify_event) +event->len) {
                        event = (const struct inotify_event *) ptr;

                        if (event->mask & IN_DELETE) {
                            printf("IN_DELETE : ");
                            this->m_timeout = 60;
                        }
                        if (event->mask & IN_MODIFY) {
                            printf("IN_MODIFY: ");
                            this->m_timeout = 60;
                        }
                    }
                }
            }
            
            void FleXdWatchdog::sendCreateMsg(){
                printf("FleXdWatchdog::sendCreateMsg \n");
            }
            
            void FleXdWatchdog::sendDeleteMsg(){
                printf("FleXdWatchdog::sendDeleteMsg \n");
            }
            
            void FleXdWatchdog::sendRestartMsg(){
                printf("FleXdWatchdog::sendRestartMsg \n");
            }
            
        } // namespace ipc
    } // namespace icl
} // namespace flexd
