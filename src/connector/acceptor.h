//
// Created by Jsiyong on 2021-02-07.
//

#ifndef CANDY_ACCEPTOR_H
#define CANDY_ACCEPTOR_H

#include "poller.h"
#include <atomic>

/**
 * 连接接收器[在单独一条线程一直接受连接]
 */
struct Acceptor {

    Acceptor();

    ~Acceptor();

    /**
     * 在指定的端口和地址上监听连接请求
     * @param host
     * @param port
     */
    void acceptAt(const std::string &host, int port);

    /**
     * 退出函数
     */
    void exitClose();

private:

    /**
     * 不停监听客户端连接
     */
    void acceptLoop();

    int _fd = 0;//服务器的监听fd
    std::atomic_bool _exit;//线程是否要退出
    Poller *_poller = NULL;//poller处理器

};

#endif //CANDY_ACCEPTOR_H
