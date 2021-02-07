//
// Created by jiangsiyong on 2020/12/19.
//

#ifndef CANDY_FILEUTIL_H
#define CANDY_FILEUTIL_H

#include <fcntl.h>

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
};

#endif //CANDY_FILEUTIL_H
