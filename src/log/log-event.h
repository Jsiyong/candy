//
// Created by Jsiyong on 2021-02-03.
//

#ifndef CANDY_LOG_EVENT_H
#define CANDY_LOG_EVENT_H

#include <string>
#include <vector>
#include <sys/time.h>

/**
 * 日志级别
 */
enum LogLevel {
    TRACE = 0,
    INFO,
    WARN,
    ERROR,
    FATAL
};

/**
 * 日志事件
 */
struct LoggingEvent {
    template<typename ...Args>
    LoggingEvent(const std::string &logName, LogLevel level, const char *format, const std::string &file, int line,
                 const std::string &function, const Args &...args) {
        gettimeofday(&_time, NULL);//获取当前时间

        _level = level;
        _file = file;
        _line = line;
        _function = function;
        _threadId = pthread_self();
        _message = getMessage(format, args...);
        _logName = logName;
    }

    LogLevel getLevel() const;

    const std::string &getMessage() const;

    const std::string &getFile() const;

    int getLine() const;

    const std::string &getFunction() const;

    pthread_t getThreadId() const;

    const std::string &getLogName() const;

    const timeval &getTime() const;

private:

    /**
     * 转换函数，默认是自己类型
     * @tparam T
     * @param t
     * @return
     */
    template<typename T>
    static T convert(T t) {
        return t;
    }

    /**
     * 遇到std::string类型转换为str类型
     * @param str
     * @return
     */
    static const char *convert(const std::string &str);

    /**
     * 获取需要记录的日志信息
     * @tparam Args
     * @param format
     * @param args
     * @return
     */
    template<typename ...Args>
    std::string getMessage(const char *format, const Args &...args) {
        //计算大小
        int size = std::snprintf(NULL, 0, format, convert(args)...) + 1;
        std::vector<char> buf;
        buf.resize(size);
        std::snprintf(buf.data(), size, format, convert(args)...);
        return std::string(buf.begin(), buf.end());
    }

private:
    LogLevel _level;//日志等级
    std::string _message;//日志信息
    struct timeval _time;//记录日志的时间
    std::string _file;//是哪个文件
    int _line;//第几行
    std::string _function;//是哪个函数
    pthread_t _threadId;//线程id
    std::string _logName;//日志名称
};

#endif //CANDY_LOG_EVENT_H
