#include <cstdlib>
#include <iostream>
#include <gtest/gtest.h>
#include "CoreAppRequestValidator.h"
#include "StartRequest.h"
#include "InvalidRequest.h"

flexd::core::CoreAppRequestValidator valid;

TEST(Validator, ValidingInstallRequest)
{
    flexd::core::InstallRequest r("Snake", "Snake", "/home/dev/work/SNAKE");
    ASSERT_TRUE(valid.validRequest(r));
    flexd::core::InstallRequest r1("Snake", "", "/home/dev/work/SNAKE");
    ASSERT_FALSE(valid.validRequest(r1));
    flexd::core::InstallRequest r2("", "Snake", "/home/dev/work/SNAKE");
    ASSERT_FALSE(valid.validRequest(r2));
}

TEST(Validator, ValidingUninstallRequest)
{
    flexd::core::UninstallRequest r("Snake", "Snake");
    ASSERT_TRUE(valid.validRequest(r));
    flexd::core::UninstallRequest r1("Snake", "");
    ASSERT_FALSE(valid.validRequest(r1));
}

TEST(Validator, ValidingStartRequest)
{
    flexd::core::StartRequest r("Snake", "Snake");
    ASSERT_TRUE(valid.validRequest(r));
    flexd::core::StartRequest r1("Snake", "S");
    ASSERT_TRUE(valid.validRequest(r1));
}

TEST(Validator, ValidingStopRequest)
{
    flexd::core::StopRequest r("Snake", "Snake");
    ASSERT_TRUE(valid.validRequest(r));
    flexd::core::StopRequest r1("", "Snake");
    ASSERT_FALSE(valid.validRequest(r1));
}

TEST(Validator, ValidingFreezRequest)
{
    flexd::core::FreezRequest r("Snake", "Snake");
    ASSERT_TRUE(valid.validRequest(r));
    flexd::core::FreezRequest r1("S", "Snake");
    ASSERT_TRUE(valid.validRequest(r1));
}

TEST(Validator, ValidingUnfreezRequest)
{
    flexd::core::UnfreezRequest r("Snake", "Snake");
    ASSERT_TRUE(valid.validRequest(r));
    flexd::core::UnfreezRequest r1("", "");
    ASSERT_FALSE(valid.validRequest(r1));
}

TEST(Validator, ValidingUpdateRequest)
{
    flexd::core::UpdateRequest r("Snake", "Snake", "/home/dev/work/SNAKE");
    ASSERT_TRUE(valid.validRequest(r));
}