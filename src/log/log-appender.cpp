//
// Created by Jsiyong on 2021-02-03.
//

#include "./log-appender.h"
#include <iostream>

void ConsoleLogAppender::append(const LoggingEvent &event) {
    std::string str = _layout->format(event);
    std::cout << str;
    std::cout.flush();//刷新缓冲区
}

ConsoleLogAppender::ConsoleLogAppender() {
    _layout = new LogLayout();
}

void FileLogAppender::append(const LoggingEvent &event) {

}

void LogAppender::setLevel(LogLevel level) {
    _level = level;
}

LogAppender::~LogAppender() {

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
