#include <cstdlib>
#include <iostream>
#include <gtest/gtest.h>

#include "iCoreAppAck.h"


flexd::core::iCoreAppAck acks(flexd::core::RqstAck::succes, "Snake", "Snake");
flexd::core::iCoreAppAck acke(flexd::core::RqstAck::fail, "Snake", "Snake");

TEST(Ack, Testing_creating_ack)
{
    ASSERT_EQ(acks.getName(), acke.getName());
    ASSERT_EQ(acks.getVersion(), acke.getVersion());
    ASSERT_NE(acks.getType(), acke.getType());
    std::cout<< acks.getType() <<std::endl;
}

