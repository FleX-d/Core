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
 * File:   FleXdWatchdog.h
 * Author: Branislav Podkonicky
 *
 * Created on July 29, 2018, 09:45 AM
 */

#ifndef FLEXDWATCHDOG_H
#define FLEXDWATCHDOG_H

#include "IPCInterface.h"
#include <FleXdEpoll.h>
#include <FleXdTimer.h>

namespace flexd {
    namespace icl {
        namespace ipc {
            
            class FleXdWatchdog {  
            public:
                explicit FleXdWatchdog(FleXdEpoll& poller, IPCInterface& ipc, std::string path, int timeout, int coreAppIpcId);
                virtual ~FleXdWatchdog();
                std::string getPath();
                std::string getAppName();
                int  getDefaultTimeoutValue();
                void setDefaultTimeoutValue(int value);
                int  getActualTimeoutValue();
                void setActualTimeoutValue(int value);
                void resetTimer();
                bool isValid();
            
            private:
                void onTimer();
                void sendRestartMsg();
            
            protected:
                FleXdEpoll& m_poller;
                FleXdTimer m_timer;
                IPCInterface& m_ipc;
                std::string m_path;
                std::string m_appName;
                int m_defaultTimeout;
                int m_actualTimeout;
                int m_coreAppIpcId;
                bool m_valid;
            };
            
        } // namespace ipc
    } // namespace icl
} // namespace flexd

#endif /* FLEXDWATCHDOG_H */

