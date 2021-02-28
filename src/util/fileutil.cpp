//
// Created by jiangsiyong on 2020/12/19.
//

#include "fileutil.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
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
        error("fopen[%s] error: %s", path, strerror(errno));
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

bool FileUtil::isExists(const std::string &path) {
    int ret = access(path.c_str(), F_OK);
    error_if(ret != 0, "access[%s] error: %s", path, strerror(errno));
    return ret == 0;
}

bool FileUtil::isRegularFile(const std::string &path) {
    struct stat buf{0};
    //获取文件信息，把信息放到buf中
    int ret = stat(path.c_str(), &buf);
    if (ret < 0) {
        error("stat[%s] error: %s", path, strerror(errno));
        return false;
    }
    //若输入的文件路径是普通文件
    return S_ISREG(buf.st_mode);
}
