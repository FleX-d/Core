/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StateMachine.cpp
 * Author: dev
 * 
 * Created on April 20, 2018, 8:50 AM
 */

#include "StateMachine.h"

namespace flexd {
    namespace core {

        StateMachine::StateMachine(){
        }
        
//        StateMachine::~StateMachine(){
            /*TODO control destruct of shared of this*/
//        }

        StateMachine_t StateMachine::starting() {
            return shared_from_this();
//            return this;
        }

        StateMachine_t StateMachine::updating() {
//            return this;
            return shared_from_this();
        }

        StateMachine_t StateMachine::stoping() {
//            return this;
            return shared_from_this();
        }

        StateMachine_t StateMachine::freezing() {
//            return this;
            return shared_from_this();
        }

        StateMachine_t StateMachine::unfreezing() {
//            return this;
            return shared_from_this();
        }
        
        int StateMachine::getType(){
            return 0;
        }
        
        Stop::Stop(){
            FLEX_LOG_TRACE("Stop::Stop()");
        }

        StateMachine_t Stop::starting() {
//            return new Run();
            return std::shared_ptr<StateMachine>(new Run);
        }

        StateMachine_t Stop::updating() {
//            return new Update();
            return std::shared_ptr<StateMachine>(new Update);
        }
        
        int Stop::getType(){
            return 1;
        }

        Run::Run(){
            FLEX_LOG_TRACE("Run::Run()");
        }

        StateMachine_t Run::stoping() {
//            return new Stop();
            return std::shared_ptr<StateMachine>(new Stop);
        }

        StateMachine_t Run::freezing() {
//            return new Freez();
            return std::shared_ptr<StateMachine>(new Freez);
        }
        
        int Run::getType(){
            return 2;
        }

        Freez::Freez(){
            FLEX_LOG_TRACE("Freez::Freez()");
        }

        StateMachine_t Freez::stoping() {
//            return new Stop();
            return std::shared_ptr<StateMachine>(new Stop);
        }

        StateMachine_t Freez::unfreezing() {
//            return new Run();
            return std::shared_ptr<StateMachine>(new Run);
        }
        
        int Freez::getType(){
            return 3;
        }

        Update::Update(){
            FLEX_LOG_TRACE("Update::Update()");
        }

        StateMachine_t Update::stoping() {
//            return new Stop();
            return std::shared_ptr<StateMachine>(new Stop);
        }
        
        int Update::getType(){
            return 4;
        }

    }
}