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


namespace flexd {
    namespace core {

        IPCClient::IPCClient(flexd::icl::ipc::FleXdEpoll& poller)
        : IPCInterface(poller)
        {
        }
        
        IPCClient::~IPCClient()
        {
        }
        
        void IPCClient::onLambda(iCoreAppRequest& rqst){
            m_onLambda(rqst);
        }
        
        void IPCClient::setOnLambda(std::function<void(iCoreAppRequest&) > onLambda){
            m_onLambda=onLambda;
        }

        void IPCClient::receiveRequestCoreMsg(uint8_t Operation, const std::string& Message, const std::string& AppID) 
        {
            FLEX_LOG_TRACE("IPCClient::receiveRequestCoreMsg(): incoming message ", Operation);

            iCoreAppRequest_t rqst = m_factory.makeRqst(Operation, Message, AppID);
            
            FLEX_LOG_TRACE("IPCClient::receiveRequestCoreMsg(): seting lamda function");
            std::function<void(const iCoreAppAck&) > onAck = std::bind(&IPCClient::sendAck, this, std::placeholders::_1);
            rqst->setOnAck(onAck);
                   
            /*check if request is not null or invalid, send error ack if is*/
            if(rqst->getType()==flexd::core::RqstType::Enum::undefined){
                FLEX_LOG_DEBUG("IPCClient::receiveRequestCoreMsg(): bad request");
                rqst->onAck(iCoreAppAck(RqstAck::Enum::fail, rqst->getName(), rqst->getVersion()));
            }
            else{
                FLEX_LOG_TRACE("IPCClient::receiveRequestCoreMsg(): send message to manager to execute");
                /*execute in manager using sending lambda*/
                onLambda(*rqst);
            }
        }
        
//******************************************TODO - Temporary solution *************************************************  

        void IPCClient::receiveRequestCoreSegmented(uint8_t segment, uint8_t count, const std::string& PayloadMsg)
        {
           FLEX_LOG_TRACE("IPCClient::receiveRequestCoreSegmented(): incoming message segment ", segment, "/", count);

           m_segmentBuffer += PayloadMsg;
           if(segment == count){               
               flexd::icl::JsonObj json(m_segmentBuffer);
               uint8_t Operation;
               std::string Message;
               std::string AppID;

               if(json.exist("/Operation")){
                   json.get<uint8_t>("Operation", Operation);
               }
               if(json.exist("/Message")){
                   json.get<std::string>("/Message", Message);
               }
               if(json.exist("/AppID")){
                   json.get<std::string>("/AppID", AppID);
               }

               receiveRequestCoreMsg(Operation, Message, AppID);

               m_segmentBuffer.clear();
           }
       }
 //**************************************************************************************************************************       
        
        void IPCClient::onConnectPeer(uint32_t peerID) 
        {
        }
        
        
        void IPCClient::sendAck(const iCoreAppAck& ack){
            /*Read from ACK name app and ver and create publis message*/
            if(ack.getType()==RqstAck::Enum::succes){
                sendRequestCoreAckMsg(true, " ", ack.getName());
                FLEX_LOG_TRACE("IPCClient::sendSuccesAck(): ", true, " ", ack.getName());
            }
            
            if(ack.getType()==RqstAck::Enum::fail){
                sendRequestCoreAckMsg(false, " ", ack.getName());
                FLEX_LOG_TRACE("IPCClient::sendErrorAck(): ", false, " ", ack.getName());
            }
        }
    }
}
