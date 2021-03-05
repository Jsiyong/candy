//
// Created by jiangsiyong on 2020/12/16.
//

#include "server.h"

#define MAX_CLIENT_QUEUE 50

void Server::execLoopAt(const std::string &host, unsigned short port) {
    //在对应的地址和端口号上开始接受客户端连接
    _acceptor->acceptAt(host, port);
}

Server::Server() {
    _acceptor = new Acceptor();
}

Server::~Server() {
    delete _acceptor;
}
