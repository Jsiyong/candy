//
// Created by jiangsiyong on 2020/12/15.
//

#include <unistd.h>
#include <getopt.h>

#include "cmd-parser.h"
#include "../conf/serv-conf.h"

//-a --addr 监听地址
//-d --daemon 后台运行
//-f --foreground 前台运行
//-h --help 帮助
//-p --port 端口
void CmdParser::parse(int argc, char **argv) {

    static struct option options[] = {
            {"addr",       required_argument, NULL, 'a'},
            {"daemon",     no_argument,       NULL, 'd'},
            {"foreground", no_argument,       NULL, 'f'},
            {"help",       no_argument,       NULL, 'h'},
            {"port",       required_argument, NULL, 'p'},
            {NULL,         no_argument,       NULL, 0}
    };

    int c;
    //解析命令行参数
    while ((c = getopt_long(argc, argv, "a:dfhp:", options, NULL)) != -1) {
        switch (c) {
            case 'a':
                servConf.host = optarg;
                break;
            case 'd':
                servConf.mode = ServMode::DAEMON;
                break;
            case 'f':
                servConf.mode = ServMode::FOREGROUND;
                break;
            case '?':
            case 'h':
                printHelp();
                exit(EXIT_SUCCESS);
                break;
            case 'p':
                servConf.port = atoi(optarg);
                break;
            default:
                printf("?? getopt returned character code 0%o ??\n", c);
                abort();//未知错误，直接结束程序
                break;
        }
    }
}

void CmdParser::printHelp() {
    printf("-a --addr 监听地址\n");
    printf("-d --daemon 后台运行\n");
    printf("-f --foreground 前台运行\n");
    printf("-h --help 帮助\n");
    printf("-p --port 端口\n");
}
