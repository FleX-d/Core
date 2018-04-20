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
 * File:   IPCClient.cpp
 * Author: Peter Kocity
 *
 * Created on February 5, 2018, 2:33 PM
 */

#include "IPCClient.h"
#include "iCoreAppAck.h"
#include "FleXdLogger.h"
#include "iCoreAppRequest.h"


namespace flexd {
    namespace core {

        IPCClient::IPCClient(const std::string& id)
        : iIPCClient(id) {
            FLEX_LOG_INIT("IPCClient");
        }

        void IPCClient::onMessage(const rsm::conn::mqtt::MqttMessage& msg) {
            FLEX_LOG_TRACE("IPCClient::onMessage(): incoming message");
            flexd::icl::JsonObj j(msg.getMessage());
            iCoreAppRequest_t rqst = m_factory.makeRqst(j);
            
            FLEX_LOG_TRACE("IPCClient::onMessage(): seting lamda function");
            std::function<void(const iCoreAppAck&) > onSuccess = std::bind(&IPCClient::sendSuccesAck, this, std::placeholders::_1);
            std::function<void(const iCoreAppAck&) > onError = std::bind(&IPCClient::sendErrorAck, this, std::placeholders::_1);
            rqst->setOnError(onError);
            rqst->setOnSucces(onSuccess);
                   
            /*check if request is not null or invalid, send error ack if is*/
            if(rqst->getType()==flexd::core::RqstType::Enum::undefined){
                FLEX_LOG_DEBUG("IPCClient::onMessage(): bad request");
                rqst->onError(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
            }
            else{
                FLEX_LOG_TRACE("IPCClient::onMessage(): send message to manager to execute");
                /*TODO execute in manager creating order*/
                
                
                
                
                
                
            }
        }
        
        void IPCClient::sendSuccesAck(const iCoreAppAck& ack){
            /*Read from ACK name app and ver and create publis message*/
            if(ack.getType()==RqstAck::Enum::succes){
                std::string publishMsg="{\"Operation Result\": \"True\", \"Message\": \"\", \"AppID\": \""+ ack.getName() +"\"};";
                FLEX_LOG_TRACE("IPCClient::sendSuccesAck(): ", publishMsg);
                publish(publishMsg);
            }
        }
        
        void IPCClient::sendErrorAck(const iCoreAppAck& ack){
            /*Read from ACK name app and ver and create publis message*/
            if(ack.getType()==RqstAck::Enum::fail){
                std::string publishMsg = "{\"Operation Result\": \"False\", \"Message\": \"\", \"AppID\": \""+ ack.getName() +"\"};";
                FLEX_LOG_TRACE("IPCClient::sendErrorAck(): ", publishMsg);
                publish(publishMsg);
            }
        }
    }
}
