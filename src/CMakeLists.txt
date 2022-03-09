cmake_minimum_required(VERSION 3.0)

project(lapack-example)

# set the flags, may want to add/change debugging or optimization (-O3) flags as appropriate
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -std=c++11 -DHAVE_BLAS=1 -DHAVE_LAPACK=1")

find_package (Eigen3 3.3.4 REQUIRED NO_MODULE)

add_executable(lapack-example
lapack-example.cpp
)

target_link_libraries(lapack-example Eigen3::Eigen lapack )
