cmake_minimum_required(VERSION 3.5)
project(ray_tracing)

set(CMAKE_CXX_STANDARD 14)

add_executable(ray_tracing main.cpp shapes.h config.h material.h)