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
 * File:   iIPCClient.cpp
 * Author: Peter Kocity
 *
 * Created on February 16, 2018, 1:46 PM
 */


#include "iIPCClient.h"
#include "FleXdLogger.h"

namespace flexd {
    namespace core {

        iIPCClient::iIPCClient(const std::string& id)
        : rsm::conn::mqtt::iMosquittoClient(id, rsm::conn::mqtt::MosquittoSetting("127.0.0.1")) {
            subscribeTopic("coreapp/in");
            FLEX_LOG_INIT("iIPCClient");
            FLEX_LOG_TRACE("iIPCClient(): CLIENT ID: ",id);
        }

        bool iIPCClient::addClient(const std::string& request) {
            publishMessage(rsm::conn::mqtt::MqttMessage("testapp/in", request));
            FLEX_LOG_TRACE("iIPCClient::addClient(): ", request);
            return true;
        }

        bool iIPCClient::sendRequest(const std::string& request) {
            publishMessage(rsm::conn::mqtt::MqttMessage("testapp/in", request));
            FLEX_LOG_TRACE("iIPCClient::sendRequest(): ",request);
            return true;
        }

        bool iIPCClient::publish(const std::string& message) {
            publishMessage(rsm::conn::mqtt::MqttMessage("testapp/in", message));
            FLEX_LOG_TRACE("iIPCClient::publish(): ",message);
            return true;
        }

        void iIPCClient::echo(const std::string& msg) const {
            FLEX_LOG_TRACE("iIPCClient::echo(): ",msg);
            std::cout << "IPC: " << msg << std::endl;
        }

    }//namespace core
}//namespace flexd