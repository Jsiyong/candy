//
// Created by jiangsiyong on 2020/12/17.
//

#include "channel.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "../log/logger.h"

constexpr int maxIpBuffSize = 20;
constexpr size_t maxReadBuffSize = 4096;

int SocketChannel::fd() const {
    return _fd;
}

SocketChannel::SocketChannel(int fd) : _fd(fd) {

    //设置ip地址和端口
    struct sockaddr_in raddr{};
    socklen_t socklen = sizeof(raddr);
    //获取地址
    getpeername(_fd, (struct sockaddr *) &raddr, &socklen);
    char iptmp[maxIpBuffSize];
    inet_ntop(AF_INET, &raddr.sin_addr, iptmp, socklen);
    _host = iptmp;
    _port = ntohs(raddr.sin_port);
}

SocketChannel::~SocketChannel() {
    _close = true;
//    close(_fd);
}

size_t SocketChannel::read(std::vector<char> &dsts, size_t offset) {
    size_t newOffset = offset;

    if (newOffset > dsts.size()) {
        error("offset error! dists size: %d, offset: %d", dsts.size(), offset);
        return 0;
    }

    size_t allReadSize = 0;//读到的所有字节数
    bool alloc = false;//是否重新分配过空间

    while (true) {
        //判断是不是需要创建新的空间
        if (newOffset + maxReadBuffSize > dsts.size()) {
            dsts.resize(newOffset + maxReadBuffSize);//新的大小
            alloc = true;
        }

        int readSize = ::read(_fd, dsts.data() + newOffset, maxReadBuffSize);
        if (0 > readSize) {
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
        } else if (0 == readSize) {
            _close = true;
            //同时关闭fd，防止四次挥手的时候，客户端又发来一遍
//            close(_fd);
            //通道的状态变为关闭
            break;
        } else {
            allReadSize += readSize;//添加到总数去
            newOffset += readSize;//插入的开始位置也发生偏移
        }
    }

    //判断是不是需要修改数组的大小，剪掉无用的内容
    if (alloc) {
        dsts.erase(dsts.begin() + offset + allReadSize, dsts.end());
    }
    return allReadSize;
}

std::string SocketChannel::getHost() const {
    return _host;
}

int SocketChannel::getPort() const {
    return _port;
}

size_t SocketChannel::write(const std::vector<char> &srcs, size_t offset) {
    //读指针的偏移大小不能大于数组的大小
    if (offset > srcs.size()) {
        error("offset error! srcs size: %d, offset: %d", srcs.size(), offset);
        return 0;
    }

    size_t allWritedSize = 0;//所有已经写出的字节大小
    size_t toWriteSize = srcs.size() - offset;//等待写出的字节大小

    while (toWriteSize > 0) {
        size_t writedSize = ::write(_fd, srcs.data() + offset, toWriteSize);
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
