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
 * File:   CoreAppRequestQueue.cpp
 * Author: Martin Strenger
 *
 * Created on July 9, 2018
 */

#include "CoreAppRequestQueue.h"
#include <FleXdLogger.h>


namespace flexd {
    namespace core {

        CoreAppRequestQueue::CoreAppRequestQueue()
        : m_requests(),
          m_mutex(),
          m_condition(),
          m_safeStop(false) {
        }

        CoreAppRequestQueue::~CoreAppRequestQueue() {
        }

        bool CoreAppRequestQueue::push(pSharediCoreAppRequest_t&& rqst) {
            if (rqst) {
                FLEX_LOG_TRACE("CoreAppRequestQueue::push(): ", rqst->getName(), rqst->getVersion(), rqst->getType());
                std::lock_guard<std::mutex> lock(m_mutex);
                m_requests.push(std::move(rqst));
                m_condition.notify_one();
                return true;
            } else {
                FLEX_LOG_DEBUG("CoreAppRequestQueue::push(): invalid request");
            }
            return false;
        }

        pSharediCoreAppRequest_t CoreAppRequestQueue::front() {
            FLEX_LOG_TRACE("CoreAppRequestQueue::front()");
            std::unique_lock<std::mutex> lock(m_mutex);
            while(!m_safeStop.load() && m_requests.empty())
            {
                FLEX_LOG_TRACE("CoreAppRequestQueue::front(): wait for request");
                // release lock as long as the wait and reaquire it afterwards
                m_condition.wait(lock);
            }
            if (!m_safeStop.load()) {
                pSharediCoreAppRequest_t rqst(m_requests.front());
                if (rqst) {
                    FLEX_LOG_TRACE("CoreAppRequestQueue::front(): ", rqst->getName(), rqst->getVersion(), rqst->getType());
                } else {
                    FLEX_LOG_ERROR("CoreAppRequestQueue::front(): nullptr stored in the queue");
                }
                return rqst;
            }
            return nullptr;
        }

        void CoreAppRequestQueue::pop() {
            FLEX_LOG_TRACE("CoreAppRequestQueue::pop()");
            std::lock_guard<std::mutex> lock(m_mutex);
            if (!m_safeStop.load() && !m_requests.empty()) {
                m_requests.pop();
            }
        }

        void CoreAppRequestQueue::stopActions() {
            FLEX_LOG_TRACE("CoreAppRequestQueue::stopActions()");
            m_safeStop.store(true);
            m_condition.notify_one();
        }

    }
}
