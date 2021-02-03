message(STATUS "#### hello,now start to build project")
cmake_minimum_required(VERSION 3.10)
project(candy)
message(STATUS "#### current project ${PROJECT_NAME}")
set(CMAKE_CXX_STANDARD 11)

# 遍历目录下所有的.h和.cpp文件
file(GLOB_RECURSE all_src_files src/*.cpp src/*.h)

# 打印一下文件信息
foreach (src_file ${all_src_files})
    message(STATUS "add a file ${src_file}")
endforeach (src_file ${all_src_files})

add_executable(${PROJECT_NAME} ${all_src_files})
target_link_libraries(${PROJECT_NAME} pthread)

# 添加测试程序
message(STATUS "#### test case")
file(GLOB_RECURSE exclude_file src/bootstrap.cpp)
message(STATUS "exclude a file ${exclude_file}")

set(test_src_file ${all_src_files})
# 测试文件需要排除bootstrap.cpp
list(REMOVE_ITEM test_src_file ${exclude_file})

add_subdirectory(test)