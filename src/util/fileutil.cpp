//
// Created by jiangsiyong on 2020/12/19.
//

#include "fileutil.h"
#include <fcntl.h>
#include "../log/logger.h"
#include "./guard.h"

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

bool FileUtil::readFile(const std::string &path, std::string &dst) {

    //先以读形式打开文件
    FILE *pFile = fopen(path.c_str(), "rb");
    if (!pFile) {
        error("fopen error: %s", strerror(errno));
        return false;
    }
    ExitCaller caller([=]() { fclose(pFile); });
    //移到文件尾，计算大小
    fseek(pFile, 0, SEEK_END);
    long len = ftell(pFile);
    //重新定位到文件头
    rewind(pFile);
    dst.resize(len, '\0');
    fread((char *) dst.data(), 1, len, pFile);
    return true;
}
