# this file is a part of AES project which is under the MIT license; See LICENSE for more info

cmake_minimum_required(VERSION 3.29)
project(aes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(aes STATIC
        src/utils.cpp
        src/world.cpp
)

target_include_directories(aes PUBLIC
        include
)

enable_testing()
add_subdirectory(tests)
