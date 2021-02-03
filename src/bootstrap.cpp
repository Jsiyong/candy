//
// Created by jiangsiyong on 2020/12/15.
//

#include <stdlib.h>
#include "conf/serv-conf.h"
#include "startup/cmd-parser.h"
#include "log/logger.h"
#include "core/serv-base.h"

int main(int argc, char **argv) {
    //1：参数解析
    CmdParser::parse(argc, argv);

    Logger *log = new Logger();
    LogManager::getInstance()->setLogger(log);
    log->addAppender(new ConsoleLogAppender());

//    //2：fork日志进程，日记进程单独处理
//    if (Log::init() < 0) {
//        //初始化失败
//        exit(1);
//    }
    //开始写日志
    info("log info, server config [port:%d],[addr:%s],[mode:%d]", servConf.port, servConf.host, servConf.mode);


    //3:启动服务器
    ServBase servBase;
    servBase.start(servConf.host, servConf.port);
    servBase.run();

    exit(EXIT_SUCCESS);
}