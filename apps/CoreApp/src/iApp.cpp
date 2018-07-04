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
 * File:   iApp.cpp
 * Author: Peter Kocity
 *
 * Created on April 20, 2018, 9:19 AM
 */

#include "iApp.h"

namespace flexd {
    namespace core {

        iApp::iApp() {
            m_state=std::shared_ptr<StateMachine>(new StoppedState);
        }

        iApp::~iApp() {
            FLEX_LOG_TRACE(m_state.use_count());
            m_state.reset();
            /*TODO verifi corect funcionality*/
            FLEX_LOG_TRACE("delete iApp: count of m_state:", m_state.use_count());
        }

        int iApp::getState(){
            return m_state->getState();
        }

        bool iApp::start() {
            StateMachine_t running(m_state->start());
            if (m_state != running) {
                m_state.swap(running);
                running.reset();
                return true;
            }
            running.reset();
            return false;
        }

        bool iApp::update() {
            StateMachine_t updating(m_state->update());
            if (m_state != updating) {
                m_state.swap(updating);
                updating.reset();
                return true;
            }
            updating.reset();
            return false;
        }

        bool iApp::stop() {
            StateMachine_t stopped(m_state->stop());
            if (m_state != stopped) {
                m_state.swap(stopped);
                stopped.reset();
                return true;
            }
            stopped.reset();
            return false;
        }

        bool iApp::freeze() {
            StateMachine_t frozen(m_state->freeze());
            if (m_state != frozen) {
                m_state.swap(frozen);
                frozen.reset();
                return true;
            }
            frozen.reset();
            return false;
        }

        bool iApp::unfreeze() {
            StateMachine_t uf = m_state->unfreeze();
            if (m_state!=uf) {
                m_state.swap(uf);
                uf.reset();
                return true;
            }
            uf.reset();
            return false;
        }
    }
}
