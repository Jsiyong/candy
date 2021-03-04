//
// Created by jiangsiyong on 2020/12/19.
//

#include "fileutil.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../log/logger.h"
#include "./guard.h"
#include <dirent.h>
#include <sys/types.h>

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
//        error("stat[%s] error: %s", path, strerror(errno));
        return false;
    }
    //若输入的文件路径是普通文件
    return S_ISREG(buf.st_mode);
}

bool FileUtil::scanDirectory(const std::string &path, std::list<std::string> &folder, std::list<std::string> &file) {

    DIR *pDir;
    struct dirent *pEntry;
    struct dirent *pResult;
    if ((pDir = opendir(path.data())) == NULL) {
        error("opendir[%s] error: %s", path, strerror(errno));
        //文件夹无效
        return false;
    }
    pEntry = (struct dirent *) malloc(sizeof(struct dirent));
    pResult = (struct dirent *) malloc(sizeof(struct dirent));
    while (1) {
        if ((readdir_r(pDir, pEntry, &pResult)) != 0) {
            error("readdir_r[%s] error: %s", path, strerror(errno));
            return false;
        }
        if (pResult == NULL)
            break;
        if (pResult->d_name[0] == '.')
            continue;
        if (DT_DIR == pResult->d_type) {
            folder.push_back(pResult->d_name);
        } else if (DT_REG == pResult->d_type) {
            file.push_back(pResult->d_name);
        }
    }
    closedir(pDir);
    free(pEntry);
    free(pResult);

    return true;
}

bool FileUtil::getFileInfo(const std::string &path, struct stat &fileInfo) {
    if (stat(path.c_str(), &fileInfo) < 0) {
        error("stat[%s] error: %s", path, strerror(errno));
        return false;
    }
    return true;
}
