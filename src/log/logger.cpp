//
// Created by jiangsiyong on 2020/12/15.
//

#include "logger.h"
#include "../conf/servconf.h"
#include "../util/threadpool.h"
#include "../util/guard.h"
#include <unistd.h>

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
    //信号量初始化
    sem_init(&_sem, 0, 0);

    //加入线程池管理
    ThreadPoolExecutor::getInstance()->submit(this);
}

void AsyncLogger::run() {

    MutexLocker locker(&this->_mutex);
    //日志线程还没有退出的时候
    while (!this->_exit) {
        do {
            //如果任务数为空，那么就跳过，然后去等待
            if (this->_events.empty()) {
                break;
            }
            LoggingEvent event = this->_events.front();
            this->_events.pop_front();
            locker.unlock();
            //线程开始写日志
            //一个一个写
            for (LogAppender *appender : this->_appenderList) {
                appender->doAppend(event);
            }

            locker.relock();//写完重新获取锁
        } while (true);

        pthread_cond_wait(&this->_cond, &this->_mutex);//令进程等待在条件变量上
    }
    //退出之前，发布一个信号量给析构函数
    sem_post(&_sem);
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

    sem_wait(&_sem);//等待线程释放
    usleep(10);
    //销毁信号量
    sem_destroy(&_sem);
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
