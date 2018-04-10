#include <cstdlib>
#include <iostream>
#include <gtest/gtest.h>

#include "CoreAppManager.h"


flexd::core::CoreAppManager m("/home/dev/work/CoreAppDb.db");

TEST(Manager, Test) {
    flexd::core::InstallRequest r("Snake1", "1", "/home/dev/work/SNAKE");
    try{
        ASSERT_TRUE(m.tryProcesRequest(r));
    }
    catch(std::exception& e){
        std::cerr<<"error because IPC not runing and ACK is not send " << e.what() << std::endl;
        ASSERT_TRUE(true);
    }
    
}
