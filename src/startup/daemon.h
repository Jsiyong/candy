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
    static bool start(const std::string &path = std::string());

private:
};

#endif //CANDY_DAEMON_H
