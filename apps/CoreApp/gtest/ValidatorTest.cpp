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
 * File:   ValidatorTest.cpp
 * Author: Peter Kocity
 *
 * Created on March 27, 2018, 11:10 AM
 */

#include <cstdlib>
#include <iostream>
#include <gtest/gtest.h>
#include <FleXdEpoll.h>
#include "CoreAppRequestValidator.h"

namespace {

    flexd::icl::ipc::FleXdEpoll pollerDummy(10);

    TEST(Validator, ValidingInstallRequest)
    {
        flexd::core::InstallRequest r(pollerDummy, "Snake", "Snake", "/home/dev/work/SNAKE");
        ASSERT_TRUE(flexd::core::CoreAppRequestValidator::validateRequestInstall(r));
        flexd::core::InstallRequest r1(pollerDummy, "Snake", "", "/home/dev/work/SNAKE");
        ASSERT_FALSE(flexd::core::CoreAppRequestValidator::validateRequestInstall(r1));
        flexd::core::InstallRequest r2(pollerDummy, "", "Snake", "/home/dev/work/SNAKE");
        ASSERT_FALSE(flexd::core::CoreAppRequestValidator::validateRequestInstall(r2));
    }

    TEST(Validator, ValidingUninstallRequest)
    {
        flexd::core::UninstallRequest r(pollerDummy, "Snake", "Snake");
        ASSERT_TRUE(flexd::core::CoreAppRequestValidator::validateRequestUninstall(r));
        flexd::core::UninstallRequest r1(pollerDummy, "Snake", "");
        ASSERT_FALSE(flexd::core::CoreAppRequestValidator::validateRequestUninstall(r1));
        flexd::core::UninstallRequest r2(pollerDummy, "", "Snake");
        ASSERT_FALSE(flexd::core::CoreAppRequestValidator::validateRequestUninstall(r2));
    }

    TEST(Validator, ValidingStartRequest)
    {
        flexd::core::StartRequest r(pollerDummy, "Snake", "Snake");
        ASSERT_TRUE(flexd::core::CoreAppRequestValidator::validateRequestStart(r));
        flexd::core::StartRequest r1(pollerDummy, "Snake", "");
        ASSERT_FALSE(flexd::core::CoreAppRequestValidator::validateRequestStart(r1));
        flexd::core::StartRequest r2(pollerDummy, "", "Snake");
        ASSERT_FALSE(flexd::core::CoreAppRequestValidator::validateRequestStart(r2));
    }

    TEST(Validator, ValidingStopRequest)
    {
        flexd::core::StopRequest r(pollerDummy, "Snake", "Snake");
        ASSERT_TRUE(flexd::core::CoreAppRequestValidator::validateRequestStop(r));
        flexd::core::StopRequest r1(pollerDummy, "Snake", "");
        ASSERT_FALSE(flexd::core::CoreAppRequestValidator::validateRequestStop(r1));
        flexd::core::StopRequest r2(pollerDummy, "", "Snake");
        ASSERT_FALSE(flexd::core::CoreAppRequestValidator::validateRequestStop(r2));
    }

    TEST(Validator, ValidingFreezeRequest)
    {
        flexd::core::FreezeRequest r(pollerDummy, "Snake", "Snake");
        ASSERT_TRUE(flexd::core::CoreAppRequestValidator::validateRequestFreeze(r));
        flexd::core::FreezeRequest r1(pollerDummy, "Snake", "");
        ASSERT_FALSE(flexd::core::CoreAppRequestValidator::validateRequestFreeze(r1));
        flexd::core::FreezeRequest r2(pollerDummy, "", "Snake");
        ASSERT_FALSE(flexd::core::CoreAppRequestValidator::validateRequestFreeze(r2));
    }

    TEST(Validator, ValidingUnfreezeRequest)
    {
        flexd::core::UnfreezeRequest r(pollerDummy, "Snake", "Snake");
        ASSERT_TRUE(flexd::core::CoreAppRequestValidator::validateRequestUnfreeze(r));
        flexd::core::UnfreezeRequest r1(pollerDummy, "Snake", "");
        ASSERT_FALSE(flexd::core::CoreAppRequestValidator::validateRequestUnfreeze(r1));
        flexd::core::UnfreezeRequest r2(pollerDummy, "", "Snake");
        ASSERT_FALSE(flexd::core::CoreAppRequestValidator::validateRequestUnfreeze(r2));
    }

    TEST(Validator, ValidingUpdateRequest)
    {
        flexd::core::UpdateRequest r(pollerDummy, "Snake", "Snake", "/home/dev/work/SNAKE");
        ASSERT_TRUE(flexd::core::CoreAppRequestValidator::validateRequestUpdate(r));
        flexd::core::UpdateRequest r1(pollerDummy, "Snake", "", "/home/dev/work/SNAKE");
        ASSERT_FALSE(flexd::core::CoreAppRequestValidator::validateRequestUpdate(r1));
        flexd::core::UpdateRequest r2(pollerDummy, "", "Snake", "/home/dev/work/SNAKE");
        ASSERT_FALSE(flexd::core::CoreAppRequestValidator::validateRequestUpdate(r2));
    }

}
