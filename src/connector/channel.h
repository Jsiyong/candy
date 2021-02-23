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
     * @param 0表示从头开始添加，其他表示从特定位置开始添加
     * @return
     */
    size_t read(std::vector<char> &dsts, size_t offset = 0);

    /**
     * 将数据写出去
     * @param srcs 源数据
     * @param offset 写的数据的开始地方
     * @return
     */
    size_t write(const std::vector<char> &srcs, size_t offset = 0);

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
