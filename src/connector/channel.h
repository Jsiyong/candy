//
// Created by jiangsiyong on 2020/12/17.
//

#ifndef CANDY_CHANNEL_H
#define CANDY_CHANNEL_H

#include <string>
#include <vector>
#include <functional>

/**
 * socket的状态
 */
enum class SocketStatus {
    OPEN_READ,
    OPEN_WRITE,
    STOP,
    TIMEOUT,
    DISCONNECT,
    ERROR,
    CLOSE_NOW
};

/**
 * 通道，包装了读写事件
 */
struct SocketChannel {

    explicit SocketChannel(int fd);

    ~SocketChannel();

    /**
     * 将数据读到dsts中
     * @param dsts
     * @param offset -1表示从末尾开始添加，0表示从头开始添加，其他表示从特定位置开始添加
     * @return
     */
    long long read(std::vector<char> &dsts, long long offset = -1);

    /**
     * 将数据写出去
     * @param srcs 源数据
     * @param offset 写的数据的开始地方
     * @return
     */
    long long write(const std::vector<char> &srcs, long long offset = 0);

    int fd() const;

    std::string getHost() const;

    int getPort() const;

    bool close() const;

private:

    int _fd;//对应的文件描述符

    int _port;//端口
    std::string _host;//ip地址

    bool _close = false;
};

#endif //CANDY_CHANNEL_H
