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
 * File:   FleXdWatchdogEvent.h
 * Author: Branislav Podkonicky
 *
 * Created on July 29, 2018, 09:45 AM
 */

#ifndef FLEXDWATCHDOGEVENT_H
#define FLEXDWATCHDOGEVENT_H

#include "FleXdEpoll.h"
#include <vector>

struct itimerspec;
namespace flexd {
    namespace icl {
        namespace ipc {
            
            class FleXdWatchdogEvent {
            private:
                typedef std::function<void(FleXdEpoll& poller, int, int*, std::vector<std::string>)> WatchdogEventFunction;
            public:
                explicit FleXdWatchdogEvent(FleXdEpoll& poller, int fd, int * wdp, std::vector<std::string> folders, WatchdogEventFunction onEvent = nullptr);
                virtual ~FleXdWatchdogEvent();
            
                bool init();
                bool uninit();
                bool trigger();
                int getFd() const;
                void setOnEvent(WatchdogEventFunction onEvent);
                
                FleXdWatchdogEvent(const FleXdEpoll&) = delete;
                FleXdWatchdogEvent& operator=(const FleXdEpoll&) = delete;
                FleXdWatchdogEvent(const FleXdEpoll&&) = delete;
                FleXdWatchdogEvent& operator=(const FleXdEpoll&&) = delete;

                virtual void onEvent() {}
            
            private:
                void onEvent(FleXdEpoll::Event e);
            
            protected:
                FleXdEpoll& m_poller;
                WatchdogEventFunction m_onEvent;
                int m_fd;
                int * m_wd;
                std::vector<std::string> m_folders;
            };
        } // namespace ipc
    } // namespace icl
} // namespace flexd

#endif /* FLEXDEVENT_H */
