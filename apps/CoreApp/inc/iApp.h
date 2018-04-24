/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   iApp.h
 * Author: dev
 *
 * Created on April 20, 2018, 9:19 AM
 */

#ifndef IAPP_H
#define IAPP_H

#include "FleXdLogger.h"
#include "StateMachine.h"

namespace flexd {
    namespace core {
        
        class iApp {
        public:
            iApp();
            virtual ~iApp();
            
            int getState();

            virtual bool starting();
            virtual bool updating();
            virtual bool stoping();
            virtual bool freezing();
            virtual bool unfreezing();

            iApp(const iApp& orig) = default;
            iApp& operator=(const iApp& orig) = default;

        private:
            StateMachine_t m_state;
        };
    }
}

#endif /* IAPP_H */

