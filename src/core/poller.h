//
// Created by jiangsiyong on 2020/12/17.
//

#ifndef CANDY_POLLER_H
#define CANDY_POLLER_H

#include <list>
#include <sys/epoll.h>
#include "channel.h"

/**
 * 选择器，使用epoll实现
 */
struct Poller {
    int _epfd;

    Poller();

    ~Poller();

    /**
     * 添加一个频道
     * @param fd 文件描述符
     * @param type 文件描述符类型
     * @param events
     */
    void addChannel(int fd, unsigned int events);

    /**
     * 开始执行事件循环
     */
    void execEventLoop();

private:

    void removeChannelInternal(Channel *channel);

    std::list<Channel *> _channels;
};

#endif //CANDY_POLLER_H
