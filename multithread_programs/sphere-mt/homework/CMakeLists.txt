cmake_minimum_required(VERSION 3.10)
project(allocator)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(01-allocator)
include_directories(thirdparty)

add_executable(allocator
        01-allocator/allocator.cpp
        01-allocator/allocator_error.cpp
        01-allocator/allocator_pointer.cpp
        01-allocator/allocator_test.cpp
        thirdparty/gtest/gtest.h
        thirdparty/gtest/gtest-all.cc
        thirdparty/gtest/gtest_main.cc)