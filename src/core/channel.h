//
// Created by jiangsiyong on 2020/12/17.
//

#ifndef CANDY_CHANNEL_H
#define CANDY_CHANNEL_H

#include "http.h"
#include <string>
#include <vector>
#include <functional>

/**
 * 通道，包装了读写事件
 */
struct Channel {

    explicit Channel(int fd);

    ~Channel();

    void read();

    void write();

    int fd() const;

    short getState() const;

    std::string getHost() const;

    int getPort() const;

    HttpRequest *getHttpRequest() const;

    std::vector<char> &getReadBuff();

    bool finishWrite();

private:

    int _fd;//对应的文件描述符

    int _port;//端口
    std::string _host;//ip地址

    short _state;//通道的状态

    std::vector<char> _readBuff;//读缓冲区
};

#endif //CANDY_CHANNEL_H
