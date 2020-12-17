//
// Created by jiangsiyong on 2020/12/15.
//

#include <stdlib.h>
#include "conf/serv-conf.h"
#include "startup/cmd-parser.h"
#include "log/log.h"

//全局变量服务器配置
struct ServConf servConf = {
        .port = DEFAULT_SERV_PORT,
        .host = (char *) DEFAULT_SERV_ADDR,
        .mode = ServMode::FOREGROUND
};

int main(int argc, char **argv) {

    //1：参数解析
    CmdParser::parse(argc, argv);

    //2：fork日志进程，日记进程单独处理
    if (Log::init() < 0) {
        //初始化失败
        exit(1);
    }
    //开始写日志
    info("log info, server config [port:%d],[addr:%s],[mode:%d]", servConf.port, servConf.host, servConf.mode);

    while (1);



    //3:启动服务器

    exit(EXIT_SUCCESS);
}