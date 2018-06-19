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

//#ifndef STATEMACHINE_H
//#define STATEMACHINE_H

#include "FleXdLogger.h"
#include <memory>

namespace flexd {
    namespace core {
        class StateMachine: public std::enable_shared_from_this<StateMachine> {
        public:
            StateMachine();
            virtual ~StateMachine();
            
            virtual std::shared_ptr<StateMachine> starting();
            virtual std::shared_ptr<StateMachine> updating();
            virtual std::shared_ptr<StateMachine> stoping();
            virtual std::shared_ptr<StateMachine> freezing();
            virtual std::shared_ptr<StateMachine> unfreezing();
            
            virtual int getType();
            
            StateMachine(const StateMachine& orig) = default;
            StateMachine& operator=(const StateMachine& orig) = default;
        private:
            
        };
        typedef std::shared_ptr<StateMachine> StateMachine_t;
        
        
        class Stop : public StateMachine {
        public:
            Stop();
            ~Stop() = default;

            StateMachine_t starting();
            StateMachine_t updating();
            int getType();

            Stop(const Stop& orig) = default;
            Stop& operator=(const Stop& orig) = default;
        private:       
        };
        
        
        
        class Run : public StateMachine {
        public:
            Run();
            ~Run() = default;

            StateMachine_t stoping();
            StateMachine_t freezing();
            int getType();

            Run(const Run& orig) = default;
            Run& operator=(const Run& orig) = default;
        private:
        };
        
        
        
        class Freez : public StateMachine {
        public:
            Freez();
            ~Freez() = default;

            StateMachine_t stoping();
            StateMachine_t unfreezing();
            int getType();

            Freez(const Freez& orig) = default;
            Freez& operator=(const Freez& orig) = default;
        private:
        };
        
        
        
        class Update : public StateMachine {
        public:
            Update();
            ~Update() = default;

            StateMachine_t stoping();
            int getType();

            Update(const Update& orig) = default;
            Update& operator=(const Update& orig) = default;
        private:
        };
    }
}

//#endif /* STATEMACHINE_H */

