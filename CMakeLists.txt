cmake_minimum_required(VERSION 3.8)
project(net)

# set(CMAKE_CXX_STANDARD 11) -std=c++0x
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++14 -Wall -ggdb3 -pthread")
set(SOCKET_LIB
        common/socket.cpp)

add_library(socket_lib STATIC ${SOCKET_LIB})

add_executable(multiplex multiplexing/multiplex.cpp)
add_executable(client_read_timeout client_read_timeout/client_timeouts.cpp)
add_executable(client_connect_timeout client_connect_timeout/client_timeouts.cpp)
add_executable(simple_server simple_server/simple_server.cpp)
# add_executable(lib_event_main libevent/main.cpp)
add_executable(http_client http_client/http_client.cpp)
#add_executable(asio asio/main.cpp)

target_link_libraries(client_read_timeout socket_lib)
target_link_libraries(client_connect_timeout socket_lib)
target_link_libraries(simple_server socket_lib)
# target_link_libraries(lib_event_main socket_lib "-levent")
target_link_libraries(http_client socket_lib)
target_link_libraries(multiplex socket_lib)
#target_link_libraries(asio socket_lib -lboost_system -lpthread -std=c++14)

add_executable(simpliest_client simplest_client_server/client.cpp)
add_executable(server_select simplest_client_server/server_select.cpp)
add_executable(server_fork simplest_client_server/server_fork.cpp)
add_executable(server_poll simplest_client_server/server_poll.cpp)
add_executable(server simplest_client_server/server.cpp)