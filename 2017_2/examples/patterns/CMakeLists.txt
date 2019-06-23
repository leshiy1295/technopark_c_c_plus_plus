cmake_minimum_required(VERSION 3.8)
project(patterns)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -Wall -ggdb3")

add_executable(029_abstract_factory 029_abstract_factory.cpp)
add_executable(030_adapter 030_adapter.cpp)
add_executable(031_bridge 031_bridge.cpp)
add_executable(032_command 032_command.cpp)
add_executable(033_observer 033_observer.cpp)
add_executable(034_visitor 034_visitor.cpp)




