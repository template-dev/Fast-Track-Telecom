cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

set(PROJECT_NAME "ForwardList")

project(${PROJECT_NAME})

add_executable(
        ${PROJECT_NAME}
        my_forward_list.hpp
        my_forward_list.inl
        main.cpp
)

set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)