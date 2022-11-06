cmake_minimum_required(VERSION 3.10)

project(AnonymousMutex)

find_package(Threads REQUIRED)

add_executable("${PROJECT_NAME}main" main.cpp)
target_link_libraries("${PROJECT_NAME}main" PRIVATE rt ${CMAKE_THREAD_LIBS_INIT})
add_executable("${PROJECT_NAME}secondary" secondary.cpp)
target_link_libraries("${PROJECT_NAME}secondary" PRIVATE rt ${CMAKE_THREAD_LIBS_INIT})

