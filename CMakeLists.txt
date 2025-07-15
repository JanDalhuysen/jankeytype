cmake_minimum_required(VERSION 3.10)

project(jankeytype)

find_package(raylib CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
