#include <cstdlib>
#include <iostream>
#include <gtest/gtest.h>

#include "CoreAppManager.h"
#include "IPCClient.h"

flexd::core::CoreAppManager m("./CoreAppDb.db");

TEST(Manager, Installrequest) {
    flexd::core::InstallRequest r("snakeinstall", "1", "/home/dev/work/SNAKE");
    /*TODO set lambda for dont fall test*/
    ASSERT_TRUE(m.tryProcesRequest(r));
}

TEST(Manager, Uninstallrequest) {
    flexd::core::UninstallRequest r("snakeuninstall", "2");
    ASSERT_TRUE(m.tryProcesRequest(r));
    
}

TEST(Manager, Startrequest) {
    flexd::core::StartRequest r("snakestart", "3");
    ASSERT_TRUE(m.tryProcesRequest(r));
    
}

TEST(Manager, Stoprequest) {
    flexd::core::StopRequest r("snakestop", "4");
    ASSERT_TRUE(m.tryProcesRequest(r));
    
}

TEST(Manager, Freezrequest) {
    flexd::core::FreezRequest r("snakefreez", "5");
    ASSERT_TRUE(m.tryProcesRequest(r));
    
}

TEST(Manager, Unfreezrequest) {
    flexd::core::UnfreezRequest r("snakeunfreez", "6");
    ASSERT_TRUE(m.tryProcesRequest(r));
    
}

TEST(Manager, Updaterequest) {
    flexd::core::UpdateRequest r("snakeupdate", "7", "/home/dev/work/SNAKE");
    ASSERT_TRUE(m.tryProcesRequest(r));
    
}