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

#define IP_SIZE 20

#define MAX_BUFF_SIZE 1024

int Channel::fd() const {
    return _fd;
}

Channel::Channel(int fd) : _fd(fd) {

    //ip地址

    //设置ip地址和端口
    struct sockaddr_in raddr;
    socklen_t socklen = sizeof(raddr);
    memset(&raddr, 0, socklen);
    getpeername(_fd, (struct sockaddr *) &raddr, &socklen);
    char iptmp[IP_SIZE];
    inet_ntop(AF_INET, &raddr.sin_addr, iptmp, socklen);
    _host = iptmp;
    _port = ntohs(raddr.sin_port);
}

Channel::~Channel() {
    close(_fd);
}

void Channel::read() {

    char buf[MAX_BUFF_SIZE];
    while (true) {

        int size = ::read(_fd, buf, MAX_BUFF_SIZE);
        if (0 > size) {
            //假错
            if (errno == EINTR) {
                //被信号打断
                continue;
            }
            //读完了
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                error("read complete");
//                close(_fd);
                //通道的状态变为关闭
//                _state = State::CLOSE;
            }
            break;
        } else if (0 == size) {
            error("client disconnect.");
//            info("recv all data:%s", _readBuff.data());

            //同时关闭fd，防止四次挥手的时候，客户端又发来一遍
//            close(_fd);
            //通道的状态变为关闭

            break;
        } else {
            _readBuff.insert(_readBuff.end(), buf, buf + size);
//            trace("recv size[%d],buff size[%d]", size, _readBuff.size());
        }
    }
}

short Channel::getState() const {
    return _state;
}

std::string Channel::getHost() const {
    return _host;
}

int Channel::getPort() const {
    return _port;
}

void Channel::write() {

    char resp[MAX_BUFF_SIZE] = {0};
    char *p = resp;
    const char *header = "HTTP/1.1 200 OK\r\n";
    const char *linefmt = "Content-Type:%s\r\nContent-Length:%d\r\n";
    //body
    const char *body = "{\"title\":\"hello\",\"body\":\"world\"}";
    char line[MAX_BUFF_SIZE] = {0};
    sprintf(line, linefmt, "application/json; charset=UTF-8", strlen(body));

    //头部
    strcat(p, header);
    p += strlen(header);

    //键值对
    strcat(p, line);
    p += strlen(line);

    //换行
    strcat(p, "\r\n");
    p += strlen("\r\n");


    strcat(p, body);
    trace("response data:%s", resp);
    trace("response ok!!");
    ::write(_fd, resp, strlen(resp));

}

std::vector<char> &Channel::getReadBuff() {
    return _readBuff;
}

bool Channel::finishWrite() {
    return true;
}
