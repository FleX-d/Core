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
 * File:   CoreAppDatabase.h
 * Author: Peter Kocity
 *
 * Created on Februar 7, 2018, 9:32 AM
 */

#ifndef COREAPPDATABASE_H
#define COREAPPDATABASE_H

#include <string>
#include <iostream>
#include <memory>
#include "sqlite3pp.h"
#include <fstream>

namespace flexd {
    namespace core {

        class CoreAppDatabase {
        public:
            explicit CoreAppDatabase(const std::string& dbPath, const std::string& dbName);
            virtual ~CoreAppDatabase() = default;

            bool getRecord(const std::string& appName, const std::string& appVer, std::string& dest, const std::string& key);
            bool editRecord(const std::string& appName, const std::string& appVer, std::string& dest, const std::string& key);
            bool getTimeout(const std::string& appName, const std::string& appVer, time_t& timeout);
            bool editTimeout(const std::string& appName, const std::string& appVer, time_t timouet);
            bool addRecord(const std::string& appName, const std::string& appVer);
            bool eraseRecord(const std::string& appName, const std::string& appVer);
            bool fileExists(const std::string& filename);

            CoreAppDatabase(const CoreAppDatabase&) = delete;
            CoreAppDatabase& operator=(const CoreAppDatabase&) = delete;
        private:
            sqlite3pp::database db;
            const std::string m_path;
            const std::string m_name;
        };
    }//namespace core
}//namespace flexd


#endif /* COREAPPDATABASE_H */


