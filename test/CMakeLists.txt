# 取得test目录下的所有文件
aux_source_directory(. test_src_list)

# 循环添加
foreach (test_src ${test_src_list})
    # 去掉文件名小数点之后的.cpp字样
    string(REGEX REPLACE "^\\./|\\.c[a-zA-Z0-9_]*$" "" test_exe_name ${test_src})
    message(STATUS "#### add a test case -- ${test_exe_name}")
    # 添加为exe程序
    add_executable(${test_exe_name} ${test_src} ${test_src_file})
    target_link_libraries(${test_exe_name} pthread)

endforeach (test_src ${test_src_list})