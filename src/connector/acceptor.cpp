//
// Created by Jsiyong on 2021-02-07.
//

#include "acceptor.h"
#include "../util/fileutil.h"
#include "../log/logger.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENT_QUEUE 50
#define IP_SIZE 20

Acceptor::Acceptor() {
    _poller = new Poller();
}

void Acceptor::acceptAt(const std::string &host, int port) {
    //服务端监听套接字
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    exit_if(_fd < 0, "socket error:%s", strerror(errno));

    //设置端口复用
    int val = 1;
    int ret = setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    exit_if(ret < 0, "setsockopt set SO_REUSEADDR error:%s", strerror(errno));

    //设置fork exec自动close
    ret = FileUtil::addFlag2Fd(_fd, FD_CLOEXEC);
    exit_if(ret < 0, "addFlag2Fd FD_CLOEXEC error:%s", strerror(errno));

    //绑定端口
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(struct sockaddr_in));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    inet_pton(AF_INET, host.data(), &servAddr.sin_addr);
    ret = ::bind(_fd, (struct sockaddr *) &servAddr, sizeof(servAddr));
    exit_if(ret < 0, "bind error:%s", strerror(errno));

    //监听
    ret = ::listen(_fd, MAX_CLIENT_QUEUE);
    exit_if(ret < 0, "listen error:%s", strerror(errno));
    trace("\n\n"
          "#######   #####   #     #  ######   ##   ## \n"
          "##       #     #  # #   #  #     #    # #   \n"
          "##       #######  #  #  #  #     #     #    \n"
          "##       #     #  #   # #  #     #     #    \n"
          "#######  #     #  #     #  ######      #    run at http://%s:%d\n\n", host, port);
#if 0
    //设置非阻塞，防止在accept之前客户端发送RST然后出现卡在accept的情况
    FileUtil::setNonBlock(_fd);
#endif

    //开始接受客户端连接
    acceptLoop();
}

void Acceptor::acceptLoop() {
    struct sockaddr_in clientaddr{};
    socklen_t socklen = sizeof(struct sockaddr_in);
    char ip[IP_SIZE] = {0};
    int clientfd = 0;

    while (!_exit) {

        clientfd = ::accept(_fd, (struct sockaddr *) &clientaddr, &socklen);
        if (clientfd < 0) {
            fatal("accept error:%s", strerror(errno));
            return;
        }
        info("[servfd:%d]process accept...", _fd);

        inet_ntop(AF_INET, &clientaddr.sin_addr, ip, socklen);
        trace("[new conn]client host:%s, port:%d", ip, ntohs(clientaddr.sin_port));

        //将他添加进入poller选择器中，采用边缘非阻塞形式触发，监听可读事件和可写事件
        //EPOLLONESHOT避免同一个事件被触发多次
        _poller->attach(clientfd);
    }
}

Acceptor::~Acceptor() {
    _exit = true;
    delete _poller;
}

