cmake_minimum_required(VERSION 3.12)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

project(courses)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDART_REQUIRED ON)

if(WIN32 AND MSVC AND MSVC_VERSION GREATER 1400)
    add_compile_options(/MP)
endif()

set (MAIN_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/main.cmake")
set (INNER_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/inner.cmake")

link_directories(${BOOST_LIBRARYDIR})
include_directories(${BOOST_INCLUDEDIR})

set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads)

add_subdirectory("courses")
