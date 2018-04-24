/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   iApp.cpp
 * Author: dev
 * 
 * Created on April 20, 2018, 9:19 AM
 */

#include "iApp.h"

namespace flexd {
    namespace core {

        iApp::iApp() {
//            m_state = new Stop();
            m_state=std::shared_ptr<StateMachine>(new Stop);
        }

        iApp::~iApp() {
//            delete m_state;
        }
        
        int iApp::getState(){
            return m_state->getType();
        }

        bool iApp::starting() {
            StateMachine_t s = m_state->starting();
            if (m_state!=s) {
                m_state = s;
                return true;
            }
            return false;
        }

        bool iApp::updating() {
            StateMachine_t u = m_state->updating();
            if (m_state!=u) {
                m_state = u;
                return true;
            }
            return false;
        }

        bool iApp::stoping() {
            StateMachine_t st = m_state->stoping();
            if (m_state!=st) {
                m_state = st;
                return true;
            }
            return false;
        }

        bool iApp::freezing() {
            StateMachine_t f = m_state->freezing();
            if (m_state!=f) {
                m_state = f;
                return true;
            }
            return false;
        }

        bool iApp::unfreezing() {
            StateMachine_t uf = m_state->unfreezing();
            if (m_state!=uf) {
                m_state = uf;
                return true;
            }
            return false;
        }
    }
}