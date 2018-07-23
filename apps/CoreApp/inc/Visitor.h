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
 * File:   Visitor.h
 * Author: Peter Kocity
 *
 * Created on February 22, 2018, 1:47 PM
 */


#ifndef VISITOR_H
#define VISITOR_H

#include "InstallRequest.h"
#include "UninstallRequest.h"
#include "StartRequest.h"
#include "StopRequest.h"
#include "FreezeRequest.h"
#include "UnfreezeRequest.h"
#include "UpdateRequest.h"
#include "InvalidRequest.h"

namespace flexd{
    namespace core{
        class Visitor{
        public:
            explicit Visitor();
            ~Visitor() = default;
            virtual bool visit(InstallRequest_t rqst);
            virtual bool validate(InstallRequest_t rqst);
            virtual bool visit(UninstallRequest_t rqst);
            virtual bool validate(UninstallRequest_t rqst);
            virtual bool visit(StartRequest_t rqst);
            virtual bool validate(StartRequest_t rqst);
            virtual bool visit(StopRequest_t rqst);
            virtual bool validate(StopRequest_t rqst);
            virtual bool visit(FreezeRequest_t rqst);
            virtual bool validate(FreezeRequest_t rqst);
            virtual bool visit(UnfreezeRequest_t rqst);
            virtual bool validate(UnfreezeRequest_t rqst);
            virtual bool visit(UpdateRequest_t rqst);
            virtual bool validate(UpdateRequest_t rqst);
            virtual bool visit(InvalidRequest_t rqst);
            virtual bool validate(InvalidRequest_t rqst);

            Visitor(Visitor&) = default;
            Visitor& operator=(Visitor&) = default;
        };
    }
}

#endif /* VISITOR_H */

