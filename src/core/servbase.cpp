//
// Created by jiangsiyong on 2020/12/16.
//

#include "servbase.h"

#define MAX_CLIENT_QUEUE 50

void ServBase::startAt(const char *host, unsigned short port) {
    //在对应的地址和端口号上开始接受客户端连接
    _acceptor->acceptAt(host, port);
}

ServBase::ServBase() {
    _acceptor = new Acceptor();
}

ServBase::~ServBase() {
    delete _acceptor;
}
