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

#include "CoreAppDatabase.h"
#include <FleXdLogger.h>

namespace flexd {
    namespace core {

        CoreAppDatabase::CoreAppDatabase(const std::string& dbPath, const std::string& dbName)
        : m_path(dbPath),
          m_name(dbName) {
            FLEX_LOG_TRACE("Path for database: ", m_path, " Name for database: ", m_name);
        }

        bool CoreAppDatabase::getRecord(const std::string& appName, const std::string& appVer, std::string& dest, const std::string& key) {
            constexpr auto sqliteFlags = SQLITE_OPEN_READONLY;
            const std::string queryString = "SELECT " + key + " FROM " + m_name + " WHERE AppName=\"" + appName + "\" AND AppVer=\"" + appVer + "\";";
            FLEX_LOG_TRACE("CoreAppDatabase::getRecord(): ", queryString);
            try {
                db.connect(m_path.c_str(), sqliteFlags);
                sqlite3pp::query query(db, queryString.c_str());
                const auto rowCount = std::distance(query.begin(), query.end());
                if (rowCount != 1) {
                    FLEX_LOG_ERROR("CoreAppDatabase::getRecord(): expected 1 entry for key='", key, "' from table='", m_name, "', got ", rowCount, " entries\n");
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

        bool CoreAppDatabase::editRecord(const std::string& appName, const std::string& appVer, std::string& val, const std::string& key) {
            constexpr auto sqliteFlags = SQLITE_OPEN_READWRITE;
            const std::string queryString = "UPDATE " + m_name + " SET " + key + "=\"" + val + "\" WHERE AppName=\"" + appName + "\" AND AppVer=\"" + appVer + "\";";
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
            if (getRecord(appName, appVer, tmp, key)) {
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

        bool CoreAppDatabase::getTimeout(const std::string& appName, const std::string& appVer, time_t& timeout) {
            constexpr auto sqliteFlags = SQLITE_OPEN_READONLY;
            const std::string queryString = "SELECT Timeout FROM " + m_name + " WHERE AppName=\"" + appName + "\" AND AppVer=\"" + appVer + "\";";
            FLEX_LOG_TRACE("CoreAppDatabase::getTimeout(): ", queryString);
            try {
                db.connect(m_path.c_str(), sqliteFlags);
                sqlite3pp::query query(db, queryString.c_str());
                const auto rowCount = std::distance(query.begin(), query.end());
                if (rowCount != 1) {
                    FLEX_LOG_ERROR("CoreAppDatabase::getTimeout(): expected 1 entry for key='Timeout' from table='", m_name, "', got ", rowCount, " entries\n");
                    return false;
                }
                auto it = query.begin();
                timeout = (*it).get<long long int>(0);
            } catch (const sqlite3pp::database_error& e) {
                FLEX_LOG_ERROR("CoreAppDatabase::getTimeout(): database error: ", e.what());
                db.disconnect();
                return false;
            }
            FLEX_LOG_TRACE("CoreAppDatabase::getTimeout(): Success return record: ", timeout);
            db.disconnect();
            return true;
        }

        bool CoreAppDatabase::editTimeout(const std::string& appName, const std::string& appVer, time_t timeout) {
            constexpr auto sqliteFlags = SQLITE_OPEN_READWRITE;
            const std::string queryString = "UPDATE " + m_name + " SET Timeout=\"" + std::to_string(timeout) + "\" WHERE AppName=\"" + appName + "\" AND AppVer=\"" + appVer + "\";";
            FLEX_LOG_TRACE("CoreAppDatabase::editTimeout(): ", queryString);
            try {
                db.connect(m_path.c_str(), sqliteFlags);
                db.execute(queryString.c_str());
            } catch (const sqlite3pp::database_error& e) {
                FLEX_LOG_ERROR("CoreAppDatabase::editTimeout(): database error: ", e.what());
                db.disconnect();
                return false;
            }
            time_t tmp;
            if (getTimeout(appName, appVer, tmp)) {
                db.disconnect();
                if (timeout == tmp) {
                    FLEX_LOG_TRACE("CoreAppDatabase::editTimeout(): Success Edit");
                    return true;
                }
                FLEX_LOG_ERROR("CoreAppDatabase::editTimeout(): Incorect Write");
            } else {
                FLEX_LOG_ERROR("CoreAppDatabase::editTimeout(): Cant read now write note");
            }
            db.disconnect();
            return false;
        }

        bool CoreAppDatabase::addRecord(const std::string& appName, const std::string& appVer) {
            constexpr auto sqliteFlags = SQLITE_OPEN_READWRITE;
            const std::string queryString = "INSERT INTO "
                    + m_name + "(AppName, AppVer, Install, Uninstall, Start, Stop, Freeze, Unfreeze, UpdateDB, Timeout, MD5) VALUES"
                    " (\"" + appName + "\", \"" + appVer + "\",\" \",\" \",\" \",\" \",\" \",\" \",\" \",0,\" \");";
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

        bool CoreAppDatabase::eraseRecord(const std::string& appName, const std::string& appVer) {
            constexpr auto sqliteFlags = SQLITE_OPEN_READWRITE;
            const std::string queryString = "DELETE FROM " + m_name + " WHERE AppName=\"" + appName + "\" AND AppVer=\"" + appVer + "\";";
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
