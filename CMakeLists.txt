cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

set(PROJECT_NAME "Fast-Track-Telecom")
project(${PROJECT_NAME})

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/Bitwise-Operations
)

#add_subdirectory(
#        ${CMAKE_CURRENT_SOURCE_DIR}/Forward-List
#)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/Tokenize
)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/Client-Server
)