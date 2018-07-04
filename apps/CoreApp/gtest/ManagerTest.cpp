#include <cstdlib>
#include <iostream>
#include <gtest/gtest.h>

#include "CoreAppManager.h"


flexd::core::CoreAppManager m("./CoreAppDb.db");

/*

TEST(Manager, Installrequest) {
    flexd::core::InstallRequest r("snakeinstall", "1", "/home/dev/work/SNAKE");
    //TODO add lambda because test fail. lambda call nullprt

    r.setOnError(on);
    r.setOnSucces(on);
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

TEST(Manager, Freezerequest) {
    flexd::core::FreezeRequest r("snakefreeze", "5");
    ASSERT_TRUE(m.tryProcesRequest(r));

}

TEST(Manager, Unfreezerequest) {
    flexd::core::UnfreezeRequest r("snakeunfreeze", "6");
    ASSERT_TRUE(m.tryProcesRequest(r));

}

TEST(Manager, Updaterequest) {
    flexd::core::UpdateRequest r("snakeupdate", "7", "/home/dev/work/SNAKE");
    ASSERT_TRUE(m.tryProcesRequest(r));

}*/
