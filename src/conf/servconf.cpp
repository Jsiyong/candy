//
// Created by Jsiyong on 2021-02-03.
//
#include "servconf.h"
#include <unistd.h>

//代表程序的路径，可以通过访问这个路径来获取
#define CURRENT_EXE_PATH "/proc/self/exe"
#define PATH_MAX 1024

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
    readlink(CURRENT_EXE_PATH, servPath, PATH_MAX);
    _serverPath = servPath;
}

std::string ServerConf::getAbsoulteWebRoot() const {
    return getServerDir() + "/" + webRoot;
}

struct ServerConf serverConf;