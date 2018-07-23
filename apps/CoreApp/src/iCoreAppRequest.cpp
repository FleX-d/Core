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
 * File:   CoreAppRequest.cpp
 * Author: Peter Kocity
 *
 * Created on February 8, 2018, 1:51 PM
 */

#include "iCoreAppRequest.h"
#include <FleXdLogger.h>

namespace flexd {
    namespace core {

        iCoreAppRequest::iCoreAppRequest(flexd::icl::ipc::FleXdEpoll& rqstPoller, RqstType::Enum type, const std::string& name, const std::string& ver, time_t timeout)
        : flexd::icl::ipc::FleXdEvent(rqstPoller),
          m_timer(timeout > 0 ? std::make_unique<flexd::icl::ipc::FleXdTimer>(rqstPoller, timeout, 0, false, std::bind(&iCoreAppRequest::onTimer, this)) : nullptr),
          m_ack(nullptr),
          m_type(type),
          m_name(name),
          m_version(ver),
          m_onRequestDone(nullptr),
          m_onAck(nullptr),
          m_ackMutex(),
          m_done(false) {
            FLEX_LOG_TRACE("iCoreAppRequest: TYPE: ", m_type, " NAME: ", m_name, " VERSION: ", m_version);
        }

        iCoreAppRequest::~iCoreAppRequest() {
        }

        RqstType::Enum iCoreAppRequest::getType() const {
            FLEX_LOG_TRACE("iCoreAppRequest::getType(): ", m_type);
            return m_type;
        }

        const std::string& iCoreAppRequest::getName() const {
            FLEX_LOG_TRACE("iCoreAppRequest::getName(): ", m_name);
            return m_name;
        }

        const std::string& iCoreAppRequest::getVersion() const {
            FLEX_LOG_TRACE("iCoreAppRequest::getVersion(): ", m_version);
            return m_version;
        }

        bool iCoreAppRequest::isDone() const {
            FLEX_LOG_TRACE("iCoreAppRequest::isDone(): ", m_done.load());
            return m_done.load();
        }

        bool iCoreAppRequest::setTimeout(flexd::icl::ipc::FleXdEpoll& rqstPoller, time_t timeout) {
            FLEX_LOG_TRACE("iCoreAppRequest::setTimeout(): ", timeout);
            if (!m_timer) {
                if (timeout > 0) {
                    m_timer = std::make_unique<flexd::icl::ipc::FleXdTimer>(rqstPoller, timeout, 0, false, std::bind(&iCoreAppRequest::onTimer, this));
                }
                return true;
            } else {
                FLEX_LOG_DEBUG("iCoreAppRequest::setTimeout(): timer already set");
            }
            return false;
        }

        bool iCoreAppRequest::prepareRqst() {
            FLEX_LOG_TRACE("iCoreAppRequest::prepareRqst(): preparing request for processing");
            if (init()) {
                if (m_timer) {
                    if (m_timer->start()) {
                        return true;
                    } else {
                        FLEX_LOG_DEBUG("iCoreAppRequest::prepareRqst(): timer start failed");
                    }
                } else {
                    return true;
                }
            } else {
                FLEX_LOG_DEBUG("iCoreAppRequest::prepareRqst(): event init failed");
            }
            return false;
        }

        void iCoreAppRequest::setAck(RqstAck::Enum ack, const std::string& msg /*= ""*/) {
            FLEX_LOG_TRACE("iCoreAppRequest::setAck(): ", ack, msg);
            std::lock_guard<std::mutex> lock(m_ackMutex);
            if (!m_ack) {
                m_ack = std::make_unique<iCoreAppAck>(ack, m_name, m_version, msg);
            }
        }

        void iCoreAppRequest::onAck(const iCoreAppAck& ack) {
            FLEX_LOG_TRACE("iCoreAppRequest::onAck(): sending acknowledge");
            if (m_onAck) {
                m_onAck(ack);
            }
        }

        void iCoreAppRequest::setOnAck(std::function<void(const iCoreAppAck&)> onAck) {
            FLEX_LOG_TRACE("iCoreAppRequest::setOnAck(): setting acknowledge");
            m_onAck = onAck;
        }

        void iCoreAppRequest::onRequestDone(bool result) {
            if (m_onRequestDone) {
                FLEX_LOG_TRACE("iCoreAppRequest::onRequestDone(): executing post action");
                m_onRequestDone(result);
            } else {
                FLEX_LOG_TRACE("iCoreAppRequest::onRequestDone(): no post action");
            }
        }

        void iCoreAppRequest::setOnRequestDone(std::function<void(bool)> onRequestDone) {
            FLEX_LOG_TRACE("iCoreAppRequest::setOnRequestDone(): setting post action");
            m_onRequestDone = onRequestDone;
        }

        void iCoreAppRequest::onEvent() {
            FLEX_LOG_TRACE("iCoreAppRequest::onEvent()");
            if (m_timer && !m_timer->stop()) {
                FLEX_LOG_ERROR("iCoreAppRequest::onEvent(): cannot stop timer");
            }
            {
                std::lock_guard<std::mutex> lock(m_ackMutex);
                onRequestDone(m_ack ? (m_ack->getType() == RqstAck::Enum::success) : false);
                onAck(m_ack ? *m_ack : iCoreAppAck());
            }
            m_done.store(true);
        }

        void iCoreAppRequest::onTimer() {
            FLEX_LOG_TRACE("iCoreAppRequest::onTimer()");
            if (!flexd::icl::ipc::FleXdEvent::uninit()) {
                FLEX_LOG_ERROR("iCoreAppRequest::onTimer(): cannot uninit event");
            }
            {
                std::lock_guard<std::mutex> lock(m_ackMutex);
                onRequestDone(false);
                onAck(iCoreAppAck(RqstAck::Enum::fail, m_name, m_version, COREAPP_RQST_TIMEOUT_MESSAGE));
            }
            m_done.store(true);
        }
    }
}
