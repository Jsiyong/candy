//
// Created by jiangsiyong on 2020/12/17.
//

#include "channel.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "../log/log.h"

#define MAX_BUFF_SIZE 1024
#define IP_SIZE 20

int Channel::fd() const {
    return _fd;
}

Channel::Channel(int fd, short type)
        : _fd(fd), _type(type), _state(State::ACTIVE) {
    //创建读写缓冲区
    _readBuff = (char *) malloc(MAX_BUFF_SIZE);
    memset(_readBuff, 0, MAX_BUFF_SIZE);
    _readPos = 0;

    //ip地址
    _host = (char *) malloc(IP_SIZE);
    memset(_host, 0, IP_SIZE);

    //设置ip地址和端口
    struct sockaddr_in raddr;
    socklen_t socklen = sizeof(raddr);
    memset(&raddr, 0, socklen);
    getpeername(_fd, (struct sockaddr *) &raddr, &socklen);
    inet_ntop(AF_INET, &raddr.sin_addr, _host, socklen);
    _port = ntohs(raddr.sin_port);

}

short Channel::getType() const {
    return _type;
}

Channel::~Channel() {
    //释放读写缓冲区
    free(_readBuff);
    free(_host);
    close(_fd);
}

void Channel::doRead() {

    while (true) {

        int size = ::read(_fd, _readBuff + _readPos, MAX_BUFF_SIZE);
        if (0 > size) {
            //假错
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                info("recv all data:%s", _readBuff);
                //读完了，重新设置缓冲区数据
                memset(_readBuff, 0, _readPos);
                _readPos = 0;
                break;
            }

            close(_fd);
            //通道的状态变为关闭
            _state = State::CLOSE;
            break;

        } else if (0 == size) {
            info("client disconnect.");

            //同时关闭fd，防止四次挥手的时候，客户端又发来一遍
            close(_fd);
            //通道的状态变为关闭
            _state = State::CLOSE;

            break;
        } else {
            _readPos += size;
        }
    }
}

short Channel::getState() const {
    return _state;
}

char *Channel::getHost() const {
    return _host;
}

int Channel::getPort() const {
    return _port;
}

void Channel::doWrite() {

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
    write(_fd, resp, strlen(resp));

}
