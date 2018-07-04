#include <cstdlib>
#include <iostream>
#include <gtest/gtest.h>

#include "iCoreAppAck.h"


flexd::core::iCoreAppAck ackSuccess(flexd::core::RqstAck::success, "Snake", "Snake");
flexd::core::iCoreAppAck ackFail(flexd::core::RqstAck::fail, "Snake", "Snake");

TEST(Ack, Testing_creating_ack)
{
    ASSERT_EQ(ackSuccess.getName(), ackFail.getName());
    ASSERT_EQ(ackSuccess.getVersion(), ackFail.getVersion());
    ASSERT_NE(ackSuccess.getType(), ackFail.getType());
    std::cout<< ackSuccess.getType() <<std::endl;
}

