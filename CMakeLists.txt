cmake_minimum_required(VERSION 3.0)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "setting default value 'Debug' for CMAKE_BUILD_TYPE")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

option(
    ENABLE_TESTS
    "Run Test?"
    ON
    )
    
message(STATUS "option ENABLE_TESTS=" ${ENABLE_TESTS})

add_subdirectory(apps)
