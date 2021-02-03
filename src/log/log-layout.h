//
// Created by Jsiyong on 2021-02-03.
//

#ifndef CANDY_LOG_LAYOUT_H
#define CANDY_LOG_LAYOUT_H

#include <map>
#include "log-event.h"

enum LogColor {
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    PURPLE,//紫色
    DARK_GREEN,//深绿色
    WHITE//白色
};

/**
 * 日志格式化的相关信息
 */
struct LogFormatInfo {
    //字体颜色
    LogColor fontColor;

    //日志级别的字符串
    std::string levelStr;

    LogFormatInfo();

    LogFormatInfo(LogColor fontColor, const std::string &levelStr);
};

/**
 * 日志的布局、格式
 */
struct LogLayout {

    LogLayout();

    /**
     * 格式化日志信息为字符串
     * @param event
     * @return
     */
    virtual std::string format(const LoggingEvent &event);

protected:

    /**
     * 格式化时间
     * @param time
     * @return
     */
    std::string formatTime(struct timeval time);

    /**
     * 格式化文件名
     * @param file
     * @return
     */
    std::string formatFile(const std::string &file);

    /**
     * 格式化函数名
     * @param function
     * @return
     */
    std::string formatFunction(const std::string &function);

    //key: 日志级别
    std::map<int, LogFormatInfo> _formatTable;

};


#endif //CANDY_LOG_LAYOUT_H
