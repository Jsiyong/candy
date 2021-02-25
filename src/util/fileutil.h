//
// Created by jiangsiyong on 2020/12/19.
//

#ifndef CANDY_FILEUTIL_H
#define CANDY_FILEUTIL_H

#include <fcntl.h>
#include <string>

/**
 * 文件相关工具类
 */
struct FileUtil {
    /**
     * 为文件描述符添加标志
     * @param fd
     * @param flag
     * @return
     */
    static int addFlag2Fd(int fd, int flag);

    /**
     * 设置非阻塞形式
     * @param fd
     * @return
     */
    static int setNonBlock(int fd);


    /**
     * 读取文件到dst中
     * @param path 路径
     * @param dst
     * @return
     */
    static bool readFile(const std::string &path, std::string &dst);

};

#endif //CANDY_FILEUTIL_H
