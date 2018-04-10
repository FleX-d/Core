#include <cstdlib>
#include <iostream>
#include <gtest/gtest.h>
#include "CoreAppRequestFactory.h"


flexd::core::CoreAppRequestFactory rf;

TEST(Factory, Create_Install)
{
    flexd::icl::JsonObj o("{\"Operation\": \"Install\", \"Message\": \"AHOJ\", \"AppID\": \"SNAKE\"}");
    flexd::core::iCoreAppRequest_t r=rf.makeRqst(o);
    flexd::core::InstallRequest ir("SNAKE", "SNAKE", "./testfile");
    ASSERT_EQ(ir.getType(),r->getType());
    ASSERT_EQ(ir.getName(),r->getName());
    ASSERT_EQ(ir.getVersion(),r->getVersion());
    std::cout<<ir.getBase64()<<std::endl;
}

TEST(Factory, Create_Uninstall)
{
    flexd::icl::JsonObj o("{\"Operation\": \"Uninstall\", \"Message\": \"AHOJ\", \"AppID\": \"SNAKE\"}");
    flexd::core::iCoreAppRequest_t r=rf.makeRqst(o);
    flexd::core::UninstallRequest ur("SNAKE", "SNAKE");
    ASSERT_EQ(ur.getType(), r->getType());
    ASSERT_EQ(ur.getName(), r->getName());
    ASSERT_EQ(ur.getVersion(), r->getVersion());
}

TEST(Factory, Create_Start)
{
    flexd::icl::JsonObj o("{\"Operation\": \"Start\", \"Message\": \"AHOJ\", \"AppID\": \"SNAKE\"}");
    flexd::core::iCoreAppRequest_t r=rf.makeRqst(o);
    flexd::core::StartRequest sr("SNAKE", "SNAKE");
    ASSERT_EQ(sr.getType(), r->getType());
    ASSERT_EQ(sr.getName(), r->getName());
    ASSERT_EQ(sr.getVersion(), r->getVersion());
}

TEST(Factory, Create_Stop)
{
    flexd::icl::JsonObj o("{\"Operation\": \"Stop\", \"Message\": \"AHOJ\", \"AppID\": \"SNAKE\"}");
    flexd::core::iCoreAppRequest_t r=rf.makeRqst(o);
    flexd::core::StopRequest tr("SNAKE", "SNAKE");
    ASSERT_EQ(tr.getType(), r->getType());
    ASSERT_EQ(tr.getName(), r->getName());
    ASSERT_EQ(tr.getVersion(), r->getVersion());
}

TEST(Factory, Create_Freez)
{
    flexd::icl::JsonObj o("{\"Operation\": \"Freez\", \"Message\": \"AHOJ\", \"AppID\": \"SNAKE\"}");
    flexd::core::iCoreAppRequest_t r=rf.makeRqst(o);
    flexd::core::FreezRequest fr("SNAKE", "SNAKE");
    ASSERT_EQ(fr.getType(), r->getType());
    ASSERT_EQ(fr.getName(), r->getName());
    ASSERT_EQ(fr.getVersion(), r->getVersion());
}

TEST(Factory, Create_Unfreez)
{
    flexd::icl::JsonObj o("{\"Operation\": \"Unfreez\", \"Message\": \"AHOJ\", \"AppID\": \"SNAKE\"}");
    flexd::core::iCoreAppRequest_t r=rf.makeRqst(o);
    flexd::core::UnfreezRequest ufr("SNAKE", "SNAKE");
    ASSERT_EQ(ufr.getType(), r->getType());
    ASSERT_EQ(ufr.getName(), r->getName());
    ASSERT_EQ(ufr.getVersion(), r->getVersion());
}

TEST(Factory, Create_Update)
{
    flexd::icl::JsonObj o("{\"Operation\": \"Update\", \"Message\": \"AHOJ\", \"AppID\": \"SNAKE\"}");
    flexd::core::iCoreAppRequest_t r=rf.makeRqst(o);
    flexd::core::UpdateRequest upr("SNAKE", "SNAKE", "./testfile");
    ASSERT_EQ(upr.getType(), r->getType());
    ASSERT_EQ(upr.getName(), r->getName());
    ASSERT_EQ(upr.getVersion(),r->getVersion());
    std::cout<<upr.getBase64()<<std::endl;
}

TEST(Factory, Create_Invalid)
{
    flexd::icl::JsonObj o("{\"Operation\": \"S\", \"Message\": \"AHOJ\", \"AppID\": \"SNAKE\"}");
    flexd::core::iCoreAppRequest_t r=rf.makeRqst(o);
    flexd::core::InvalidRequest inr("SNAKE", "SNAKE");
    ASSERT_EQ(inr.getType(), r->getType());
    ASSERT_EQ(inr.getName(), r->getName());
    ASSERT_EQ(inr.getVersion(), r->getVersion());
}
