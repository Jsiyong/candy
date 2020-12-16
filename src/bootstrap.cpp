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
        .addr = (char *) DEFAULT_SERV_ADDR,
        .mode = ServMode::FOREGROUND
};

#include <unistd.h>

int main(int argc, char **argv) {


    //1：参数解析
    CmdParser::parse(argc, argv);

    //2：fork日志进程，日记进程单独处理
    if (Log::init() < 0) {
        //初始化失败
        exit(1);
    }
    //开始写日志
    log(Log::TRACE, "log trace, server config [port:%d],[addr:%s],[mode:%d]", servConf.port, servConf.addr,
        servConf.mode);
    log(Log::INFO, "log info, server config [port:%d],[addr:%s],[mode:%d]", servConf.port + 1, servConf.addr,
        servConf.mode + 1);
    log(Log::WARN, "log warn, server config [port:%d],[addr:%s],[mode:%d]", servConf.port + 2, servConf.addr,
        servConf.mode + 2);
    log(Log::ERROR, "log error, server config [port:%d],[addr:%s],[mode:%d]", servConf.port + 3, servConf.addr,
        servConf.mode + 3);
    log(Log::FATAL, "log fatal, server config [port:%d],[addr:%s],[mode:%d]", servConf.port + 4, servConf.addr,
        servConf.mode + 4);


    while (1);



    //3:启动服务器

    exit(EXIT_SUCCESS);
}