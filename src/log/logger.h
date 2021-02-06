//
// Created by jiangsiyong on 2020/12/15.
//

#ifndef CANDY_LOGGER_H
#define CANDY_LOGGER_H

#include <list>
#include "../util/singleton.h"
#include "log-appender.h"

/**************************************
 * 日志记录的宏
 **************************************/
#define jlog(level, format, ...)                                                                                             \
    do {                                                                                                                     \
        Logger *logger = LogManager::getInstance()->getLogger();                                                             \
        if (logger) {                                                                                                        \
            logger->log(LoggingEvent(logger->getName(), level, format, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__));    \
        }                                                                                                                    \
    } while (0)

#define trace(format, ...) jlog(LogLevel::TRACE, format, ##__VA_ARGS__)
#define info(format, ...) jlog(LogLevel::INFO, format, ##__VA_ARGS__)
#define warn(format, ...) jlog(LogLevel::WARN, format, ##__VA_ARGS__)
#define error(format, ...) jlog(LogLevel::ERROR, format, ##__VA_ARGS__)
#define fatal(format, ...) jlog(LogLevel::FATAL, format, ##__VA_ARGS__)

#define exit_if(b, ...)         \
    do {                        \
        if ((b)) {              \
            error(__VA_ARGS__); \
            exit(1);            \
        }                       \
    } while (0)

/**
 * 同步日志管理器
 */
struct Logger {

    Logger();

    /**
     * 添加一个输出地
     * @param appender 输出地，可以是文件，或者控制台
     */
    void addAppender(LogAppender *appender);

    /**
     * 设置日志级别，大于这个级别的日志才会输出
     * @param level
     */
    void setLevel(LogLevel level);

    const std::string &getName() const;

    /**
     * 写日志
     * @param level
     * @param event
     */
    void log(const LoggingEvent &event);

protected:
    virtual void write(const LoggingEvent &event);

    std::list<LogAppender *> _appenderList;//日志的输出地方
    std::string _name;//日志名称
    LogLevel _level;//日志级别: 大于这个级别的日志才会输出
};

/**
 * 异步日志器
 */
struct AsyncLogger : public Logger {

    AsyncLogger();

    ~AsyncLogger();

private:
    virtual void write(const LoggingEvent &event);

    static void *runLoop(void *param);

    std::list<LoggingEvent> _events;//存放的日志事件
    pthread_t _threadId;//线程id
    pthread_cond_t _cond;//条件变量
    pthread_mutex_t _mutex;//互斥量

    bool _exit = false;//是否退出线程
};


/**
 * 日志管理器，单例
 */
struct LogManager : public Singleton<LogManager> {

    /**
     * 设置日志器
     * @param logger
     */
    void setLogger(Logger *logger);

    /**
     * 获取日志器
     * @return
     */
    Logger *getLogger() const;


private:
    Logger *_logger = NULL;
};

#endif //CANDY_LOGGER_H
