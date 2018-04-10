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
#include "FreezRequest.h"
#include "UnfreezRequest.h"
#include "UpdateRequest.h"
#include "InvalidRequest.h"

namespace flexd{
    namespace core{
        class Visitor{
        public:
            explicit Visitor();
            ~Visitor() = default;
            void visit(InstallRequest_t r);
            void visit(UninstallRequest_t r);
            void visit(StartRequest_t r);
            void visit(StopRequest_t r);
            void visit(FreezRequest_t r);
            void visit(UnfreezRequest_t r);
            void visit(UpdateRequest_t r);
            void visit(InvalidRequest_t r);
            
            Visitor(Visitor&) = default;
            Visitor& operator=(Visitor&) = default;
        private:
                
        };   
    }
}

#endif /* VISITOR_H */

