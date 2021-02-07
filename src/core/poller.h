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
     * 添加一个频道，支持多线程添加epoll_ctl和epoll_wait线程安全
     * @param fd 文件描述符
     * @param type 文件描述符类型
     * @param events
     */
    void addChannel(int fd, unsigned int events);

private:

    /**
     * 开始执行事件循环
     */
    static void *execEventLoop(void *param);

    void removeChannelInternal(Channel *channel);

    std::list<Channel *> _channels;

    bool _exit = false;//线程是否退出

    pthread_t _threadId;//线程号
    pthread_mutex_t _mutex;//互斥量

};

#endif //CANDY_POLLER_H
