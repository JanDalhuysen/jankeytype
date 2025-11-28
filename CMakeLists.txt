cmake_minimum_required(VERSION 3.10)

project(jankeytype)

find_package(raylib CONFIG REQUIRED)

add_custom_target(
    format
    COMMAND clang-format -style=Microsoft -i ${CMAKE_SOURCE_DIR}/main.cpp
    COMMENT "Running clang-format..."
)

add_executable(${PROJECT_NAME} main.cpp)
add_dependencies(${PROJECT_NAME} format)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
