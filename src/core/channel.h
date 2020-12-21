//
// Created by jiangsiyong on 2020/12/17.
//

#ifndef CANDY_CHANNEL_H
#define CANDY_CHANNEL_H

/**
 * 通道，包装了读写事件
 */
struct Channel {

    /**
     * 文件描述符类型
     */
    enum Type {
        LISTEN = 0,//服务端监听的文件描述符
        CONN//客户端连接的文件描述符
    };

    /**
     * Channel的状态
     */
    enum State {
        ACTIVE = 0,//活跃的状态
        CLOSE//已经关闭的状态了
    };

    Channel(int fd, short type);

    ~Channel();

    void doRead();

    void doWrite();

    int fd() const;

    short getType() const;

    short getState() const;

    char *getHost() const;

    int getPort() const;

private:

    int _fd;//对应的文件描述符

    int _port;//端口
    char *_host;//ip地址

    short _type;//文件描述符类型

    short _state;//通道的状态

    char *_readBuff;//读缓冲区
    int _readPos;//读指针位置
};

#endif //CANDY_CHANNEL_H
