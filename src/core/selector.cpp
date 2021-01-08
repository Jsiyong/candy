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
#include <arpa/inet.h>

#define MAX_EVENT 1024
#define IP_SIZE 20

Selector::Selector() {

    _epfd = epoll_create(1);
    exit_if(_epfd < 0, "epoll_create error:%s", strerror(errno));
    info("epoll create success!!");
}

void Selector::addChannel(int fd, short fdtype, unsigned int events) {
    //找到一块空的地方放
    Channel *channel = new Channel(fd, fdtype);
    //存放起来，方便之后通过索引找到这一块东西
    _channels.push_back(channel);
    info("current client num:%d", _channels.size() - 1);

    //设置为非阻塞
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = events;
    event.data.ptr = channel;//添加channel上去
    int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, channel->fd(), &event);
    exit_if(ret < 0, "epoll_ctl add error:%s", strerror(errno));
}

void Selector::doSelect() {

    struct epoll_event events[MAX_EVENT];

    int eventCount = epoll_wait(_epfd, events, MAX_EVENT, -1);
    //epoll_wait有可能被信号打断，所以还要判断errno是不是为EINTR
    exit_if(eventCount == -1 && errno != EINTR, "epoll_wait error:%s", strerror(errno));
    //处理每一个事件
    for (int i = 0; i < eventCount; ++i) {
        Channel *channel = (Channel *) events[i].data.ptr;

        if (events[i].events & EPOLLIN) {
            //如果事件的fd是监听的fd，说明有新的客户端连接进来，客户端的fd采用边缘非阻塞形式
            if (Channel::LISTEN == channel->getType()) {
                this->doAccept(channel->fd());
            } else {
                //如果事件的fd不是监听的fd，说明有读写事件发生，由于是边缘非阻塞，所以需要注意要一次性读完缓冲区的所有数据
                //客户端的读事件产生
                channel->doRead();
                HttpRequest *pRequest = channel->getHttpRequest();
                pRequest->tryDecode();

                info("[method]%s", pRequest->getMethod().c_str());
                info("[url]%s", pRequest->getUrl().c_str());
                info("[request params]");
                for (auto &p : pRequest->getRequestParams()) {
                    trace("++ key: %s", p.first.c_str());
                    trace("-- value: %s", p.second.c_str());
                }

                info("[protocol]%s", pRequest->getProtocol().c_str());
                info("[version]%s", pRequest->getVersion().c_str());
                info("[request headers]");
                for (auto &h : pRequest->getHeaders()) {
                    trace("++ key: %s", h.first.c_str());
                    trace("-- value: %s", h.second.c_str());
                }
                info("[body]%s", pRequest->getBody().c_str());

                if (channel->getState() == Channel::CLOSE) {
                    this->removeChannelInternal(channel);
                }
            }
        }
        //如果是可写事件触发
        if (events[i].events & EPOLLOUT) {
            channel->doWrite();
        }
    }

}

void Selector::doAccept(int servfd) {

    info("[servfd:%d]process accept", servfd);
    struct sockaddr_in clientaddr;
    socklen_t socklen = sizeof(struct sockaddr_in);

    int clientfd = ::accept(servfd, (struct sockaddr *) &clientaddr, &socklen);
    if (clientfd < 0) {
        fatal("accept error:%s", strerror(errno));
        return;
    }

    char ip[IP_SIZE] = {0};
    inet_ntop(AF_INET, &clientaddr.sin_addr, ip, socklen);
    trace("[new conn]client host:%s, port:%d", ip, ntohs(clientaddr.sin_port));

    //设置他exec退出，同时设置为非阻塞
    int ret = FileUtil::addFlag2Fd(clientfd, FD_CLOEXEC | O_NONBLOCK);
    exit_if(ret < 0, "addFlag2Fd FD_CLOEXEC error:%s", strerror(errno));

    //将他添加进入select选择器中，采用边缘非阻塞形式触发，监听可读事件和可写事件
    this->addChannel(clientfd, Channel::CONN, EPOLLIN | EPOLLOUT | EPOLLET);
}

Selector::~Selector() {
    for (Channel *channel:_channels) {
        delete channel;
    }
}

void Selector::removeChannelInternal(Channel *channel) {
    //从epoll树上删除
    int clifd = channel->fd();
    epoll_ctl(_epfd, EPOLL_CTL_DEL, clifd, NULL);
    _channels.remove(channel);

    info("epoll ctl delete");
    trace("client disconnected.ip:%s, port:%d,current client num:%d", channel->getHost().data(), channel->getPort(),
          _channels.size() - 1);

    //删除节点的内存
    delete channel;
}

