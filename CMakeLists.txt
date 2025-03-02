cmake_minimum_required(VERSION 3.29)
project(aes)

set(CMAKE_CXX_STANDARD 20)

add_library(aes STATIC src/world.cpp)

target_include_directories(aes PRIVATE
        include
)