//
// Created by jiangsiyong on 2020/12/17.
//
#include "../log/logger.h"
#include "poller.h"
#include "../util/fileutil.h"
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#if !defined(__APPLE__)
#include <sys/eventfd.h>
#endif

#define MAX_EVENT 1024
#define IP_SIZE 20
#if !defined(__APPLE__)
constexpr static uint64_t MaxEventfdNum = UINT64_MAX - 1;//最大的eventfd数
#endif

Poller::Poller() {

    _executor = ThreadPoolExecutor::getInstance();

    _epfd = epoll_create(1);
    exit_if(_epfd < 0, "epoll_create error:%s", strerror(errno));

#if defined(__APPLE__)
    int fds[2];
    exit_if(pipe(fds) < 0, "pipe error: %s", strerror(errno));
    _wakeupRecvFd = fds[0];
    _wakeupSendFd = fds[1];
    FileUtil::addFlag2Fd(_wakeupRecvFd, FD_CLOEXEC);
    FileUtil::addFlag2Fd(_wakeupSendFd, FD_CLOEXEC);
#else
    _wakeupRecvFd = eventfd(0, EFD_SEMAPHORE);//信号量形式
    exit_if(_wakeupRecvFd < 0, "eventfd error: %s", strerror(errno));
    _wakeupSendFd = _wakeupRecvFd;
#endif
    //将唤醒 fd 加入反应堆中
    this->addEvent(_wakeupRecvFd, false);//采用阻塞的形式

    info("epoll create success!!");

    //初始化互斥锁
    pthread_mutex_init(&_mutex, NULL);
    pthread_mutex_init(&_exitMutex, NULL);
    pthread_cond_init(&_exitCond, NULL);

    //创建反应堆线程，也就是把当前这个对象加入线程池中，记得最后才加上
    _executor->submit(this);
}

void Poller::attach(int fd) {
    //找到一块空的地方放
    //封装为socketProcessor
    auto socketProcessor = new SocketProcessor(fd);
    //解析完协议之后，就需要开启epoll的写事件,
    socketProcessor->setOnAfterReadCompletedRequest([=]() {
        this->updateEvent(fd, EPOLLOUT);
    });
    //解析到未完整的协议，就重新开启epoll读事件
    socketProcessor->setOnAfterReadUnCompletedRequest([=]() {
        this->updateEvent(fd, EPOLLIN);
    });
    //写完完就开始重新触发读事件
    socketProcessor->setOnAfterWriteCompletedResponse([=]() {
        this->updateEvent(fd, EPOLLIN);
    });
    //写不完就继续写
    socketProcessor->setOnAfterWriteUnCompletedResponse([=]() {
        this->updateEvent(fd, EPOLLOUT);
    });
    //socket关闭了，触发epoll_wait释放信息
    socketProcessor->setOnAfterSocketChannelClosed([=]() {
        this->updateEvent(fd, EPOLLRDHUP | EPOLLHUP | EPOLLERR);
    });

    //需要加锁
    pthread_mutex_lock(&_mutex);
    if (!_socketProcessors.emplace(fd, socketProcessor).second) {
        error("socketProcessors emplace error");
    }
    pthread_mutex_unlock(&_mutex);

    info("add new clieant[%d], map size: %d", fd, _socketProcessors.size());

    //添加事件
    this->addEvent(fd);
}

void Poller::run() {

    struct epoll_event events[MAX_EVENT];

    while (!this->_exit) {

        int eventCount = epoll_wait(this->_epfd, events, MAX_EVENT, -1);
        //epoll_wait有可能被信号打断，所以还要判断errno是不是为EINTR
        exit_if(eventCount == -1 && errno != EINTR, "epoll_wait error:%s", strerror(errno));
        //处理每一个事件
        for (int i = 0; i < eventCount; ++i) {
            if (events[i].data.fd == _wakeupRecvFd) {
                //如果是唤醒 fd，则说明可能是结束进程了，那就直接退出
                trace("get eventfd event!!");
                break;
            }

            pthread_mutex_lock(&this->_mutex);
            auto item = this->_socketProcessors.find(events[i].data.fd);
#if 0
            if (item == this->_socketProcessors.end()) {
                error("epoll wait: _socketProcessors error!!");
                continue;//找不到这个socket，可能是EPOLLRDHUP没有和EPOLLIN一起触发，而是先触发了EPOLLRDHUP后又触发了EPOLLIN
            }
#endif
            SocketProcessor *pSocketProcessor = item->second;
            pthread_mutex_unlock(&this->_mutex);

            //先判断是不是有异常情况，是不是断开了连接等等，异常了直接移除这个socket，否则才提交线程池处理
            if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {//客户端断开了连接

                int clifd = pSocketProcessor->getChannel()->fd();

                //记得删掉指针
                delete pSocketProcessor;

                pthread_mutex_lock(&this->_mutex);
                this->_socketProcessors.erase(clifd);//移除掉这个key对应的内容
                int size = this->_socketProcessors.size();
                pthread_mutex_unlock(&this->_mutex);

                //注意，close事件要放到erase之后，避免close之后的瞬间客户端又用这个fd建立起新的连接，并且发生数据
                //如果是这样的话，那时候如果还没erase,但是客户端却发来了连接建立的请求，而且发送了数据来。但是来了之后可能这个线程又erase了
                //就会奔溃
                this->removeEvent(clifd);

                trace("client[%d] disconnected,current client size[%d]..", clifd, size);
            } else if (events[i].events & (EPOLLIN | EPOLLOUT)) {//可读事件和可写事件都进入这个地方
                //提交任务
                this->_executor->submit(pSocketProcessor);
            }
        }
    }
    uint64_t num = 0;//每次智能读出1
    int ret = ::read(_wakeupRecvFd, &num, sizeof(uint64_t));//回应写端
    trace("eventfd read[%d byte/%lld] ok!!", ret, (long long) num);

    pthread_mutex_lock(&_exitMutex);
    _loopExited = true;
    pthread_cond_signal(&_exitCond);
    pthread_mutex_unlock(&_exitMutex);
}

Poller::~Poller() {
    _exit = true;
#if defined(__APPLE__)
    uint64_t one = 1;
    int ret = ::write(_wakeupSendFd, &one, sizeof(one));
#else
    int ret = ::write(_wakeupSendFd, &MaxEventfdNum, sizeof(uint64_t));
#endif
    exit_if(ret < 0, "error: %s", strerror(errno));
    trace("poll exiting...");
    // 等待 run() 循环真正退出（Linux 原先用 eventfd 写满阻塞，macOS pipe 无法等价模拟）
    pthread_mutex_lock(&_exitMutex);
    while (!_loopExited) {
        pthread_cond_wait(&_exitCond, &_exitMutex);
    }
    pthread_mutex_unlock(&_exitMutex);
    trace("poll exit ok...");
    pthread_mutex_destroy(&_mutex);
    pthread_mutex_destroy(&_exitMutex);
    pthread_cond_destroy(&_exitCond);
    if (_wakeupRecvFd >= 0) {
        close(_wakeupRecvFd);
    }
    if (_wakeupSendFd >= 0 && _wakeupSendFd != _wakeupRecvFd) {
        close(_wakeupSendFd);
    }
    if (_epfd >= 0) {
        close(_epfd);
    }
}

void Poller::addEvent(int fd, bool nonblock) {
    int ret = 0;
    if (nonblock) {
        //设置为非阻塞
        //设置他exec退出，同时设置为非阻塞
        ret = FileUtil::addFlag2Fd(fd, FD_CLOEXEC | O_NONBLOCK);
        exit_if(ret < 0, "addFlag2Fd FD_CLOEXEC error:%s", strerror(errno));
    }

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
    struct epoll_event event{};
    event.data.fd = fd;
    event.events = events | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    int ret = epoll_ctl(_epfd, EPOLL_CTL_MOD, fd, &event);//注意是EPOLL_CTL_MOD修改
    exit_if(ret < 0, "epoll_ctl mod[events: %d] error:%s", events, strerror(errno));
}

void Poller::removeEvent(int fd) {
    int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
    exit_if(ret < 0, "epoll_ctl del error:%s", strerror(errno));
    //关了之后顺便把fd也关闭了
    close(fd);
}

