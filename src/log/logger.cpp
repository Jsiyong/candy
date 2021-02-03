//
// Created by jiangsiyong on 2020/12/15.
//

#include "logger.h"
#include <unistd.h>

//代表程序的路径，可以通过访问这个路径来获取
#define CURRENT_EXE_PATH "/proc/self/exe"
#define PATH_MAX 1024

void Logger::addAppender(LogAppender *appender) {
    _appenderList.push_back(appender);
}

void Logger::setLevel(LogLevel level) {
    _level = level;
}

void Logger::log(const LoggingEvent &event) {
    //若日志级别小于设定的级别，就不输出
    if (event.getLevel() < _level) {
        return;
    }
    write(event);
}

void Logger::write(const LoggingEvent &event) {
    //分别写日志
    for (LogAppender *appender : _appenderList) {
        appender->doAppend(event);
    }
}

Logger::Logger() {
    char buffer[PATH_MAX] = {0};
    readlink(CURRENT_EXE_PATH, buffer, PATH_MAX);
    std::string exePath = buffer;
    _name = exePath.substr(exePath.rfind('/') + 1);
}

const std::string &Logger::getName() const {
    return _name;
}

AsyncLogger::AsyncLogger() {
    pthread_create(&_threadId, NULL, AsyncLogger::runLoop, this);
}

void *AsyncLogger::runLoop(void *param) {
    AsyncLogger *logger = (AsyncLogger *) param;
    //日志线程还没有退出的时候
    while (!logger->_exit) {
        std::list<LoggingEvent> events;

        pthread_mutex_lock(&logger->_mutex);//拿到互斥锁，进入临界区
        //为空，就一直等，防止信号打断wait
        while (logger->_events.empty()) {
            pthread_cond_wait(&logger->_cond, &logger->_mutex);//令进程等待在条件变量上
        }

        //消费队列所有的数据
        while (!logger->_events.empty()) {
            events.push_back(logger->_events.front());
            logger->_events.pop_front();
        }
        pthread_mutex_unlock(&logger->_mutex); //释放互斥锁

        //线程开始写日志
        for (const LoggingEvent &event:events) {
            //一个一个写
            for (LogAppender *appender : logger->_appenderList) {
                appender->doAppend(event);
            }
        }
    }
}

void AsyncLogger::write(const LoggingEvent &event) {
    //把它放进队列里
    pthread_mutex_lock(&_mutex);//拿到互斥锁
    _events.push_back(event);
    pthread_mutex_unlock(&_mutex); //释放互斥锁
}

void LogManager::setLogger(Logger *logger) {
    _logger = logger;
}

Logger *LogManager::getLogger() const {
    return _logger;
}
