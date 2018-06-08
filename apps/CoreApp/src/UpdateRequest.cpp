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
 * File:   UpdateRequest.cpp
 * Author: Peter Kocity
 *
 * Created on February 16, 2018, 2:47 PM
 */


#include "UpdateRequest.h"
#include "Visitor.h"
#include "FleXdLogger.h"


namespace flexd {
    namespace core {

        UpdateRequest::UpdateRequest(const std::string& name, const std::string& ver, const std::string& path)
        : iCoreAppRequest(RqstType::Enum::update, nullptr, nullptr, name, ver),
        m_path(path) {
            FLEX_LOG_TRACE("UpdateRequest: Request path: ",m_path);
        }

        const std::string& UpdateRequest::getBase64() {
            base::BinStream b(m_path);
            FLEX_LOG_TRACE("UpdateRequest::getBase64(): ");
            return b.getBase64();
        }

        void UpdateRequest::accept(Visitor &v) {
            FLEX_LOG_TRACE("UpdateRequest::accept(): Visiting");
            v.visit(this);
        }

    }
}