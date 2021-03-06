//
// Created by jiangsiyong on 2020/12/16.
//

#ifndef CANDY_SERVER_H
#define CANDY_SERVER_H

#include "channel.h"
#include "poller.h"
#include "acceptor.h"

struct Server {

    Server();

    ~Server();

    /**
     * 循环执行监听
     * @param host
     * @param port
     */
    void execLoopAt(const std::string &host, unsigned short port);

    /**
     * 关闭服务器监听
     */
    void close();

private:

    Acceptor *_acceptor;
};

#endif //CANDY_SERVER_H
