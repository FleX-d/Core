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
 * File:   IPCClient.h
 * Author: Peter Kocity
 *
 * Created on February 5, 2018, 2:33 PM
 */

#ifndef IPCCLIENT_H
#define IPCCLIENT_H


#include "CoreDocIPCInterface.h"
#include "Base64.h"
#include "CoreAppRequestFactory.h"
#include "iCoreAppAck.h"
#include "FleXdLogger.h"
#include "iCoreAppRequest.h"

namespace flexd {
    namespace core {

        class IPCClient : public flexd::gen::Interface {
        public:
            IPCClient(flexd::icl::ipc::FleXdEpoll& poller);
            virtual ~IPCClient() = default;

            void onLambda(iCoreAppRequest& rqst);
            void setOnLambda(std::function<void(iCoreAppRequest&) > onLambda);
            
            IPCClient(const IPCClient&) = delete;
            IPCClient& operator=(const IPCClient&) = delete;

        private:
            virtual void receiveRequestCoreMsg(uint8_t Operation, const std::string& Message, const std::string& AppID);
            void sendAck(const iCoreAppAck& ack);

            virtual void onConnectPeer(uint32_t peerID) override;
            
            std::function<void(iCoreAppRequest&) > m_onLambda;
            CoreAppRequestFactory m_factory;
        };

    }//namespace core
}//namespace flexd

#endif /* IPCCLIENT_H */

