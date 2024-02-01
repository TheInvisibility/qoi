cmake_minimum_required(VERSION 3.27)
project(test)

set(CMAKE_CXX_STANDARD 17)

add_executable(test src/main.cpp
        src/qoi.h
        src/qoi.cpp
)
