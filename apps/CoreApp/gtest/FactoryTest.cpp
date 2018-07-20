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
 * File:   FactoryTest.cpp
 * Author: Peter Kocity
 *
 * Created on March 27, 2018, 11:10 AM
 */

#include <cstdlib>
#include <iostream>
#include <gtest/gtest.h>
#include <FleXdEpoll.h>
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

namespace {

    std::string msg, id;
    uint8_t op;
    flexd::icl::ipc::FleXdEpoll pollerDummy(10);

    TEST(Factory, Create_Install)
    {
        op=0;
        msg="HELLO";
        id="SNAKE";
        flexd::core::pSharediCoreAppRequest_t r(flexd::core::CoreAppRequestFactory::makeRqst(pollerDummy, op, msg, id));
        flexd::core::InstallRequest ir(pollerDummy, "SNAKE", "SNAKE", "./testfile");
        ASSERT_EQ(ir.getType(),r->getType());
        ASSERT_EQ(ir.getName(),r->getName());
        ASSERT_EQ(ir.getVersion(),r->getVersion());
    }

    TEST(Factory, Create_Uninstall)
    {
        op=1;
        msg="HELLO";
        id="SNAKE";
        flexd::core::pSharediCoreAppRequest_t r(flexd::core::CoreAppRequestFactory::makeRqst(pollerDummy, op, msg, id));
        flexd::core::UninstallRequest ur(pollerDummy, "SNAKE", "SNAKE");
        ASSERT_EQ(ur.getType(), r->getType());
        ASSERT_EQ(ur.getName(), r->getName());
        ASSERT_EQ(ur.getVersion(), r->getVersion());
    }

    TEST(Factory, Create_Start)
    {
        op=2;
        msg="HELLO";
        id="SNAKE";
        flexd::core::pSharediCoreAppRequest_t r(flexd::core::CoreAppRequestFactory::makeRqst(pollerDummy, op, msg, id));
        flexd::core::StartRequest sr(pollerDummy, "SNAKE", "SNAKE");
        ASSERT_EQ(sr.getType(), r->getType());
        ASSERT_EQ(sr.getName(), r->getName());
        ASSERT_EQ(sr.getVersion(), r->getVersion());
    }

    TEST(Factory, Create_Stop)
    {
        op=3;
        msg="HELLO";
        id="SNAKE";
        flexd::core::pSharediCoreAppRequest_t r(flexd::core::CoreAppRequestFactory::makeRqst(pollerDummy, op, msg, id));
        flexd::core::StopRequest tr(pollerDummy, "SNAKE", "SNAKE");
        ASSERT_EQ(tr.getType(), r->getType());
        ASSERT_EQ(tr.getName(), r->getName());
        ASSERT_EQ(tr.getVersion(), r->getVersion());
    }

    TEST(Factory, Create_Freeze)
    {
        op=4;
        msg="HELLO";
        id="SNAKE";
        flexd::core::pSharediCoreAppRequest_t r(flexd::core::CoreAppRequestFactory::makeRqst(pollerDummy, op, msg, id));
        flexd::core::FreezeRequest fr(pollerDummy, "SNAKE", "SNAKE");
        ASSERT_EQ(fr.getType(), r->getType());
        ASSERT_EQ(fr.getName(), r->getName());
        ASSERT_EQ(fr.getVersion(), r->getVersion());
    }

    TEST(Factory, Create_Unfreeze)
    {
        op=5;
        msg="HELLO";
        id="SNAKE";
        flexd::core::pSharediCoreAppRequest_t r(flexd::core::CoreAppRequestFactory::makeRqst(pollerDummy, op, msg, id));
        flexd::core::UnfreezeRequest ufr(pollerDummy, "SNAKE", "SNAKE");
        ASSERT_EQ(ufr.getType(), r->getType());
        ASSERT_EQ(ufr.getName(), r->getName());
        ASSERT_EQ(ufr.getVersion(), r->getVersion());
    }

    TEST(Factory, Create_Update)
    {
        op=6;
        msg="HELLO";
        id="SNAKE";
        flexd::core::pSharediCoreAppRequest_t r(flexd::core::CoreAppRequestFactory::makeRqst(pollerDummy, op, msg, id));
        flexd::core::UpdateRequest upr(pollerDummy, "SNAKE", "SNAKE", "./testfile");
        ASSERT_EQ(upr.getType(), r->getType());
        ASSERT_EQ(upr.getName(), r->getName());
        ASSERT_EQ(upr.getVersion(),r->getVersion());
    }

    TEST(Factory, Create_Invalid)
    {
        op=9;
        msg="HELLO";
        id="SNAKE";
        flexd::core::pSharediCoreAppRequest_t r(flexd::core::CoreAppRequestFactory::makeRqst(pollerDummy, op, msg, id));
        flexd::core::InvalidRequest inr(pollerDummy, "SNAKE", "SNAKE");
        ASSERT_EQ(inr.getType(), r->getType());
        ASSERT_EQ(inr.getName(), r->getName());
        ASSERT_EQ(inr.getVersion(), r->getVersion());
    }

}
