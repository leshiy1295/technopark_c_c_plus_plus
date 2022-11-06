cmake_minimum_required(VERSION 3.10)

project(NamedMutex)

find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME}1 main.cpp)
target_link_libraries(${PROJECT_NAME}1 PRIVATE ${CMAKE_THREAD_LIBS_INIT})
add_executable(${PROJECT_NAME}2 main.cpp)
target_link_libraries(${PROJECT_NAME}2 PRIVATE ${CMAKE_THREAD_LIBS_INIT})
