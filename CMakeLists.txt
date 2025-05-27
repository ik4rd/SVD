cmake_minimum_required(VERSION 3.30)
project(SVD C)

set(CMAKE_C_STANDARD 23)

file(GLOB_RECURSE SVD_SOURCES
        ${PROJECT_SOURCE_DIR}/src/*.c
)

add_executable(svd ${SVD_SOURCES})

target_include_directories(svd
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
