/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StateMachine.h
 * Author: dev
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
            virtual ~StateMachine() = default;

//            virtual StateMachine* starting();
//            virtual StateMachine* updating();
//            virtual StateMachine* stoping();
//            virtual StateMachine* freezing();
//            virtual StateMachine* unfreezing();
            
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
        //typedef StateMachine* StateMachine_t;
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

