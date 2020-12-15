//
// Created by jiangsiyong on 2020/12/15.
//

#ifndef CANDY_SERV_CONF_H
#define CANDY_SERV_CONF_H

#include <string>
#include <ostream>

#define DEFAULT_SERV_PORT 8888
#define DEFAULT_SERV_ADDR "0.0.0.0"

enum ServMode {
    FOREGROUND = 0,//前台运行
    DAEMON//守护进程
};

/**
 * 服务器的配置信息
 */
struct ServConf {

    int port; //服务器端口
    char *addr;//服务器的监听地址
    short mode;//启动模式
};

extern struct ServConf servConf;

#endif //CANDY_SERV_CONF_H
