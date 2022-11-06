cmake_minimum_required(VERSION 3.10)

project(Extensions)

find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC Boost::filesystem)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::filesystem ${CMAKE_THREAD_LIBS_INIT})
