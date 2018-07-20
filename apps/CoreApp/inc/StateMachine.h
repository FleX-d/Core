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
 * File:   StateMachine.h
 * Author: Peter Kocity
 *
 * Created on April 20, 2018, 8:50 AM
 */

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>

namespace flexd {
    namespace core {

        class StateMachine: public std::enable_shared_from_this<StateMachine> {
        public:
            StateMachine();
            virtual ~StateMachine();

            virtual std::shared_ptr<StateMachine> start();
            virtual std::shared_ptr<StateMachine> stop();
            virtual std::shared_ptr<StateMachine> update();
            virtual std::shared_ptr<StateMachine> freeze();
            virtual std::shared_ptr<StateMachine> unfreeze();

            virtual int getState();

            StateMachine(const StateMachine& orig) = default;
            StateMachine& operator=(const StateMachine& orig) = default;
        };
        typedef std::shared_ptr<StateMachine> StateMachine_t;

        class StoppedState : public StateMachine {
        public:
            StoppedState();
            virtual ~StoppedState() = default;

            virtual StateMachine_t start() override;
            virtual StateMachine_t update() override;
            virtual int getState() override;

            StoppedState(const StoppedState& orig) = default;
            StoppedState& operator=(const StoppedState& orig) = default;
        };

        class RunningState : public StateMachine {
        public:
            RunningState();
            virtual ~RunningState() = default;

            virtual StateMachine_t stop() override;
            virtual StateMachine_t freeze() override;
            virtual int getState() override;

            RunningState(const RunningState& orig) = default;
            RunningState& operator=(const RunningState& orig) = default;
        };

        class FrozenState : public StateMachine {
        public:
            FrozenState();
            virtual ~FrozenState() = default;

            virtual StateMachine_t stop() override;
            virtual StateMachine_t unfreeze() override;
            virtual int getState() override;

            FrozenState(const FrozenState& orig) = default;
            FrozenState& operator=(const FrozenState& orig) = default;
        };

        class UpdatingState : public StateMachine {
        public:
            UpdatingState();
            virtual ~UpdatingState() = default;

            virtual StateMachine_t stop() override;
            virtual int getState() override;

            UpdatingState(const UpdatingState& orig) = default;
            UpdatingState& operator=(const UpdatingState& orig) = default;
        };

    }
}

#endif /* STATEMACHINE_H */

