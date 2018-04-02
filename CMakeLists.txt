cmake_minimum_required(VERSION 3.9)
project(rc_p)

set(CMAKE_CXX_STANDARD 11)

add_executable(rc_p main.cpp)
target_link_libraries( rc_p pthread )