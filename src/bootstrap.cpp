//
// Created by jiangsiyong on 2020/12/15.
//

#include <stdlib.h>
#include "conf/serv-conf.h"
#include "startup/cmd-parser.h"

//全局变量服务器配置
struct ServConf servConf = {
        .port = DEFAULT_SERV_PORT,
        .addr = (char *) DEFAULT_SERV_ADDR,
        .mode = ServMode::FOREGROUND
};


int main(int argc, char **argv) {


    //1：参数解析
    CmdParser::parse(argc, argv);
    printf("server config [port:%d],[addr:%s],[mode:%d]\n", servConf.port, servConf.addr, servConf.mode);

    //2：fork日志进程，日记进程单独处理




    //3:启动服务器

    exit(EXIT_SUCCESS);
}