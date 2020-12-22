//
// Created by jiangsiyong on 2020/12/16.
//

#include "serv-base.h"
#include "../log/log.h"
#include "../util/file-util.h"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void ServBase::start(const char *host, unsigned short port) {
    //服务端监听套接字
    _servfd = socket(AF_INET, SOCK_STREAM, 0);
    exit_if(_servfd < 0, "socket error:%s", strerror(errno));

    //设置端口复用
    int val = 1;
    int ret = setsockopt(_servfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    exit_if(ret < 0, "setsockopt set SO_REUSEADDR error:%s", strerror(errno));

    //设置fork exec自动close
    ret = FileUtil::addFlag2Fd(_servfd, FD_CLOEXEC);
    exit_if(ret < 0, "addFlag2Fd FD_CLOEXEC error:%s", strerror(errno));

    //绑定端口
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(struct sockaddr_in));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    inet_pton(AF_INET, host, &servAddr.sin_addr);
    ret = ::bind(_servfd, (struct sockaddr *) &servAddr, sizeof(servAddr));
    exit_if(ret < 0, "bind error:%s", strerror(errno));

    //监听
    ret = ::listen(_servfd, 20);
    exit_if(ret < 0, "listen error:%s", strerror(errno));
    info("listen success: ip is %s, port is %d", host, port);


    //添加进去选择器中
    _selector->addChannel(_servfd, Channel::LISTEN, EPOLLIN);//监听她的读时间，采用水平触发，可以防止并发连接的时候丢失连接的情况[默认情况]

}

ServBase::ServBase() {
    _selector = new Selector();

}

ServBase::~ServBase() {
    delete _selector;
}

void ServBase::run() {
    //选择器开始选择
    while (1) {
        _selector->doSelect();
    }
}
