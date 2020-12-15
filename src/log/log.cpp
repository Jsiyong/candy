//
// Created by jiangsiyong on 2020/12/15.
//

#include "log.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

//管道的两端
static int pfd[2];

int Log::init() {

    if (pipe(pfd) < 0) {
        perror("pipe error");
        exit(1);
    }

    //fork一个进程去做日志
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error");
        exit(1);
    }
    if (0 == pid) {
        //子进程[读]，关闭写端
        close(pfd[1]);

    } else {
        //父进程[写]，关闭读端
        close(pfd[0]);
    }


    return 0;
}
