//
// Created by jiangsiyong on 2020/12/15.
//

#include <stdlib.h>
#include "conf/servconf.h"
#include "startup/cmdparser.h"
#include "log/logger.h"
#include "connector/server.h"
#include <signal.h>

//valgrind --log-file="valgrind.log" --tool=memcheck --leak-check=full --show-reachable=yes --trace-children=yes ./candy
int main(int argc, char **argv) {
    //1：参数解析
    CmdParser::parse(argc, argv);

    //2: 设置线程池属性
    ThreadPoolExecutor::getInstance()->startWith(10, 15, 30);

    //3：日志模块初始化，采用异步日志的形式
    static Logger *pLogger = new AsyncLogger();
    static LogAppender *pLogAppender = new ConsoleLogAppender();
    pLogger->addAppender(pLogAppender);
    LogManager::getInstance()->setLogger(pLogger);

    //4:启动服务器
    static auto pServer = new Server();
    //挂上信号处理函数
    signal(SIGINT, [](int) { pServer->close(); });

    //挂上清理函数:主函数退出前需要做些清理工作
    atexit([]() {
        delete pServer;//删除服务器
        //最后再删除日志
        delete pLogger;//删除logger
        delete pLogAppender;//删除日志
    });

    //主线程循环accept
    pServer->execLoopAt(serverConf.getHost(), serverConf.getPort());
    return 0;
}