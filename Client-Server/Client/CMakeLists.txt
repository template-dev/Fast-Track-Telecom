cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

set(PROJECT_NAME "Client")
project(${PROJECT_NAME})

add_executable(
    ${PROJECT_NAME}
    client.cpp
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)