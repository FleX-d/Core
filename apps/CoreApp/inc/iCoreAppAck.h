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
 * File:   iCoreAppAck.h
 * Author: Peter Kocity
 *
 * Created on February 16, 2018, 2:20 PM
 */

#ifndef ICOREAPPACK_H
#define ICOREAPPACK_H

#include "CoreAppTypes.h"
#include <string>



namespace flexd {
    namespace core {

        class iCoreAppAck {
        public:
            explicit iCoreAppAck(RqstAck::Enum ack, const std::string& name, const std::string& ver, const std::string& msg = "");
            virtual ~iCoreAppAck() = default;

            virtual const RqstAck::Enum getType() const;
            virtual const std::string& getName() const;
            virtual const std::string& getVersion() const;
            virtual const std::string& getMessage() const;

            iCoreAppAck(iCoreAppAck&) = default;
            iCoreAppAck& operator=(const iCoreAppAck&) = default;
        private:
            RqstAck::Enum m_ack;
            std::string m_name;
            std::string m_version;
            std::string m_message;
        };
    }
}
#endif /* ICOREAPPACK_H */

