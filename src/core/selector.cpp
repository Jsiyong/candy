//
// Created by jiangsiyong on 2020/12/17.
//
#include "../log/log.h"
#include "../util/file-util.h"
#include "selector.h"
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_EVENT 2000

Selector::Selector() {
    //初始化环境
    this->initInternal();

    _epfd = epoll_create(1);
    exit_if(_epfd < 0, "epoll_create error:%s", strerror(errno));
    info("epoll create success!!");
}

void Selector::addChannel(int fd, short fdtype, unsigned int events) {
    //找到一块空的地方放
    int pos = this->getFreePos();
    if (pos >= 0) {
        Channel *channel = new Channel(fd, fdtype);
        //存放起来，方便之后通过索引找到这一块东西
        _channels[pos] = channel;

        //设置为非阻塞
        struct epoll_event event;
        memset(&event, 0, sizeof(event));
        event.events = events;
        event.data.u64 = pos;//添加channel上去
        int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, channel->fd(), &event);
        exit_if(ret < 0, "epoll_ctl add error:%s", strerror(errno));
    } else {
        warn("no more space for channel");
    }
}

void Selector::select() {

    struct epoll_event events[MAX_EVENT];

    int eventCount = epoll_wait(_epfd, events, MAX_EVENT, -1);
    //epoll_wait有可能被信号打断，所以还要判断errno是不是为EINTR
    exit_if(eventCount == -1 && errno != EINTR, "epoll_wait error:%s", strerror(errno));
    //处理每一个事件
    for (int i = 0; i < eventCount; ++i) {
        int pos = events[i].data.u64;
        Channel *channel = _channels[pos];

        if (events[i].events & EPOLLIN) {
            //如果事件的fd是监听的fd，说明有新的客户端连接进来，客户端的fd采用边缘非阻塞形式
            if (Channel::LISTEN == channel->getType()) {
                this->doAccept(channel->fd());
            } else {
                //如果事件的fd不是监听的fd，说明有读写事件发生，由于是边缘非阻塞，所以需要注意要一次性读完缓冲区的所有数据
                //客户端的读事件产生
                channel->doRead();
                if (channel->getState() == Channel::CLOSE) {
                    this->removeChannelInternal(pos);
                }
            }
        }
    }

}

void Selector::doAccept(int servfd) {

    info("[servfd:%d]process accept", servfd);
    struct sockaddr_in clientaddr;
    socklen_t socklen = sizeof(struct sockaddr_in);

    int clientfd = ::accept(servfd, (struct sockaddr *) &clientaddr, &socklen);
    exit_if(clientfd < 0, "accept error:%s", strerror(errno));

    //设置他exec退出，同时设置为非阻塞
    int ret = FileUtil::addFlag2Fd(clientfd, FD_CLOEXEC | O_NONBLOCK);
    exit_if(ret < 0, "addFlag2Fd FD_CLOEXEC error:%s", strerror(errno));

    //将他添加进入select选择器中，采用边缘非阻塞形式触发
    this->addChannel(clientfd, Channel::CONN, EPOLLIN | EPOLLET);
}

Selector::~Selector() {
    for (int i = 0; i < MAX_EVENT; ++i) {
        delete _channels[i];
    }
    free(_channels);
}

void Selector::initInternal() {
    _channels = (Channel **) malloc(MAX_EVENT * sizeof(Channel *));
    memset(_channels, 0, MAX_EVENT);//清0
}

int Selector::getFreePos() {
    for (int i = 0; i < MAX_EVENT; ++i) {
        if (NULL == _channels[i]) {
            return i;
        }
    }
    return -1;
}

void Selector::removeChannelInternal(int pos) {
    //从epoll树上删除
    int clifd = _channels[pos]->fd();
    epoll_ctl(_epfd, EPOLL_CTL_DEL, clifd, NULL);
    info("epoll ctl delete");

    //删除节点的内存
    delete _channels[pos];
    _channels[pos] = NULL;
}

