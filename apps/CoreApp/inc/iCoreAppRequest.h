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
 * File:   CoreAppRequest.h
 * Author: Peter Kocity
 *
 * Created on February 8, 2018, 1:51 PM
 */

#include "CoreAppTypes.h"
#include "iCoreAppAck.h"
#include <functional>
#include <string>

#ifndef ICOREAPPREQUEST_H
#define ICOREAPPREQUEST_H

namespace flexd {
    namespace core {

        class Visitor;

        /*AppName /<path>/<for contaier>/<appName>
         example /RSM/curl -> RSM-lxc container + curl app name */
        class iCoreAppRequest {
        public:
            explicit iCoreAppRequest(RqstType::Enum type, std::function<void(const iCoreAppAck&) > onAck, const std::string& name, const std::string& ver);
            virtual ~iCoreAppRequest() = default;

            const RqstType::Enum getType() const;
            const std::string& getName() const;
            const std::string& getVersion() const;

            void onAck(const iCoreAppAck& ack);

            virtual void accept(Visitor& v) = 0; /*TODO*/

            void setOnAck(std::function<void(const iCoreAppAck&) > onAck);

            iCoreAppRequest(const iCoreAppRequest&) = default;
            iCoreAppRequest& operator=(const iCoreAppRequest&) = default;
        private:
            const RqstType::Enum m_type;
            std::function<void(const iCoreAppAck&) > m_onAck;
            const std::string m_name;
            const std::string m_version;
        };
        typedef iCoreAppRequest* iCoreAppRequest_t;
    } // namespace core
} // namespace flexd


#endif /* ICOREAPPREQUEST_H */
