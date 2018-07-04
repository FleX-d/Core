cmake_minimum_required(VERSION 3.0)
project(CoreAppTest VERSION "0.0.1")

message(STATUS "***" )
message(STATUS "* " ${PROJECT_NAME})
message(STATUS "***")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTEST REQUIRED gtest>=1.7.0)
pkg_check_modules(GMOCK REQUIRED gmock>=1.7.0)

message(STATUS "GTEST_LIBRARIES: " ${GTEST_LIBRARIES})
message(STATUS "GTEST_LIBRARIES: " ${GMOCK_LIBRARIES})
message(STATUS "GTEST_LIBRARIES: " ${GTEST_INCLUDE_DIRS})
message(STATUS "GTEST_LIBRARIES: " ${GMOCK_INCLUDE_DIRS})
message(STATUS "CoreAppPublicInc: " ${CoreAppPublicInc})

SET(privateHeaders
    ${CoreApp_SOURCE_DIR}/inc/CoreAppDatabase.h
    ${CoreApp_SOURCE_DIR}/inc/CoreAppExecutor.h
    ${CoreApp_SOURCE_DIR}/inc/CoreAppManager.h
    ${CoreApp_SOURCE_DIR}/inc/CoreAppRequestFactory.h
    ${CoreApp_SOURCE_DIR}/inc/CoreAppRequestValidator.h
    ${CoreApp_SOURCE_DIR}/inc/CoreAppTypes.h
    ${CoreApp_SOURCE_DIR}/inc/FreezeRequest.h
    ${CoreApp_SOURCE_DIR}/inc/iApp.h
    ${CoreApp_SOURCE_DIR}/inc/iCoreAppAck.h
    ${CoreApp_SOURCE_DIR}/inc/iCoreAppRequest.h
    ${CoreApp_SOURCE_DIR}/inc/InstallRequest.h
    ${CoreApp_SOURCE_DIR}/inc/InvalidRequest.h
    ${CoreApp_SOURCE_DIR}/inc/IPCClient.h
    ${CoreApp_SOURCE_DIR}/inc/StartRequest.h
    ${CoreApp_SOURCE_DIR}/inc/StateMachine.h
    ${CoreApp_SOURCE_DIR}/inc/StopRequest.h
    ${CoreApp_SOURCE_DIR}/inc/UnfreezeRequest.h
    ${CoreApp_SOURCE_DIR}/inc/UninstallRequest.h
    ${CoreApp_SOURCE_DIR}/inc/UpdateRequest.h
    ${CoreApp_SOURCE_DIR}/inc/Visitor.h
    ${CoreApp_SOURCE_DIR}/inc/sqlite3pp.h
    ${CoreApp_SOURCE_DIR}/inc/sqlite3pp.ipp
    ${CoreApp_SOURCE_DIR}/inc/sqlite3ppext.h
    ${CoreApp_SOURCE_DIR}/inc/sqlite3ppext.ipp
    ${CoreApp_SOURCE_DIR}/inc/IPCInterface.h
)

SET(privateSources
    ${CoreApp_SOURCE_DIR}/src/CoreAppDatabase.cpp
    ${CoreApp_SOURCE_DIR}/src/CoreAppExecutor.cpp
    ${CoreApp_SOURCE_DIR}/src/CoreAppManager.cpp
    ${CoreApp_SOURCE_DIR}/src/CoreAppRequestFactory.cpp
    ${CoreApp_SOURCE_DIR}/src/CoreAppRequestValidator.cpp
    ${CoreApp_SOURCE_DIR}/src/FreezeRequest.cpp
    ${CoreApp_SOURCE_DIR}/src/iApp.cpp
    ${CoreApp_SOURCE_DIR}/src/iCoreAppAck.cpp
    ${CoreApp_SOURCE_DIR}/src/iCoreAppRequest.cpp
    ${CoreApp_SOURCE_DIR}/src/InstallRequest.cpp
    ${CoreApp_SOURCE_DIR}/src/InvalidRequest.cpp
    ${CoreApp_SOURCE_DIR}/src/IPCClient.cpp
    ${CoreApp_SOURCE_DIR}/src/StartRequest.cpp
    ${CoreApp_SOURCE_DIR}/src/StateMachine.cpp
    ${CoreApp_SOURCE_DIR}/src/StopRequest.cpp
    ${CoreApp_SOURCE_DIR}/src/UnfreezeRequest.cpp
    ${CoreApp_SOURCE_DIR}/src/UninstallRequest.cpp
    ${CoreApp_SOURCE_DIR}/src/UpdateRequest.cpp
    ${CoreApp_SOURCE_DIR}/src/Visitor.cpp
    ${CoreApp_SOURCE_DIR}/src/IPCInterface.cpp
    AckTest.cpp
    DatabaseTest.cpp
    ExecutorTest.cpp
    FactoryTest.cpp
    FullRunTest.cpp
    Main.cpp
    ManagerTest.cpp
    StateTest.cpp
    ValidatorTest.cpp
    VisitTest.cpp
)

include_directories(
    ${GTEST_INCLUDE_DIRS}
    ${GMOCK_INCLUDE_DIRS}
    ${CoreAppPublicInc}
    ${JSON_PARSER_INCLUDE_DIRS}
    ${libsqlite3-dev_INCLUDE_DIRS}
    ${FLEXD_LOGGER_INCLUDE_DIRS}
    ${FLEXD_EPOLL_INCLUDE_DIRS}
    ${FLEXD_CONNECTOR_INCLUDE_DIRS}
    ${FLEXD_IPCCOMMON_INCLUDE_DIRS}
)


set (CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wfloat-conversion")

add_executable(${PROJECT_NAME} Main.cpp ${privateSources} ${privateHeaders})

target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion)
target_link_libraries(${PROJECT_NAME}
    ${GTEST_LIBRARIES}
    ${GMOCK_LIBRARIES}
    ${JSON_PARSER_LIBRARIES}
    ${libsqlite3-dev_LIBRARIES}
    ${FLEXD_LOGGER_LIBRARIES}
    ${FLEXD_EPOLL_LIBRARIES}
    ${FLEXD_CONNECTOR_LIBRARIES}
    ${FLEXD_IPCCOMMON_LIBRARIES}
)
