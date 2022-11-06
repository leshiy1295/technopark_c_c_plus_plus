cmake_minimum_required(VERSION 3.10)

project(SyncIO)

find_package(Boost REQUIRED COMPONENTS filesystem)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC Boost::filesystem)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::filesystem)
