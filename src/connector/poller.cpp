//
// Created by jiangsiyong on 2020/12/17.
//
#include "../log/logger.h"
#include "poller.h"
#include "../util/fileutil.h"
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

void Poller::attach(int fd) {
    static int clientNum = 0;
    clientNum++;
    //找到一块空的地方放
    //封装为socketProcessor
    SocketProcessor *socketProcessor = new SocketProcessor(fd);
    //解析完协议之后，就需要开启epoll的写事件,
    socketProcessor->setOnAfterReadCompletedRequest([this, fd]() {
        this->updateEvent(fd, EPOLLOUT);
    });
    //解析到未完整的协议，就重新开启epoll读事件
    socketProcessor->setOnAfterReadUnCompletedRequest([this, fd]() {
        this->updateEvent(fd, EPOLLIN);
    });
    //写完完就开始重新触发读事件
    socketProcessor->setOnAfterWriteCompletedResponse([this, fd]() {
        this->updateEvent(fd, EPOLLIN);
    });
    //写不完就继续写
    socketProcessor->setOnAfterWriteUnCompletedResponse([this, fd]() {
        this->updateEvent(fd, EPOLLOUT);
    });

    //TODO 需要加锁
    _socketProcessors.insert(std::make_pair(fd, socketProcessor));
    info("current client num:%d", clientNum);

    //添加事件
    this->addEvent(fd);
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
            SocketProcessor *pSocketProcessor = pPoller->_socketProcessors[events[i].data.fd];
            if (events[i].events & (EPOLLIN | EPOLLOUT)) {//可读事件和可写事件都进入这个地方
                //提交任务
                pPoller->_executor->submit(pSocketProcessor);
            }
            if (events[i].events & EPOLLRDHUP) {//客户端断开了连接
                int clifd = pSocketProcessor->getChannel()->fd();
                pPoller->removeEvent(clifd);
                close(clifd);
            }
        }
    }
}

Poller::~Poller() {
    _exit = true;
    pthread_join(_threadId, NULL);
    pthread_mutex_destroy(&_mutex);
}

void Poller::removeChannelInternal(SocketChannel *channel) {
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

void Poller::addEvent(int fd) {
    //设置为非阻塞
    //设置他exec退出，同时设置为非阻塞
    int ret = FileUtil::addFlag2Fd(fd, FD_CLOEXEC | O_NONBLOCK);
    exit_if(ret < 0, "addFlag2Fd FD_CLOEXEC error:%s", strerror(errno));

    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    /**
     * EPOLLIN 可读事件
     * EPOLLET 边缘触发
     * EPOLLONESHOT 一个事件只触发一次，因为一个事件只被触发一次且需要重置事件才能侦听下次是否发生
     * EPOLLRDHUP 对端调用了close等，关闭了连接
     */
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    event.data.fd = fd;
    ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &event);
    exit_if(ret < 0, "epoll_ctl add error:%s", strerror(errno));
}

void Poller::updateEvent(int fd, uint32_t events) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = events | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    int ret = epoll_ctl(_epfd, EPOLL_CTL_MOD, fd, &event);//注意是EPOLL_CTL_MOD修改
    exit_if(ret < 0, "epoll_ctl mod error:%s", strerror(errno));
}

void Poller::removeEvent(int fd) {
    int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
    exit_if(ret < 0, "epoll_ctl del error:%s", strerror(errno));
}

