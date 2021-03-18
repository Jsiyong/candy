//
// Created by jiangsiyong on 2021/3/18.
//

#include "daemon.h"
#include <signal.h>
#include <unistd.h>
#include "../util/fileutil.h"
#include "../log/logger.h"

bool Daemon::start(const std::string &path) {
    //首先判断进程相关的文件是不是已经开启了
    //从指定的文件中获取进程号
    pid_t pid = 0;
    std::string buf;
    FileUtil::readFile(path, buf);
    pid = std::atoi(buf.c_str());
    if (pid > 0) {
        //判断是不是进程依旧存在，存在的话就不能再启动了
        if (kill(pid, 0) == 0) {
            error("daemon exists");
            return false;
        }
    }
    //如果父进程是1号进程，那么已经是守护进程了，不用启动了
    if (getppid() == 1) {
        error("already a daemon");
        return false;
    }
    pid = fork();//fork一个进程
    if (pid < 0) {
        error("fork error: %s", strerror(errno));
        return false;
    }
    if (pid > 0) {
        //父进程退出，让子进程被1号进程托管
        exit(0);
    }
    //子进程成为会话的组长，新的进程组的组长，进程组的ID和会话的ID都被设置为pid
    pid = setsid();
    if (pid < 0) {
        error("setsid error: %s", strerror(errno));
        return false;
    }
    //将当前的进程id写入文件中
    //读写不能执行，且只有文件的所有者能操作
    if (!FileUtil::writeFile(path, std::to_string(pid), 0600)) {
        return false;
    }
    //将输入输出重定向到空设备上
    int fd = open("/dev/null", 0);
    if (fd < 0) {
        return false;
    }
    close(0);
    close(1);
    dup2(fd, 0);
    dup2(fd, 1);
    close(fd);

    //进程结束的时候自动删除这个文件
    static std::string pidFile = path;
    atexit([] { unlink(pidFile.c_str()); });

    return true;
}
