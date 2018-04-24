cmake_minimum_required(VERSION 3.10)
project(StackAllocator)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_LIB StackAllocator.cpp main.cpp)
add_executable(main ${SOURCE_LIB})