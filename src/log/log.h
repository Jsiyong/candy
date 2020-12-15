//
// Created by jiangsiyong on 2020/12/15.
//

#ifndef CANDY_LOG_H
#define CANDY_LOG_H

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

    static int init();

};

#endif //CANDY_LOG_H
