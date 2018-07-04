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
 * File:   CoreApRequestValidator.h
 * Author: Peter Kocity
 *
 * Created on February 16, 2018, 2:06 PM
 */

#ifndef COREAPPREQUESTVALIDATOR_H
#define COREAPPREQUESTVALIDATOR_H

#include "iCoreAppRequest.h"
#include "InstallRequest.h"
#include "UninstallRequest.h"
#include "StartRequest.h"
#include "StopRequest.h"
#include "FreezeRequest.h"
#include "UnfreezeRequest.h"
#include "UpdateRequest.h"


namespace flexd {
    namespace core {

        class CoreAppRequestValidator {
        public:
            explicit CoreAppRequestValidator();
            ~CoreAppRequestValidator() = default;

            /*TODO validation*/
            template<class T> bool validRequest(const T& rqt);

        private:
            bool validRequestInstall(const InstallRequest& rqt);
            bool validRequestUninstall(const UninstallRequest& rqt);
            bool validRequestStart(const StartRequest& rqt);
            bool validRequestStop(const StopRequest& rqt);
            bool validRequestFreeze(const FreezeRequest& rqt);
            bool validRequestUnfreeze(const UnfreezeRequest& rqt);
            bool validRequestUpdate(const UpdateRequest& rqt);
        };

        template<>
        inline bool CoreAppRequestValidator::validRequest<InstallRequest>(const InstallRequest& rqt) {
            return validRequestInstall(rqt);
        }

        template<>
        inline bool CoreAppRequestValidator::validRequest<UninstallRequest>(const UninstallRequest& rqt) {
            return validRequestUninstall(rqt);
        }

        template<>
        inline bool CoreAppRequestValidator::validRequest<StartRequest>(const StartRequest& rqt) {
            return validRequestStart(rqt);
        }

        template<>
        inline bool CoreAppRequestValidator::validRequest<StopRequest>(const StopRequest& rqt) {
            return validRequestStop(rqt);
        }

        template<>
        inline bool CoreAppRequestValidator::validRequest<FreezeRequest>(const FreezeRequest& rqt) {
            return validRequestFreeze(rqt);
        }

        template<>
        inline bool CoreAppRequestValidator::validRequest<UnfreezeRequest>(const UnfreezeRequest& rqt) {
            return validRequestUnfreeze(rqt);
        }

        template<>
        inline bool CoreAppRequestValidator::validRequest<UpdateRequest>(const UpdateRequest& rqt) {
            return validRequestUpdate(rqt);
        }

    }
}

#endif /* COREAPPREQUESTVALIDATOR_H */

