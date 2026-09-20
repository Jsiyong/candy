//
// Created by Jsiyong on 2021-02-03.
//
#include "servconf.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#include <vector>
#endif

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

const std::string &ServerConf::getServerPath() {
    return _serverPath;
}

std::string ServerConf::getServerName() const {
    return _serverPath.substr(_serverPath.rfind('/') + 1);
}

std::string ServerConf::getServerDir() const {
    return _serverPath.substr(0, _serverPath.rfind('/'));
}

ServerConf::ServerConf() {
    //服务器运行路径
    char servPath[PATH_MAX] = {0};
#if defined(__APPLE__)
    uint32_t size = 0;
    _NSGetExecutablePath(NULL, &size);
    std::vector<char> buf(size > 0 ? size : 1);
    if (_NSGetExecutablePath(buf.data(), &size) == 0) {
        if (realpath(buf.data(), servPath) == NULL) {
            // realpath 失败时退回未解析路径（可能是相对路径）
            snprintf(servPath, sizeof(servPath), "%s", buf.data());
        }
    }
#else
    // Linux: /proc/self/exe
    ssize_t n = readlink("/proc/self/exe", servPath, sizeof(servPath) - 1);
    if (n >= 0) {
        servPath[n] = '\0';
    }
#endif
    _serverPath = servPath;
}

std::string ServerConf::getAbsoulteWebRoot() const {
    return getServerDir() + "/" + webRoot;
}

bool ServerConf::isToStop() const {
    return _toStop;
}

void ServerConf::setToStop(bool toStop) {
    _toStop = toStop;
}

const std::string &ServerConf::getConfigPath() const {
    return _configPath;
}

void ServerConf::setConfigPath(const std::string &configPath) {
    _configPath = configPath;
}

struct ServerConf serverConf;