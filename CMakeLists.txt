cmake_minimum_required(VERSION 3.6)
project(openmp_test_clion)

find_package(OpenMP REQUIRED)
add_executable(main main.c)
target_link_libraries(main PRIVATE OpenMP::OpenMP_C)
