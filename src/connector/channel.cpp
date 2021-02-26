//
// Created by jiangsiyong on 2020/12/17.
//

#include "channel.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../log/logger.h"

constexpr int MaxIpBuffSize = 20;
constexpr size_t MaxReadBuffSize = 4096;

int SocketChannel::fd() const {
    return _fd;
}

SocketChannel::SocketChannel(int fd) : _fd(fd) {

    //设置ip地址和端口
    struct sockaddr_in raddr{};
    socklen_t socklen = sizeof(raddr);
    //获取地址
    getpeername(_fd, (struct sockaddr *) &raddr, &socklen);
    char iptmp[MaxIpBuffSize];
    inet_ntop(AF_INET, &raddr.sin_addr, iptmp, socklen);
    _host = iptmp;
    _port = ntohs(raddr.sin_port);
}

SocketChannel::~SocketChannel() {
    _close = true;
//    close(_fd);
}

size_t SocketChannel::read(std::string &dsts) {

    size_t allReadSize = 0;//读到的所有字节数
    dsts.reserve(MaxReadBuffSize);//先预出MaxReadBuffSize的空间
    while (true) {
        char buf[MaxReadBuffSize];

        ssize_t readSize = ::read(_fd, buf, MaxReadBuffSize);
        if (0 > readSize) {
            //假错
            if (errno == EINTR) {
                //被信号打断
                continue;
            }
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                //真的读错了
                _close = true;
                error("read error: %s", strerror(errno));
            }
            break;
        } else if (0 == readSize) {
            _close = true;
            //同时关闭fd，防止四次挥手的时候，客户端又发来一遍
//            close(_fd);
            //通道的状态变为关闭
            break;
        } else {
            allReadSize += readSize;//添加到总数去
            dsts.append(buf, buf + readSize);//添加到dsts中
        }
    }
    return allReadSize;
}

std::string SocketChannel::getHost() const {
    return _host;
}

int SocketChannel::getPort() const {
    return _port;
}

size_t SocketChannel::write(const std::string &srcs) {

    size_t allWritedSize = 0;//所有已经写出的字节大小

    size_t offset = 0;//偏移量
    size_t toWriteSize = srcs.size() - offset;//等待写出的字节大小
    while (toWriteSize > 0) {
        ssize_t writedSize = ::write(_fd, srcs.data() + offset, toWriteSize);
        if (0 > writedSize) {
            //假错
            if (errno == EINTR) {
                //被信号打断
                continue;
            }
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                //真的读错了
                _close = true;
            }
            break;
        } else {
            toWriteSize -= writedSize;//需要写出去的大小减少
            allWritedSize += writedSize;//所有已经写出的字节大小
            offset += writedSize;//偏移量也需要添加
        }
    }
    return allWritedSize;
}

bool SocketChannel::close() const {
    return _close;
}
