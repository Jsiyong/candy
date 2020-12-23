//
// Created by jiangsiyong on 2020/12/17.
//

#ifndef CANDY_SELECTOR_H
#define CANDY_SELECTOR_H

#include <list>
#include "channel.h"

/**
 * 选择器，使用epoll实现
 */
struct Selector {
    int _epfd;

    Selector();

    ~Selector();

    /**
     * 添加一个频道
     * @param fd 文件描述符
     * @param type 文件描述符类型
     * @param events
     */
    void addChannel(int fd, short fdtype, unsigned int events);


    void doAccept(int servfd);

    /**
     * 开始执行选择
     */
    void doSelect();

private:

    void removeChannelInternal(Channel *channel);

    std::list<Channel *> _channels;
};

#endif //CANDY_SELECTOR_H
