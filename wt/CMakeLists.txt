CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
Project(wt.server)

set (CMAKE_CXX_STANDARD 14)

find_package(Wt REQUIRED Wt HTTP)

add_executable(client.wt simple.http.client.cpp)
add_executable(server.wt main.cpp postprocessor.h)

target_link_libraries(client.wt Wt::Wt Wt::HTTP)
target_link_libraries(server.wt Wt::Wt Wt::HTTP)