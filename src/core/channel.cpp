//
// Created by jiangsiyong on 2020/12/17.
//

#include "channel.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "../log/log.h"

#define MAX_BUFF_SIZE 1024

int Channel::fd() const {
    return _fd;
}

Channel::Channel(int fd, short type)
        : _fd(fd), _type(type), _state(State::ACTIVE) {
    //创建读写缓冲区
    _readBuff = (char *) malloc(MAX_BUFF_SIZE);
    memset(_readBuff, 0, MAX_BUFF_SIZE);
    _readPos = 0;

}

short Channel::getType() const {
    return _type;
}

Channel::~Channel() {
    //释放读写缓冲区
    free(_readBuff);
    close(_fd);
}

#include <sys/socket.h>

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
            info("client disconnect.\nrecv data:%s", _readBuff);
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
