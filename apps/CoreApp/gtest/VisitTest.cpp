#include <cstdlib>
#include <iostream>
#include <gtest/gtest.h>

#include "Visitor.h"

TEST(Visitor, Test)
{
    flexd::core::Visitor v;
    flexd::core::UnfreezeRequest_t r=new flexd::core::UnfreezeRequest("Snake","1.01");
    v.visit(r);
    r->accept(v);
    ASSERT_TRUE(true);
}
