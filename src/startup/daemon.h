//
// Created by jiangsiyong on 2020/12/15.
//

#ifndef CANDY_DAEMON_H
#define CANDY_DAEMON_H

#include <string>

/*
 * 作为守护进程启动
 */
struct Daemon {

    /**
     * 作为守护进程启动
     * @return
     */
    static bool start(const std::string &path);

    /**
     * 停止该守护进程
     * @return
     */
    static bool stop(const std::string &path);

private:

    /**
     * 从文件中获取pid
     * @param path
     * @return
     */
    static pid_t getPidFromFile(const std::string &path);
};

#endif //CANDY_DAEMON_H
