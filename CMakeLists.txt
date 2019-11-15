cmake_minimum_required(VERSION 3.15)
project(ospf_mini_lib)

set(CMAKE_CXX_STANDARD 11)

add_executable(ospf_mini_lib ospf_mini_dep main/main.cpp)
add_executable(ospf_mini_sketch ospf_mini_dep ospf_mini_sketch/ospf_mini_sketch.ino)