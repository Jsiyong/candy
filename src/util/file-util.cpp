//
// Created by jiangsiyong on 2020/12/19.
//

#include "file-util.h"
#include <fcntl.h>

int FileUtil::addFlag2Fd(int fd, int flag) {
    int old = fcntl(fd, F_GETFL, 0);
    if (old < 0) {
        return old;
    }
    return fcntl(fd, F_SETFL, old | flag);
}

int FileUtil::setNonBlock(int fd) {
    int old = fcntl(fd, F_GETFL, 0);
    if (old < 0) {
        return old;
    }
    return fcntl(fd, F_SETFL, old | O_NONBLOCK);
}
