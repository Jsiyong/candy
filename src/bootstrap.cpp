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
static ThreadPoolExecutor *pThreadPool = NULL;
static Server *pServer = NULL;

/**
 * 主函数退出前需要做些清理工作
 */
static void beforeExit() {
    delete pLogAppender;
    delete pLogger;//删除logger
    delete pServer;//删除服务器
    delete pThreadPool;//删除线程池
}

#include <unistd.h>

int main(int argc, char **argv) {
    //1：参数解析
    CmdParser::parse(argc, argv);

    //2: 设置线程池属性
    pThreadPool = ThreadPoolExecutor::getInstance();
    pThreadPool->startWith(10, 15, 30);

    //3：日志模块初始化，采用异步日志的形式
    pLogger = new AsyncLogger();
    pLogAppender = new ConsoleLogAppender();
    pLogger->addAppender(pLogAppender);
    LogManager::getInstance()->setLogger(pLogger);

    //4:启动服务器
    pServer = new Server();
    //主线程循环accept
    pServer->execLoopAt(serverConf.getHost(), serverConf.getPort());
    //挂上清理函数
    atexit(beforeExit);

    //主线程执行事件循环，等待释放
    //退出主线程
    return 0;
}