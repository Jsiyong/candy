//
// Created by jiangsiyong on 2020/12/17.
//
#include "../log/logger.h"
#include "poller.h"
#include <unistd.h>

#define MAX_EVENT 1024
#define IP_SIZE 20

Poller::Poller() {

    _epfd = epoll_create(1);
    exit_if(_epfd < 0, "epoll_create error:%s", strerror(errno));
    info("epoll create success!!");

    //初始化互斥锁
    pthread_mutex_init(&_mutex, NULL);
    //创建监听线程
    pthread_create(&_threadId, NULL, &Poller::execEventLoop, this);
}

void Poller::addChannel(int fd, unsigned int events) {
    //找到一块空的地方放
    Channel *channel = new Channel(fd);
    //存放起来，方便之后通过索引找到这一块东西
    _channels.push_back(channel);
    info("current client num:%d", _channels.size() - 1);

    //设置为非阻塞
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = events;
    event.data.ptr = channel;//添加channel上去
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
            Channel *channel = (Channel *) events[i].data.ptr;

            if (events[i].events & EPOLLIN) {
                //如果事件的fd不是监听的fd，说明有读写事件发生，由于是边缘非阻塞，所以需要注意要一次性读完缓冲区的所有数据
                //客户端的读事件产生
                channel->read();
                HttpRequest *pRequest = channel->getHttpRequest();
                pRequest->tryDecode(channel->getReadBuff());
#if 0
                info("[method]%s", pRequest->getMethod());
                info("[url]%s", pRequest->getUrl());
                info("[request params]");
                for (auto &p : pRequest->getRequestParams()) {
                    trace("++ key: %s", p.first);
                    trace("-- value: %s", p.second);
                }

                info("[protocol]%s", pRequest->getProtocol());
                info("[version]%s", pRequest->getVersion());
                info("[request headers]");
                for (auto &h : pRequest->getHeaders()) {
                    trace("++ key: %s", h.first);
                    trace("-- value: %s", h.second);
                }
                info("[body]%s", pRequest->getBody());
#endif
                warn("[body size]%lld", pRequest->getBody().size());

                //如果是可写事件触发
                if (pRequest->getDecodeState() == HttpRequestDecodeState::COMPLETE) {
                    channel->write();
                }

//                if (channel->getState() == Channel::CLOSE) {
//                    pPoller->removeChannelInternal(channel);
//                }
            }
        }
    }
}

Poller::~Poller() {
    for (Channel *channel:_channels) {
        delete channel;
    }
    _exit = true;
    pthread_join(_threadId, NULL);
    pthread_mutex_destroy(&_mutex);
}

void Poller::removeChannelInternal(Channel *channel) {
    //从epoll树上删除
    int clifd = channel->fd();
    epoll_ctl(_epfd, EPOLL_CTL_DEL, clifd, NULL);
    _channels.remove(channel);

    info("epoll ctl delete");
    trace("client disconnected.ip:%s, port:%d,current client num:%d", channel->getHost(), channel->getPort(),
          _channels.size() - 1);

    //删除节点的内存
    delete channel;
}

