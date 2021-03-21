//
// Created by Jsiyong on 2021-02-03.
//

#include "./logappender.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <iostream>
#include "string.h"
#include "../conf/servconf.h"

void ConsoleLogAppender::append(const LoggingEvent &event) {
    std::string str = _layout->format(event);
    std::cout << str;
    std::cout.flush();//刷新缓冲区
}

ConsoleLogAppender::ConsoleLogAppender() {
    this->setLayout(new LogLayout());
}

ConsoleLogAppender::~ConsoleLogAppender() {
    delete _layout;
}

void FileLogAppender::append(const LoggingEvent &event) {
    if (_file.empty()) {
        this->setFile(serverConf.getServerDir() + "/candy.log");
    }

    std::string str = _layout->format(event);
    ssize_t writed = 0;
    while (str.size() - writed > 0) {
        int len = write(_fd, str.c_str() + writed, str.size() - writed);
        if (len < 0) {
            if (errno == EAGAIN) {
                continue;//假错，被信号打断
            }
            fprintf(stderr, "write error: %s", strerror(errno));
            break;//其他情况是真错
        }
        writed += len;
    }
}

void FileLogAppender::setFile(const std::string &file) {
    this->_file = file;
    _fd = open(file.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
    if (_fd < 0) {
        //文件打开错误
        fprintf(stderr, "open error: %s", strerror(errno));
    }
}

FileLogAppender::FileLogAppender() {
    this->setLayout(new FileLogLayout());
}

FileLogAppender::~FileLogAppender() {
    delete _layout;
    close(_fd);
}

void LogAppender::setLevel(LogLevel level) {
    _level = level;
}

void LogAppender::doAppend(const LoggingEvent &event) {
    if (event.getLevel() < _level) {
        return;
    }
    append(event);
}

void LogAppender::setLayout(LogLayout *layout) {
    _layout = layout;
}
