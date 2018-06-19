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
 * File:   CoreAppRequestFactory.cpp
 * Author: Peter Kocity
 *
 * Created on February 20, 2018, 10:48 AM
 */

#include "CoreAppRequestFactory.h"
#include "FleXdLogger.h"

namespace flexd {
    namespace core {
        
        CoreAppRequestFactory::CoreAppRequestFactory(){
        }

        iCoreAppRequest_t CoreAppRequestFactory::makeRqst(uint8_t& Operation, const std::string& Message, const std::string& AppID) const {
            FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): Creating Request");
            FLEX_LOG_DEBUG("CoreAppRequestFactory::makeRqst(): parsing Json: AppID: ",AppID, " Message: ",Message, " Operation: ",Operation);
            if (Operation == RqstType::Enum::install) {
                FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): return install");
                return new InstallRequest(AppID, AppID, Message);
            } else if (Operation == RqstType::Enum::unintall) {
                FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): return uninstall");
                return new UninstallRequest(AppID, AppID);
            } else if (Operation == RqstType::Enum::start) {
                FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): return start");
                return new StartRequest(AppID, AppID);
            } else if (Operation == RqstType::Enum::stop) {
                FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): return stop");
                return new StopRequest(AppID, AppID);
            } else if (Operation == RqstType::Enum::freez) {
                FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): return freez");
                return new FreezRequest(AppID, AppID);
            } else if (Operation == RqstType::Enum::unfreez) {
                FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): return unfreez");
                return new UnfreezRequest(AppID, AppID);
            } else if (Operation == RqstType::Enum::update) {
                FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): return update");
                return new UpdateRequest(AppID, AppID, Message);
            } else {
                FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): return invalid");
                return new InvalidRequest(AppID, AppID);
            }
        }
    }
}

