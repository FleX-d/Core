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
 * File:   InstallRequest.h
 * Author: Peter Kocity
 *
 * Created on February 8, 2018, 1:51 PM
 */

#include "iCoreAppRequest.h"
#include "Base64.h"
#include <string>


#ifndef INSTALLREQUEST_H
#define INSTALLREQUEST_H

namespace flexd {
    namespace core {

        class InstallRequest : public iCoreAppRequest {
        public:
            explicit InstallRequest(const std::string& name, const std::string& ver, const std::string& path);
            virtual ~InstallRequest() = default;

            const std::string& getBase64() const;
            virtual void accept(Visitor &v);

            InstallRequest(const InstallRequest&) = default;
            InstallRequest& operator=(const InstallRequest&) = default;
        private:
            const std::string m_path;

        };
        typedef InstallRequest* InstallRequest_t;

    } // namespace core
} // namespace flexd


#endif /* INSTALLREQUEST_H */

