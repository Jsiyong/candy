MESSAGE(STATUS "hello,now start to build project 'candy'")
cmake_minimum_required(VERSION 3.10)
project(candy)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE all_src_files src/*.cpp src/*.h)


add_executable(${PROJECT_NAME} ${all_src_files})