cmake_minimum_required(VERSION 3.7...3.30)

project(marching-squares)

add_library(marching_squares marching_squares.cpp)

target_include_directories(marching_squares ${CMAKE_SOURCE_DIRECTORY}/glm)
