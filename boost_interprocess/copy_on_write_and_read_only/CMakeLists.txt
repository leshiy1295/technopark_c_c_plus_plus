cmake_minimum_required(VERSION 3.0)

project(CopyOnWriteAndReadOnly)

find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE rt ${CMAKE_THREAD_LIBS_INIT})
