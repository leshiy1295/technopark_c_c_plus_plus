cmake_minimum_required(VERSION 3.8)
set (CMAKE_CXX_STANDARD 14)

project(boost.asio.server)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

# Simple Sync Client
add_executable(sync.client  sync.client.cpp response.h)
target_link_libraries(sync.client  ${Boost_LIBRARIES})

# Async HTTP Client
add_executable(async.http.client  async.http.client.cpp async.http.server.h response.h)
target_link_libraries(async.http.client  ${Boost_LIBRARIES})

# Simple Async Server
add_executable(async.simple.server  async.simple.server.cpp)
target_link_libraries(async.simple.server  ${Boost_LIBRARIES})

# Multithread Async HTTP Server
add_executable(async.http.server connection.h connection.cpp request.h response.h router.h async.http.server.h async.http.server.cpp)
target_link_libraries(async.http.server  ${Boost_LIBRARIES})