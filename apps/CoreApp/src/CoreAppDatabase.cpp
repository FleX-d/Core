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
 * File:   CoreAppDatabase.cpp
 * Author: Peter Kocity
 *
 * Created on Februar 7, 2018, 9:32 AM
 */
#include "FleXdLogger.h"
#include "CoreAppDatabase.h"

namespace flexd {
    namespace core {

        CoreAppDatabase::CoreAppDatabase(const std::string& dbPath, const std::string& dbName) :
        m_path(dbPath) {
            FLEX_LOG_TRACE("Path for database: ", m_path);
        }

        bool CoreAppDatabase::getRecord(const std::string& dbName, const std::string& name, const std::string& ver, std::string& dest, const std::string& key) {
            constexpr auto sqliteFlags = SQLITE_OPEN_READONLY;
            const std::string queryString = "SELECT " + key + " FROM " + dbName + " WHERE AppName=\"" + name + "\" AND AppVer=\"" + ver + "\";";
            FLEX_LOG_TRACE("CoreAppDatabase::getRecord(): ", queryString);
            try {
                db.connect(m_path.c_str(), sqliteFlags);
                sqlite3pp::query query(db, queryString.c_str());
                const auto rowCount = std::distance(query.begin(), query.end());
                if (rowCount != 1) {
                    FLEX_LOG_ERROR("CoreAppDatabase::getRecord(): expected 1 entry for key='", key, "' from table='", dbName, "', got ", rowCount, " entries\n");
                    return false;
                }
                auto it = query.begin();
                dest = (*it).get<std::string>(0);
            } catch (const sqlite3pp::database_error& e) {
                FLEX_LOG_ERROR("CoreAppDatabase::getRecord(): database error: ", e.what());
                db.disconnect();
                return false;
            }
            FLEX_LOG_TRACE("CoreAppDatabase::getRecord(): Success return record: ", dest);
            db.disconnect();
            return true;
        }

        bool CoreAppDatabase::editRecord(const std::string& dbName, const std::string& name, const std::string& ver, std::string& val, const std::string& key) {
            constexpr auto sqliteFlags = SQLITE_OPEN_READWRITE;
            const std::string queryString = "UPDATE " + dbName + " SET " + key + "=\"" + val + "\" WHERE AppName=\"" + name + "\" AND AppVer=\"" + ver + "\";";
            FLEX_LOG_TRACE("CoreAppDatabase::editRecord(): ", queryString);
            try {
                db.connect(m_path.c_str(), sqliteFlags);
                db.execute(queryString.c_str());
            } catch (const sqlite3pp::database_error& e) {
                FLEX_LOG_ERROR("CoreAppDatabase::editRecord(): database error: ", e.what());
                db.disconnect();
                return false;
            }
            std::string tmp;
            if (getRecord(dbName, name, ver, tmp, key)) {
                db.disconnect();
                if (val == tmp) {
                    FLEX_LOG_TRACE("CoreAppDatabase::editRecord(): Success Edit");
                    return true;
                }
                FLEX_LOG_ERROR("CoreAppDatabase::editRecord(): Incorect Write");
            } else {
                FLEX_LOG_ERROR("CoreAppDatabase::editRecord(): Cant read now write note");
            }
            db.disconnect();
            return false;
        }

        bool CoreAppDatabase::addRecord(const std::string& dbName, const std::string& name, const std::string& ver) {
            constexpr auto sqliteFlags = SQLITE_OPEN_READWRITE;
            const std::string queryString = "INSERT INTO "
                    + dbName + "(AppName, AppVer, Install, Uninstall, Start, Stop, Freeze, Unfreeze, UpdateDB, MD5) VALUES"
                    " (\"" + name + "\", \"" + ver + "\",\" \",\" \",\" \",\" \",\" \",\" \",\" \",\" \");";
            FLEX_LOG_TRACE("CoreAppDatabase::addRecord(): ", queryString);
            try {
                db.connect(m_path.c_str(), sqliteFlags);
                db.execute(queryString.c_str());
            } catch (const sqlite3pp::database_error& e) {
                FLEX_LOG_ERROR("CoreAppDatabase::addRecord(): database error: ", e.what());
                db.disconnect();
                return false;
            }
            FLEX_LOG_TRACE("CoreAppDatabase::addRecord(): Success Add");
            db.disconnect();
            return true;
        }

        bool CoreAppDatabase::eraseRecord(const std::string& dbName, const std::string& name, const std::string& ver) {
            constexpr auto sqliteFlags = SQLITE_OPEN_READWRITE;
            const std::string queryString = "DELETE FROM " + dbName + " WHERE AppName=\"" + name + "\" AND AppVer=\"" + ver + "\";";
            FLEX_LOG_TRACE("CoreAppDatabase::eraseRecord(): ", queryString);
            try {
                db.connect(m_path.c_str(), sqliteFlags);
                db.execute(queryString.c_str());
            } catch (const sqlite3pp::database_error& e) {
                FLEX_LOG_ERROR("CoreAppDatabase::eraseRecord(): database error: ", e.what());
                db.disconnect();
                return false;
            }
            FLEX_LOG_TRACE("CoreAppDatabase::eraseRecord(): Success erase");
            return true;
        }

        bool CoreAppDatabase::fileExists(const std::string& filename) {
            std::ifstream file(filename);
            FLEX_LOG_TRACE("CoreAppDatabase::fileExists(): ", file.good());
            return file.good();
        }
    }
}
