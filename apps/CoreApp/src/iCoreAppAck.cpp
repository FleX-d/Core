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
 * File:   iCoreAppAck.cpp
 * Author: Peter Kocity
 *
 * Created on February 16, 2018, 2:20 PM
 */

#include "iCoreAppAck.h"
#include "FleXdLogger.h"

namespace flexd {
    namespace core {

        iCoreAppAck::iCoreAppAck(RqstAck::Enum ack, const std::string& name, const std::string& ver, const std::string& msg)
        : m_ack(ack),
        m_name(name),
        m_version(ver),
        m_message(msg)
        {
            FLEX_LOG_TRACE("iCoreAppAck: NAME: ",m_name, " VERSION: ", m_version, " Type: ", m_ack);
        }

        const RqstAck::Enum iCoreAppAck::getType() const {
            FLEX_LOG_TRACE("iCoreAppAck::getType(): ",m_ack);
            return m_ack;
        }

        const std::string& iCoreAppAck::getName() const {
            FLEX_LOG_TRACE("iCoreAppAck::getName(): ",m_name);
            return m_name;
        }

        const std::string& iCoreAppAck::getVersion() const {
            FLEX_LOG_TRACE("iCoreAppAck::getVersion(): ",m_version);
            return m_version;
        }

        const std::string& iCoreAppAck::getMessage() const {
            FLEX_LOG_TRACE("iCoreAppAck::getMessage(): ",m_message);
            return m_message;
        }

    }
}
