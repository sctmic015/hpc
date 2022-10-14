cmake_minimum_required(VERSION 3.23)
project(HPC C)

set(CMAKE_C_STANDARD 99)

add_executable(HPC helloWorld.c serialVersion.cpp main.c)
