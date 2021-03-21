//
// Created by Jsiyong on 2021-02-03.
//

#ifndef CANDY_LOGAPPENDER_H
#define CANDY_LOGAPPENDER_H

#include <list>
#include "logevent.h"
#include "loglayout.h"

/**
 * 日志输出到不同的地方
 */
struct LogAppender {

    virtual ~LogAppender() = default;

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

    ~ConsoleLogAppender() override;

    void append(const LoggingEvent &event) override;
};

/**
 * 输出日志至文件
 */
struct FileLogAppender : public LogAppender {
    FileLogAppender();

    ~FileLogAppender() override;

    void append(const LoggingEvent &event) override;

    /**
     * 设置日志文件的路径
     * @param path
     */
    void setPath(const std::string &path);

private:
    std::string _file;//文件路径
    int _fd = 0;//日志文件的fd
};

#endif //CANDY_LOGAPPENDER_H
