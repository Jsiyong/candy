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
    // FD_CLOEXEC is a descriptor flag (F_GETFD/F_SETFD); the rest are status
    // flags (F_GETFL/F_SETFL). Mixing them in F_SETFL is ignored on Linux but
    // can change access-mode bits on macOS.
    int ret = 0;
    int fdFlags = flag & FD_CLOEXEC;
    int statusFlags = flag & ~FD_CLOEXEC;
    if (statusFlags != 0) {
        int old = fcntl(fd, F_GETFL, 0);
        if (old < 0) {
            return old;
        }
        ret = fcntl(fd, F_SETFL, old | statusFlags);
        if (ret < 0) {
            return ret;
        }
    }
    if (fdFlags != 0) {
        int old = fcntl(fd, F_GETFD, 0);
        if (old < 0) {
            return old;
        }
        ret = fcntl(fd, F_SETFD, old | fdFlags);
    }
    return ret;
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

bool FileUtil::scanDirectory(const std::string &path, std::list<std::string> &results) {

    DIR *pDir;
    if ((pDir = opendir(path.data())) == NULL) {
        error("opendir[%s] error: %s", path, strerror(errno));
        //文件夹无效
        return false;
    }
    // readdir_r is deprecated/removed on macOS; readdir on a private DIR* is fine.
    errno = 0;
    struct dirent *pResult = NULL;
    while ((pResult = readdir(pDir)) != NULL) {
        if (pResult->d_name[0] == '.')
            continue;
        if (DT_DIR == pResult->d_type || DT_REG == pResult->d_type) {
            //正规的文件夹和文件才加入里面
            results.emplace_back(pResult->d_name);
        }
        errno = 0;
    }
    if (errno != 0) {
        error("readdir[%s] error: %s", path, strerror(errno));
        closedir(pDir);
        return false;
    }
    closedir(pDir);

    return true;
}

bool FileUtil::getFileInfo(const std::string &path, struct stat &fileInfo) {
    if (stat(path.c_str(), &fileInfo) < 0) {
        error("stat[%s] error: %s", path, strerror(errno));
        return false;
    }
    return true;
}

bool FileUtil::writeFile(const std::string &path, const std::string &src, mode_t mode) {
    //先以读形式打开文件
    FILE *pFile = fopen(path.c_str(), "wb");
    if (!pFile) {
        error("fopen[%s] error: %s", path, strerror(errno));
        return false;
    }
    ExitCaller caller([=]() { fclose(pFile); });
    //改变文件的权限位
    if (chmod(path.c_str(), mode) != 0) {
        error("chmod error: %s", strerror(errno));
        return false;
    }

    fwrite(src.data(), src.size(), 1, pFile);
    return true;
}

bool FileUtil::createFolder(const std::string &path, mode_t mode) {

    int ret = mkdir(path.c_str(), mode);
    if (ret != 0) {
        error("mkdir error:%s", strerror(errno));
        return false;
    }
    return true;
}

bool FileUtil::renameFile(const std::string &src, const std::string &target) {
    if (rename(src.c_str(), target.c_str()) != 0) {
        error("rename[src:%s, target:%s] error: %s", src, target, strerror(errno));
        return false;
    }
    return true;
}

bool FileUtil::deleteFile(const std::string &file) {
    if (unlink(file.c_str()) != 0) {
        error("unlink[%s] error: %s", file, strerror(errno));
        return false;
    }
    return true;
}

bool FileUtil::deleteFolder(const std::string &path) {
    if (rmdir(path.c_str()) != 0) {
        error("rmdir[%s] error: %s", path, strerror(errno));
        return false;
    }
    return true;
}
