//
// Created by jiangsiyong on 2020/12/15.
//

#ifndef CANDY_LOG_H
#define CANDY_LOG_H


#define MAX_LOG_BUFF_SIZE 512

#define log(level, format, ...) Log::writeLog(level, format, __VA_ARGS__)

/**
 * 日志模块，在子进程中执行，通过管道进行进程间通信
 */
struct Log {
    enum Level {
        TRACE,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    /**
     * 0表示成功，-1表示失败
     * @return
     */
    static int init();

    static void readLogAndPrint();

    static void writeLog(Level level, const char *format, ...);

private:


};

#endif //CANDY_LOG_H
