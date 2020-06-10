cmake_minimum_required(VERSION 3.10)

# set the project name
project(HappyFathersDay)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("${PROJECT_SOURCE_DIR}")

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/*.h"
        "${PROJECT_SOURCE_DIR}/*.hpp"
        "${PROJECT_SOURCE_DIR}/*.cpp"
        "${PROJECT_SOURCE_DIR}/*.c"
    )

# add the executable
add_executable(happyfathersday ${all_SRCS})
