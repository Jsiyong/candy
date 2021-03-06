//
// Created by jiangsiyong on 2020/12/17.
//

#ifndef CANDY_POLLER_H
#define CANDY_POLLER_H

#include <unordered_map>
#include <sys/epoll.h>
#include "channel.h"
#include "processor.h"

/**
 * 选择器，使用epoll实现
 */
struct Poller : public Runnable {
    Poller();

    ~Poller() override;

    /**
     * 添加一个频道，支持多线程添加epoll_ctl和epoll_wait线程安全
     * @param fd 文件描述符
     * @param events
     */
    void attach(int fd);

private:

    /**
     * 添加一个文件描述符进入epoll
     * @param fd
     * @param nonblock 非阻塞
     */
    void addEvent(int fd, bool nonblock = true);

    /**
     * 修改文件描述符
     * @param fd
     */
    void updateEvent(int fd, uint32_t events);

    /**
     *
     * 移除文件描述符
     * @param fd
     */
    void removeEvent(int fd);

    void run() override;

    int _epfd = 0;

    int _eventfd;//负责告诉epoll_wait需要退出了

    std::unordered_map<int, SocketProcessor *> _socketProcessors;//key: 客户端fd, value: socket处理对象
    ThreadPoolExecutor *_executor = NULL;
    bool _exit = false;//线程是否退出

    pthread_mutex_t _mutex;//互斥量

};

#endif //CANDY_POLLER_H
