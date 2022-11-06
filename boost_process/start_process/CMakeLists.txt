cmake_minimum_required(VERSION 3.10)

project(StartProcess)

find_package(Boost REQUIRED COMPONENTS filesystem)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${BOOST_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::filesystem)
