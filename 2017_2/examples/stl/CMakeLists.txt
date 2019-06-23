cmake_minimum_required(VERSION 3.8)
project(stl)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -Wall -ggdb3")

add_executable(035_vector 035_vector.cpp)
add_executable(036_vector_2 036_vector_2.cpp)
add_executable(037_vector_3.cpp 037_vector_3.cpp)
add_executable(040_erase_remove 040_erase_remove.cpp)
add_executable(041_equality_equivalent 041_equality_equivalent.cpp)
add_executable(042_crtp_class 042_crtp_class.cpp)





