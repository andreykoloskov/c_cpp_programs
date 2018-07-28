cmake_minimum_required(VERSION 3.10)
project(lesson6)

set(CMAKE_CXX_STANDARD 17)

add_executable(lesson6 main.cpp token.cpp condition_parser.cpp condition_parser_test.cpp database.cpp date.cpp node.cpp)