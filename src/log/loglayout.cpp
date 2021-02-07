//
// Created by Jsiyong on 2021-02-03.
//

#include "loglayout.h"

#define TIME_STR_LEN 128

#define CLEAR_COLOR "\033[0m"

static std::map<int, const char *> LOG_CONSOLE_COLOR_MAP = {
        {LogColor::BLACK,      "\033[30m"},
        {LogColor::RED,        "\033[31m"},
        {LogColor::GREEN,      "\033[32m"},
        {LogColor::YELLOW,     "\033[33m"},
        {LogColor::BLUE,       "\033[34m"},
        {LogColor::PURPLE,     "\033[35m"},
        {LogColor::DARK_GREEN, "\033[36m"},
        {LogColor::WHITE,      "\033[37m"}
};

LogLayout::LogLayout() {
    _formatTable = {
            {LogLevel::TRACE, {LogColor::BLUE,       "TRACE"}},
            {LogLevel::INFO,  {LogColor::GREEN,      "INFO "}},
            {LogLevel::WARN,  {LogColor::YELLOW,     "WARN "}},
            {LogLevel::ERROR, {LogColor::RED,        "ERROR"}},
            {LogLevel::FATAL, {LogColor::DARK_GREEN, "FATAL"}}
    };
}

LogFormatInfo::LogFormatInfo(LogColor fontColor, const std::string &levelStr) : fontColor(fontColor),
                                                                                levelStr(levelStr) {}

LogFormatInfo::LogFormatInfo() {}

std::string LogLayout::format(const LoggingEvent &event) {

    std::string fmt;
    LogFormatInfo formatInfo = _formatTable[event.getLevel()];
    //1:颜色
    fmt.append(LOG_CONSOLE_COLOR_MAP[formatInfo.fontColor]);
    //2:时间
    fmt.append(formatTime(event.getTime()) + " ");
    //3:级别
    fmt.append(formatInfo.levelStr + " ");
    //4:日志名
    fmt.append(event.getLogName());
    //5:线程号
    fmt.append("[" + std::to_string(event.getThreadId()) + "] ");
    //6:文件名:行号
    fmt.append(formatFile(event.getFile()) + ":" + std::to_string(event.getLine()) + " ");
    //7:方法名
    fmt.append(formatFunction(event.getFunction()) + " >> ");
    //8:信息
    fmt.append(event.getMessage());
    //9:结束
    fmt.append(CLEAR_COLOR);
    //10:加上换行
    fmt.append("\n");

    return fmt;
}

std::string LogLayout::formatTime(struct timeval time) {
    time_t sec_tmp = time.tv_sec;
    struct tm tm;
    localtime_r(&sec_tmp, &tm);
    char buf[TIME_STR_LEN] = {0};
    snprintf(buf, sizeof(buf), "%d-%02d-%02d %02d:%02d:%02d.%03d",
             1900 + tm.tm_year,
             1 + tm.tm_mon,
             tm.tm_mday,
             tm.tm_hour,
             tm.tm_min,
             tm.tm_sec,
             (int) (time.tv_usec / 1000));
    return buf;
}

std::string LogLayout::formatFile(const std::string &file) {
    return file.substr(file.rfind('/') + 1);
}

std::string LogLayout::formatFunction(const std::string &function) {
    return function.substr(function.rfind(':') + 1);
}

