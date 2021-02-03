//
// Created by Jsiyong on 2021-02-03.
//

#include "log-event.h"
#include <sys/time.h>

LogLevel LoggingEvent::getLevel() const {
    return _level;
}

const std::string &LoggingEvent::getMessage() const {
    return _message;
}

const std::string &LoggingEvent::getFile() const {
    return _file;
}

int LoggingEvent::getLine() const {
    return _line;
}

const std::string &LoggingEvent::getFunction() const {
    return _function;
}

pthread_t LoggingEvent::getThreadId() const {
    return _threadId;
}

const char *LoggingEvent::convert(const std::string &str) {
    return str.c_str();
}

const std::string &LoggingEvent::getLogName() const {
    return _logName;
}

const timeval &LoggingEvent::getTime() const {
    return _time;
}
