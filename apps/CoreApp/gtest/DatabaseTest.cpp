/*
 C *opyright (c) 2018, G*loballogic s.r.o.
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
 * File:   DatabaseTest.cpp
 * Author: Peter Kocity
 *
 * Created on March 27, 2018, 11:10 AM
 */

#include <cstdlib>
#include <iostream>
#include <gtest/gtest.h>
#include "CoreAppDatabase.h"
#include <FleXdIPCCommon.h>

namespace {
    #define TEST_COREAPP_DB         std::string(getenv("HOME")) + std::string("/tmp/CoreApp/TestCoreAppDb.db")
    #define TEST_COREAPP_DB_BAD     std::string(getenv("HOME")) + std::string("/tmp/CoreApp/TestCoreAppDbBAD.db")
    #define TEST_COREAPP_DB_NAME    "TestCoreAppDb"

    std::string TestCoreAppDB(TEST_COREAPP_DB);
    flexd::core::CoreAppDatabase data(TestCoreAppDB, TEST_COREAPP_DB_NAME);

    /*SQLite structure, ID,AppName, AppVer, Install, Uninstall, Start, Stop, Freeze, Unfreeze, UpdateDB, MD5*/
    /*ID- int autoincremental other text not Null*/

    TEST(Database, fileExists) {
        ASSERT_TRUE(data.fileExists(TestCoreAppDB));
        ASSERT_FALSE(data.fileExists(TEST_COREAPP_DB_BAD));
    }

    TEST(Database, add_and_erase_record) {
        std::string appName = "TestApp_1";
        std::string ver = "1.00";
        std::string ret;
        std::string ID = "ID";
        ASSERT_TRUE(data.fileExists(TestCoreAppDB));
        ASSERT_TRUE(data.addRecord(appName, ver));
        ASSERT_TRUE(data.getRecord(appName, ver, ret, ID));
        ASSERT_GE(atoi(ret.c_str()), 2);
        ASSERT_TRUE(data.eraseRecord(appName, ver));
    }

    TEST(Database, add_record_with_edit) {
        std::string appName = "TestApp_2";
        std::string ver = "2.00";
        std::string key = "Install";
        std::string val = "docker install";
        std::string ret;
        ASSERT_TRUE(data.fileExists(TestCoreAppDB));
        ASSERT_TRUE(data.addRecord(appName, ver));
        ASSERT_TRUE(data.editRecord(appName, ver, val, key));
        ASSERT_TRUE(data.getRecord(appName, ver, ret, key));
        ASSERT_EQ(ret, val);
        ASSERT_TRUE(data.eraseRecord(appName, ver));
    }

    TEST(Database, multiple_edit_record) {
        std::string appName = "TestApp_3";
        std::string key = "Install";
        std::string ver = "3.00";
        std::string val1 = "docker install";
        std::string val2 = "install docker";
        std::string ret;
        ASSERT_TRUE(data.fileExists(TestCoreAppDB));
        ASSERT_TRUE(data.addRecord(appName, ver));
        ASSERT_TRUE(data.editRecord(appName, ver, val1, key));
        ASSERT_TRUE(data.getRecord(appName, ver, ret, key));
        ASSERT_EQ(ret, val1);
        ASSERT_TRUE(data.editRecord(appName, ver, val2, key));
        ASSERT_TRUE(data.getRecord(appName, ver, ret, key));
        ASSERT_EQ(ret, val2);
        ASSERT_TRUE(data.eraseRecord(appName, ver));
    }

}
