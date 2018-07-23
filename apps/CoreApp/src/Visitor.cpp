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
 * File:   Visitor.cpp
 * Author: Peter Kocity
 *
 * Created on February 22, 2018, 1:47 PM
 */

#include "Visitor.h"
#include <FleXdLogger.h>


namespace flexd {
    namespace core {

        Visitor::Visitor()
        {
            FLEX_LOG_TRACE("Visitor: ");
        }
        /*TODO*/
        bool Visitor::visit(InstallRequest_t rqst) {
            FLEX_LOG_TRACE("Visitor::visit(): installrequest ");
            return false;
        }

        bool Visitor::validate(InstallRequest_t rqst) {
            FLEX_LOG_TRACE("Visitor::validate(): installrequest ");
            return false;
        }

        bool Visitor::visit(UninstallRequest_t rqst) {
            FLEX_LOG_TRACE("Visitor::visit(): uninstallrequest ");
            return false;
        }

        bool Visitor::validate(UninstallRequest_t rqst) {
            FLEX_LOG_TRACE("Visitor::validate(): uninstallrequest ");
            return false;
        }

        bool Visitor::visit(StartRequest_t rqst) {
            FLEX_LOG_TRACE("Visitor::visit(): startrequest ");
            return false;
        }

        bool Visitor::validate(StartRequest_t rqst) {
            FLEX_LOG_TRACE("Visitor::validate(): startrequest ");
            return false;
        }

        bool Visitor::visit(StopRequest_t rqst) {
            FLEX_LOG_TRACE("Visitor::visit(): stoprequest ");
            return false;
        }

        bool Visitor::validate(StopRequest_t rqst) {
            FLEX_LOG_TRACE("Visitor::validate(): stoprequest ");
            return false;
        }

        bool Visitor::visit(FreezeRequest_t rqst) {
            FLEX_LOG_TRACE("Visitor::visit(): freezerequest ");
            return false;
        }

        bool Visitor::validate(FreezeRequest_t rqst) {
            FLEX_LOG_TRACE("Visitor::validate(): freezerequest ");
            return false;
        }

        bool Visitor::visit(UnfreezeRequest_t rqst) {
            FLEX_LOG_TRACE("Visitor::visit(): unfreezerequest ");
            return false;
        }

        bool Visitor::validate(UnfreezeRequest_t rqst) {
            FLEX_LOG_TRACE("Visitor::validate(): unfreezerequest ");
            return false;
        }

        bool Visitor::visit(UpdateRequest_t rqst) {
            FLEX_LOG_TRACE("Visitor::visit(): updaterequest ");
            return false;
        }

        bool Visitor::validate(UpdateRequest_t rqst) {
            FLEX_LOG_TRACE("Visitor::validate(): updaterequest ");
            return false;
        }

        bool Visitor::visit(InvalidRequest_t rqst) {
            FLEX_LOG_TRACE("Visitor::visit(): invalidrequest ");
            return false;
        }

        bool Visitor::validate(InvalidRequest_t rqst) {
            FLEX_LOG_TRACE("Visitor::validate(): invalidrequest ");
            return false;
        }

    }
}


