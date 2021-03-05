//
// Created by jiangsiyong on 2020/12/15.
//

#include "logger.h"
#include "../conf/servconf.h"
#include "../util/threadpool.h"

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
    _name = serverConf.getServerName();
}

const std::string &Logger::getName() const {
    return _name;
}

Logger::~Logger() {

}

AsyncLogger::AsyncLogger() {
    //初始化互斥锁
    pthread_mutex_init(&_mutex, NULL);
    pthread_cond_init(&_cond, NULL);
    //加入线程池管理
    ThreadPoolExecutor::getInstance()->submit(this);
}

void AsyncLogger::run() {
    //日志线程还没有退出的时候
    while (!this->_exit) {
        std::list<LoggingEvent> events;

        pthread_mutex_lock(&this->_mutex);//拿到互斥锁，进入临界区
        //为空，就一直等，防止信号打断wait
        while (this->_events.empty()) {
            pthread_cond_wait(&this->_cond, &this->_mutex);//令进程等待在条件变量上
            if (this->_exit) {
                pthread_exit(NULL);
            }
        }

        //消费队列所有的数据
        while (!this->_events.empty()) {
            events.push_back(this->_events.front());
            this->_events.pop_front();
        }
        pthread_mutex_unlock(&this->_mutex); //释放互斥锁

        //线程开始写日志
        for (const LoggingEvent &event:events) {
            //一个一个写
            for (LogAppender *appender : this->_appenderList) {
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
    pthread_cond_broadcast(&_cond);
}

AsyncLogger::~AsyncLogger() {
    //让线程正常退出，让主线程去收拾
    _exit = true;
    pthread_cond_broadcast(&_cond);
    //销毁互斥锁
    pthread_mutex_destroy(&_mutex);
    pthread_cond_destroy(&_cond);
}

void LogManager::setLogger(Logger *logger) {
    _logger = logger;
}

Logger *LogManager::getLogger() const {
    return _logger;
}
