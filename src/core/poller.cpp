//
// Created by jiangsiyong on 2020/12/17.
//
#include "../log/logger.h"
#include "poller.h"
#include <unistd.h>

#define MAX_EVENT 1024
#define IP_SIZE 20

Poller::Poller() {

    //线程池，核心线程5个，最大线程10个，每隔30秒查询一次
    _executor = new ThreadPoolExecutor(5, 10, 30);

    _epfd = epoll_create(1);
    exit_if(_epfd < 0, "epoll_create error:%s", strerror(errno));
    info("epoll create success!!");

    //初始化互斥锁
    pthread_mutex_init(&_mutex, NULL);
    //创建监听线程
    pthread_create(&_threadId, NULL, &Poller::execEventLoop, this);
}

void Poller::addChannel(int fd, unsigned int events) {
    static int clientNum = 0;
    clientNum++;
    //找到一块空的地方放
    //封装为socketProcessor
    SocketProcessor *socketProcessor = new SocketProcessor(fd);
    _socketProcessors.push_back(socketProcessor);
    info("current client num:%d", clientNum);

    //设置为非阻塞
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = events;
    event.data.ptr = socketProcessor;//添加channel上去
    int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &event);
    exit_if(ret < 0, "epoll_ctl add error:%s", strerror(errno));
}

void *Poller::execEventLoop(void *param) {

    Poller *pPoller = (Poller *) param;

    struct epoll_event events[MAX_EVENT];

    while (!pPoller->_exit) {

        int eventCount = epoll_wait(pPoller->_epfd, events, MAX_EVENT, -1);
        //epoll_wait有可能被信号打断，所以还要判断errno是不是为EINTR
        exit_if(eventCount == -1 && errno != EINTR, "epoll_wait error:%s", strerror(errno));
        //处理每一个事件
        for (int i = 0; i < eventCount; ++i) {
            SocketProcessor *pSocketProcessor = (SocketProcessor *) events[i].data.ptr;
            if (events[i].events & EPOLLIN) {
                //提交任务
                pPoller->_executor->submit(pSocketProcessor);
            }
        }
    }
}

Poller::~Poller() {
    for (SocketProcessor *socketProcessor:_socketProcessors) {
        delete socketProcessor;
    }
    _exit = true;
    pthread_join(_threadId, NULL);
    pthread_mutex_destroy(&_mutex);
}

void Poller::removeChannelInternal(Channel *channel) {
#if 0
    //从epoll树上删除
    int clifd = channel->fd();
    epoll_ctl(_epfd, EPOLL_CTL_DEL, clifd, NULL);
    _channels.remove(channel);

    info("epoll ctl delete");
    trace("client disconnected.ip:%s, port:%d,current client num:%d", channel->getHost(), channel->getPort(),
          _channels.size() - 1);

    //删除节点的内存
    delete channel;
#endif
}

