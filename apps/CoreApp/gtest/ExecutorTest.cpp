#include <cstdlib>
#include <iostream>
#include <gtest/gtest.h>
#include "CoreAppExecutor.h"

flexd::core::CoreAppExecutor exe;

TEST(Executor, run_without_output) {
    exe.runOsCmd("echo Test executor");
    ASSERT_NO_THROW(HasFailure());
}

TEST(Executor, Test) {
    std::cout<<exe.runOsCmdWithResult("echo Test executor export text", "./")<<std::endl;
    ASSERT_NO_THROW(HasFailure());
}
