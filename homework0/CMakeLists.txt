cmake_minimum_required (VERSION 2.8.11)
project (Transformation)

set (Eigen3_DIR /usr/include/eigen3)
find_package(Eigen3 REQUIRED)
include_directories(EIGEN3_INCLUDE_DIR /usr/include/eigen3)

add_executable (Transformation main.cpp)
