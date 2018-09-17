/*
Copyright (c) 2017, Globallogic s.r.o.
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
 * File:   IPCInterface.cpp
 * Author: Branislav Podkonicky
 *
 * Created on July 29, 2018, 09:45 AM
 */

#include "IPCInterface.h"
#include <JsonObj.h>
#include <vector>
#include <ctime>
#include <chrono>

IPCInterface::IPCInterface(uint32_t id, flexd::icl::ipc::FleXdEpoll& poller)
: IPCConnector(id, poller, false, true),
m_poller(poller),
m_counter(0) {
}

IPCInterface::~IPCInterface() {
}

void IPCInterface::send(uint32_t peerID, std::shared_ptr<flexd::icl::ipc::FleXdIPCMsg> msg) {
    if (sendMsg(msg, peerID)) {
        m_counter++;
    }
}

void IPCInterface::onBeforeRcvMsg(std::shared_ptr<const flexd::icl::ipc::FleXdIPCMsg> msg) {
    std::cout << "\n message was received: \n";
    for (auto it = msg->getPayload().begin(); it != msg->getPayload().end(); ++it)
        std::cout << ' ' << (int) *it;
    std::cout << std::endl;
};

uint32_t IPCInterface::getTimestamp() {
    std::chrono::time_point<std::chrono::system_clock> p;
    p = std::chrono::system_clock::now();
    std::time_t time = std::chrono::duration_cast<std::chrono::milliseconds>(p.time_since_epoch()).count();
    return static_cast<uint32_t> (time);
}
