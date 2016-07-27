cmake_minimum_required(VERSION 3.5)
project(Custom)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp FixedAllocator.cpp FixedAllocator.h)
add_executable(Custom ${SOURCE_FILES})