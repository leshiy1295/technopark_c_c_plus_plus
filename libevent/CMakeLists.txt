cmake_minimum_required(VERSION 3.8)
set (CMAKE_CXX_STANDARD 14)

project(libevent.server)

find_package(libevent REQUIRED)
include_directories(${LIBEVENT_INCLUDE_DIRS})

add_executable(libevent.server main.cpp)
target_link_libraries(libevent.server ${LIBEVENT_LIBRARIES})

add_executable(libevent.client http.client.cpp)
target_link_libraries(libevent.client ${LIBEVENT_LIBRARIES})
