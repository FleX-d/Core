#include <cstdlib>
#include <iostream>
#include <gtest/gtest.h>
#include "CoreAppExecutor.h"

flexd::core::CoreAppExecutor exe;

TEST(Executor, Test) {
    exe.runOsCmd("echo Test executor");
    ASSERT_NO_THROW(HasFailure());
}
