cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

set(PROJECT_NAME "ForwardList")

project(${PROJECT_NAME})

add_library(
        ${PROJECT_NAME}_lib
        STATIC
        my_forward_list.hpp
        my_forward_list.inl
        dummy.cpp
)

set_target_properties(
        ${PROJECT_NAME}_lib
        PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)

add_executable(
        ${PROJECT_NAME}
        main.cpp
        my_forward_list.hpp
        my_forward_list.inl
)

target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        ${PROJECT_NAME}_lib
)
