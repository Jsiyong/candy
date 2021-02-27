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
    _port = DEFAULT_SERV_PORT;
    _host = DEFAULT_SERV_ADDR;
    _mode = Mode::FOREGROUND;
    _webRoot = DEFAULT_WEB_ROOT;

    //服务器运行路径
    char servPath[PATH_MAX] = {0};
    readlink(CURRENT_EXE_PATH, servPath, PATH_MAX);
    _serverPath = servPath;
}

void ServerConf::setPort(int port) {
    _port = port;
}

void ServerConf::setHost(const std::string &host) {
    _host = host;
}

void ServerConf::setMode(short mode) {
    _mode = mode;
}

int ServerConf::getPort() const {
    return _port;
}

const std::string &ServerConf::getHost() const {
    return _host;
}

std::string ServerConf::getWebRoot() const {
    return getServerDir() + "/" + _webRoot;
}

struct ServerConf serverConf;