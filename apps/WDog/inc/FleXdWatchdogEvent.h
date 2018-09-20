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

#include "IPCInterface.h"
#include <FleXdEpoll.h>
#include <vector>

namespace flexd {
    namespace icl {
        namespace ipc {
            
            class FleXdWatchdogEvent {
            private:
                typedef std::function<void(FleXdEpoll& poller, IPCInterface& ipc, int, int*, std::vector<std::string>)> WatchdogEventFunction;
                void onEvent(FleXdEpoll::Event e);
                
            public:
                explicit FleXdWatchdogEvent(FleXdEpoll& poller, IPCInterface& ipc, std::vector<std::string> folders, int fd, int * wdp, WatchdogEventFunction onEvent = nullptr);
                virtual ~FleXdWatchdogEvent();
                FleXdWatchdogEvent(const FleXdEpoll&) = delete;
                FleXdWatchdogEvent& operator=(const FleXdEpoll&) = delete;
                FleXdWatchdogEvent(const FleXdEpoll&&) = delete;
                FleXdWatchdogEvent& operator=(const FleXdEpoll&&) = delete;
                bool init();
                bool uninit();
                bool trigger();
                int getFd() const;
                void setOnEvent(WatchdogEventFunction onEvent);
                virtual void onEvent() {}
            
            protected:
                FleXdEpoll& m_poller;
                IPCInterface& m_ipc;
                std::vector<std::string> m_folders;
                int m_fd;
                int * m_wd;
                WatchdogEventFunction m_onEvent;
            };
            
        } // namespace ipc
    } // namespace icl
} // namespace flexd

#endif /* FLEXDEVENT_H */
