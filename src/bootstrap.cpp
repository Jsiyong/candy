//
// Created by jiangsiyong on 2020/12/15.
//

#include <stdlib.h>
#include "conf/servconf.h"
#include "startup/cmdparser.h"
#include "log/logger.h"
#include "connector/server.h"

static Logger *pLogger = NULL;
static LogAppender *pLogAppender = NULL;

/**
 * 主函数退出前需要做些清理工作
 */
static void beforeExit() {
    delete pLogAppender;
    delete pLogger;
}

int main(int argc, char **argv) {
    //1：参数解析
    CmdParser::parse(argc, argv);

    //2：日志模块初始化，采用异步日志的形式
    pLogger = new AsyncLogger();
    pLogAppender = new ConsoleLogAppender();
    pLogger->addAppender(pLogAppender);
    LogManager::getInstance()->setLogger(pLogger);

    //3:启动服务器
    Server servBase;
    servBase.runAt(serverConf.getHost(), serverConf.getPort());

    //挂上清理函数
    atexit(beforeExit);

    //退出主线程
    return 0;
}