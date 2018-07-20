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
#include <FleXdLogger.h>


namespace flexd {
    namespace core {

        StateMachine::StateMachine() {
            FLEX_LOG_TRACE("StateMachine ctor");
        }

        StateMachine::~StateMachine() {
            FLEX_LOG_TRACE("StateMachine dtor");
        }

        StateMachine_t StateMachine::start() {
            return shared_from_this();
        }

        StateMachine_t StateMachine::stop() {
            return shared_from_this();
        }

        StateMachine_t StateMachine::update() {
            return shared_from_this();
        }

        StateMachine_t StateMachine::freeze() {
            return shared_from_this();
        }

        StateMachine_t StateMachine::unfreeze() {
            return shared_from_this();
        }

        int StateMachine::getState() {
            return flexd::core::JobState::undefined;
        }

        /*
         * StoppedState
         */
        StoppedState::StoppedState() {
            FLEX_LOG_TRACE("StoppedState::StoppedState()");
        }

        StateMachine_t StoppedState::start() {
            return std::shared_ptr<StateMachine>(new RunningState);
        }

        StateMachine_t StoppedState::update() {
            return std::shared_ptr<StateMachine>(new UpdatingState);
        }

        int StoppedState::getState() {
            return flexd::core::JobState::stopped;
        }

        /*
         * RunningState
         */
        RunningState::RunningState() {
            FLEX_LOG_TRACE("RunningState::RunningState()");
        }

        StateMachine_t RunningState::stop() {
            return std::shared_ptr<StateMachine>(new StoppedState);
        }

        StateMachine_t RunningState::freeze() {
            return std::shared_ptr<StateMachine>(new FrozenState);
        }

        int RunningState::getState() {
            return flexd::core::JobState::running;
        }

        /*
         * FrozenState
         */
        FrozenState::FrozenState() {
            FLEX_LOG_TRACE("FrozenState::FrozenState()");
        }

        StateMachine_t FrozenState::stop() {
            return std::shared_ptr<StateMachine>(new StoppedState);
        }

        StateMachine_t FrozenState::unfreeze() {
            return std::shared_ptr<StateMachine>(new RunningState);
        }

        int FrozenState::getState() {
            return flexd::core::JobState::pending;
        }

        /*
         * UpdatingState
         */
        UpdatingState::UpdatingState() {
            FLEX_LOG_TRACE("UpdatingState::UpdatingState()");
        }

        StateMachine_t UpdatingState::stop() {
            return std::shared_ptr<StateMachine>(new StoppedState);
        }

        int UpdatingState::getState(){
            return flexd::core::JobState::busy;
        }

    }
}
