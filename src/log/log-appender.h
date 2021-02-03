//
// Created by Jsiyong on 2021-02-03.
//

#ifndef CANDY_LOG_APPENDER_H
#define CANDY_LOG_APPENDER_H

#include <list>
#include "log-event.h"
#include "log-layout.h"

/**
 * 日志输出到不同的地方
 */
struct LogAppender {

    ~LogAppender();

    void setLevel(LogLevel level);

    /**
     * 写日志的方法
     * @param level 级别，大于这个级别的才输出
     * @param logEvent 日志信息
     */
    void doAppend(const LoggingEvent &event);

    void setLayout(LogLayout *layout);

protected:
    /**
     * 不同实现的添加日志
     */
    virtual void append(const LoggingEvent &event) = 0;

    LogLevel _level;
    std::list<LoggingEvent *> _events;
    LogLayout *_layout = NULL;
};

/**
 * 输出日志至控制台
 */
struct ConsoleLogAppender : public LogAppender {
    ConsoleLogAppender();

    void append(const LoggingEvent &event) override;
};

/**
 * 输出日志至文件
 */
struct FileLogAppender : public LogAppender {
    void append(const LoggingEvent &event) override;
};

#endif //CANDY_LOG_APPENDER_H
