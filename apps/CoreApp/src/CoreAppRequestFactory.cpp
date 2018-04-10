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

namespace flexd {
    namespace core {

        iCoreAppRequest_t CoreAppRequestFactory::makeRqst(flexd::icl::JsonObj& json) const {

            /*TODO valid rqst*/

            base::BinStream b;
            std::string path = "./";
            std::string appID, message, operation;
            json.get<std::string>("/AppID", appID);
            json.get<std::string>("/Message", message);
            json.get<std::string>("/Operation", operation);
            if (appID.empty()) {
                appID = "noID";
            }
            if (!operation.empty()) {
                if (operation == "Install") {
                    path = path + appID;
                    b.setBase64(message);
                    b.write(path);
                    std::cout << "INSTALL" << std::endl;
                    return new InstallRequest(appID, appID, path);
                } else if (operation == "Uninstall") {
                    std::cout << "UNINSTALL" << std::endl;
                    return new UninstallRequest(appID, appID);
                } else if (operation == "Start") {
                    std::cout << "START" << std::endl;
                    return new StartRequest(appID, appID);
                } else if (operation == "Stop") {
                    std::cout << "STOP" << std::endl;
                    return new StopRequest(appID, appID);
                } else if (operation == "Freez") {
                    std::cout << "FREEZ" << std::endl;
                    return new FreezRequest(appID, appID);
                } else if (operation == "Unfreez") {
                    std::cout << "UNFREEZ" << std::endl;
                    return new UnfreezRequest(appID, appID);
                } else if (operation == "Update") {
                    path = path + appID;
                    b.setBase64(message);
                    b.write(path);
                    std::cout << "UPDATE" << std::endl;
                    return new UpdateRequest(appID, appID, path);
                } else {
                    std::cout << "INVALID" << std::endl;
                    return new InvalidRequest(appID, appID);
                }
            }
            return NULL;
        }
    }
}

