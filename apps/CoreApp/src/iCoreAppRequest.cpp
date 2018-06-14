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
#include "FleXdLogger.h"

namespace flexd {
    namespace core {

        iCoreAppRequest::iCoreAppRequest(RqstType::Enum type, std::function<void(const iCoreAppAck&) > onAck, const std::string& name, const std::string& ver)
        : m_type(type),
        m_onAck(onAck),
        m_name(name),
        m_version(ver) {
            FLEX_LOG_TRACE("iCoreAppRequest: TYPE: ", m_type, " NAME: ", m_name, " VERSION: ", m_version);
        }

        const RqstType::Enum iCoreAppRequest::getType() const {
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

        void iCoreAppRequest::onAck(const iCoreAppAck& ack) {
            FLEX_LOG_TRACE("iCoreAppRequest::onAck(): sending lambda");
            m_onAck(ack);
        }

        void iCoreAppRequest::setOnAck(std::function<void(const iCoreAppAck&) > onAck) {
            FLEX_LOG_TRACE("iCoreAppRequest::setOnAck(): set lambda");
            m_onAck = onAck;
        }

    }
}