cmake_minimum_required(VERSION 3.23)
project(HPC C)

# added -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

set(SOURCE_FILES main.c)
add_executable(openmp_test_clion ${SOURCE_FILES})
