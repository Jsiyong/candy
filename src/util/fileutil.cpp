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
        error("open error: %s", strerror(errno));
        return false;
    }
    ExitCaller caller([pFile]() { fclose(pFile); });
    //然后计算文件长度
    int ret = fseek(pFile, 0, SEEK_END);
    if (ret < 0) {
        error("fseek error: %s", strerror(errno));
        return false;
    }
    long fileLen = ftell(pFile);
    //重新移到开始位置，开始读文件
    fseek(pFile, 0, SEEK_SET);
    //初始化dst的大小
    dst.resize(fileLen);
    //读取fileLen个单位的数据，每个单位1个字节
    fread((char *) dst.data(), 1, fileLen, pFile);
    return true;
}
