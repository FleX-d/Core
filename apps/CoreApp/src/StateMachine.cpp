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
 * File:   StateMachine.cpp
 * Author: Peter Kocity
 * 
 * Created on April 20, 2018, 8:50 AM
 */

#include "StateMachine.h"
#include "CoreAppTypes.h"


namespace flexd {
    namespace core {

        StateMachine::StateMachine(){
        }
        
        StateMachine::~StateMachine(){
            FLEX_LOG_TRACE("delete stateMachine");
        }

        StateMachine_t StateMachine::starting() {
            return shared_from_this();
        }

        StateMachine_t StateMachine::updating() {
            return shared_from_this();
        }

        StateMachine_t StateMachine::stoping() {
            return shared_from_this();
        }

        StateMachine_t StateMachine::freezing() {
            return shared_from_this();
        }

        StateMachine_t StateMachine::unfreezing() {
            return shared_from_this();
        }
        
        int StateMachine::getType(){
            return flexd::core::JobState::undefined;
        }
        
        Stop::Stop(){
            FLEX_LOG_TRACE("Stop::Stop()");
        }

        StateMachine_t Stop::starting() {
            return std::shared_ptr<StateMachine>(new Run);
        }

        StateMachine_t Stop::updating() {
            return std::shared_ptr<StateMachine>(new Update);
        }
        
        int Stop::getType(){
            return flexd::core::JobState::busy;
        }

        Run::Run(){
            FLEX_LOG_TRACE("Run::Run()");
        }

        StateMachine_t Run::stoping() {
            return std::shared_ptr<StateMachine>(new Stop);
        }

        StateMachine_t Run::freezing() {
            return std::shared_ptr<StateMachine>(new Freez);
        }
        
        int Run::getType(){
            return flexd::core::JobState::running;
        }

        Freez::Freez(){
            FLEX_LOG_TRACE("Freez::Freez()");
        }

        StateMachine_t Freez::stoping() {
            return std::shared_ptr<StateMachine>(new Stop);
        }

        StateMachine_t Freez::unfreezing() {
            return std::shared_ptr<StateMachine>(new Run);
        }
        
        int Freez::getType(){
            return flexd::core::JobState::peding;
        }

        Update::Update(){
            FLEX_LOG_TRACE("Update::Update()");
        }

        StateMachine_t Update::stoping() {
            return std::shared_ptr<StateMachine>(new Stop);
        }
        
        int Update::getType(){
            return flexd::core::JobState::busy;
        }

    }
}