cmake_minimum_required(VERSION 3.10)
project(StackAllocator)

set(CMAKE_CXX_STANDARD 14)

enable_testing()

include_directories(library)

set(SOURCE_LIB main.cpp)
add_executable(main ${SOURCE_LIB})