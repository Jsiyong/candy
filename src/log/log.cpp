//
// Created by jiangsiyong on 2020/12/15.
//

#include "log.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define MAX_LOG_FMT_SIZE 512
#define MAX_LOG_BUFF_SIZE 4096

//管道的两端
static int pfd[2];

int Log::init() {

    if (pipe(pfd) < 0) {
        perror("pipe error");
        return -1;
    }

    //fork一个进程去做日志
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error");
        return -1;
    }
    if (0 == pid) {
        //子进程[读]，关闭写端
        close(pfd[1]);
        readLogAndPrint();
        //正常退出
        exit(0);
    } else {
        //父进程[写]，关闭读端
        close(pfd[0]);
        //等待给子进程收拾
    }

    return 0;
}

void Log::readLogAndPrint() {
    char buff[MAX_LOG_BUFF_SIZE] = {0};
    while (true) {
        //读管道的数据
        int len = ::read(pfd[0], buff, MAX_LOG_BUFF_SIZE);

        if (len < 0) {
            perror("read error");
            break;
        } else if (len == 0) {
            //读到了0，可能父进程的文件描述符已经关闭了，就会一直返回0
            printf("write pipe close\n");
            break;
        } else {
            printf("%s", buff);
            memset(buff, 0, len);
        }
    }
}

void Log::writeLog(Level level, const char *format, ...) {

    char fmtBuf[MAX_LOG_FMT_SIZE] = {0};
    char *p = fmtBuf;
    switch (level) {
        case TRACE:
            strcat(fmtBuf, "\033[43m\033[37m[trace]\033[0m\033[33m");
            p += strlen("\033[43m\033[37m[trace]\033[0m\033[33m");
            break;
        case INFO:
            strcat(fmtBuf, "\033[42m\033[37m[info]\033[0m\033[32m");
            p += strlen("\033[42m\033[37m[info]\033[0m\033[32m");
            break;
        case WARN:
            strcat(fmtBuf, "\033[44m\033[37m[warn]\033[0m\033[34m");
            p += strlen("\033[44m\033[37m[warn]\033[0m\033[34m");
            break;
        case ERROR:
            strcat(fmtBuf, "\033[41m\033[37m[error]\033[0m\033[31m");
            p += strlen("\033[41m\033[37m[error]\033[0m\033[31m");
            break;
        case FATAL:
            strcat(fmtBuf, "\033[41m\033[37m\033[1m[fatal]\033[0m\033[31m\033[1m");
            p += strlen("\033[41m\033[37m\033[1m[fatal]\033[0m\033[31m\033[1m");
            break;
        default:
            break;
    }
    strcat(p, format);
    if (p != fmtBuf) {
        p += strlen("\033[0m");
        strcat(p, "\033[0m");
    }
    //末尾加上换行符
    strcat(p + 1, "\n");

    //构造
    char buf[MAX_LOG_BUFF_SIZE] = {0};

    //可变参解析
    va_list args;
    va_start(args, format);
    int size = vsprintf(buf, fmtBuf, args);
    va_end(args);

    ::write(pfd[1], buf, size);
}
