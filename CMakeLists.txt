cmake_minimum_required(VERSION 3.15)
project(kitti_kit_raw VERSION 0.0.1 DESCRIPTION "tool for reading kitti raw dataset")

option(ENABLE_TESTS "build unit test apps" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLGS} -fPIC")

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -ggdb3")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(MAIN_LIB ${PROJECT_NAME})
add_subdirectory(src)

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

message("=======")
message(STATUS "build type      ${CMAKE_BUILD_TYPE}")
message(STATUS "enable testing  ${ENABLE_TESTS}")
message("=======")
