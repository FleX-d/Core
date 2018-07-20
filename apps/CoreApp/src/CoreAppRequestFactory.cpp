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
#include "CoreAppTypes.h"
#include "InstallRequest.h"
#include "UpdateRequest.h"
#include "UnfreezeRequest.h"
#include "FreezeRequest.h"
#include "StopRequest.h"
#include "StartRequest.h"
#include "UninstallRequest.h"
#include "InvalidRequest.h"
#include <FleXdLogger.h>

namespace flexd {
    namespace core {
        namespace CoreAppRequestFactory {

            pSharediCoreAppRequest_t makeRqst(flexd::icl::ipc::FleXdEpoll& rqstPoller, uint8_t& Operation, const std::string& Message, const std::string& AppID, time_t timeout /*= 0L*/) {
                FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): Creating Request");
                FLEX_LOG_DEBUG("CoreAppRequestFactory::makeRqst(): parsing Json: AppID: ",AppID, " Message: ",Message, " Operation: ",Operation);
                //TODO versions
                switch (Operation) {
                    case RqstType::Enum::install:
                        FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): return install");
                        return std::make_shared<InstallRequest>(rqstPoller, AppID, AppID, Message, timeout);
                    case RqstType::Enum::uninstall:
                        FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): return uninstall");
                        return std::make_shared<UninstallRequest>(rqstPoller, AppID, AppID, timeout);
                    case RqstType::Enum::start:
                        FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): return start");
                        return std::make_shared<StartRequest>(rqstPoller, AppID, AppID, timeout);
                    case RqstType::Enum::stop:
                        FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): return stop");
                        return std::make_shared<StopRequest>(rqstPoller, AppID, AppID, timeout);
                    case RqstType::Enum::freeze:
                        FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): return freeze");
                        return std::make_shared<FreezeRequest>(rqstPoller, AppID, AppID, timeout);
                    case RqstType::Enum::unfreeze:
                        FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): return unfreeze");
                        return std::make_shared<UnfreezeRequest>(rqstPoller, AppID, AppID, timeout);
                    case RqstType::Enum::update:
                        FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): return update");
                        return std::make_shared<UpdateRequest>(rqstPoller, AppID, AppID, Message, timeout);
                    default:
                        FLEX_LOG_TRACE("CoreAppRequestFactory::makeRqst(): return invalid");
                        return std::make_shared<InvalidRequest>(rqstPoller, AppID, AppID);
                }
            }

        }
    }
}

