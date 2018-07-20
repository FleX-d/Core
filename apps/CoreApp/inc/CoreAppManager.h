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
 * File:   CoreAppManager.h
 * Author: Peter Kocity
 *
 * Created on February 8, 2018, 10:48 AM
 */

#ifndef COREAPPMANAGER_H
#define COREAPPMANAGER_H

#include "iApp.h"
#include "Visitor.h"
#include "CoreAppExecutor.h"
#include "CoreAppDatabase.h"
#include "CoreAppRequestQueue.h"
#include <FleXdEpoll.h>
#include <map>
#include <thread>
#include <atomic>

#define COREAPP_POLLER_MAXEVENTH 10

namespace flexd {
    namespace core {

        class InstallRequest;
        class UninstallRequest;
        class StartRequest;
        class StopRequest;
        class FreezeRequest;
        class UnfreezeRequest;
        class UpdateRequest;
        class InvalidRequest;

        class CoreAppManager : public Visitor {
        public:
            CoreAppManager(const std::string& dbPath, const std::string& dbName);
            virtual ~CoreAppManager();

            void startRqstPollerThread();
            void stopRqstPollerThread();
            bool isWorkerThreadRunning() const;
            bool onRequest(pSharediCoreAppRequest_t rqst);

            const flexd::icl::ipc::FleXdEpoll& getRqstPoller() const;
            flexd::icl::ipc::FleXdEpoll& getRqstPoller();

            virtual void visit(InstallRequest_t rqst) override;
            virtual bool validate(InstallRequest_t rqst) override;
            virtual void visit(UninstallRequest_t rqst) override;
            virtual bool validate(UninstallRequest_t rqst) override;
            virtual void visit(StartRequest_t rqst) override;
            virtual bool validate(StartRequest_t rqst) override;
            virtual void visit(StopRequest_t rqst) override;
            virtual bool validate(StopRequest_t rqst) override;
            virtual void visit(FreezeRequest_t rqst) override;
            virtual bool validate(FreezeRequest_t rqst) override;
            virtual void visit(UnfreezeRequest_t rqst) override;
            virtual bool validate(UnfreezeRequest_t rqst) override;
            virtual void visit(UpdateRequest_t rqst) override;
            virtual bool validate(UpdateRequest_t rqst) override;

            CoreAppManager(const CoreAppManager& orig) = delete;
            CoreAppManager& operator=(const CoreAppManager& orig) = delete;

        private:
            bool prepareRequest(iCoreAppRequest& rqst);
            void performRequests();
            void onRequestDone(bool result);

            bool appExecute(const std::string& cmd);
            bool appExecute(const std::string& cmd, std::string& message, const std::string& path = "./");
            std::string getDbKey(RqstType::Enum e);

            bool addToList(std::string appName);
            bool eraseFromList(std::string appName);
            bool findInList(std::string appName);
            bool changeStateInList(std::string appName, RqstType::Enum e);

        private:
            flexd::icl::ipc::FleXdEpoll m_rqstPoller;
            CoreAppExecutor m_exe;
            CoreAppDatabase m_db;
            CoreAppRequestQueue m_requests;
            std::map<std::string, iApp> m_apps;
            std::thread m_rqstPollerThread;
            std::thread m_workerThread;
            std::atomic_bool m_safeStop;
        };

    } // namespace core
} // namespace flexd


#endif /* COREAPPMANAGER_H */
