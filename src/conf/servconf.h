//
// Created by jiangsiyong on 2020/12/15.
//

#ifndef CANDY_SERVCONF_H
#define CANDY_SERVCONF_H

#include <string>

#define DEFAULT_SERV_PORT 8888
#define DEFAULT_SERV_ADDR "192.168.66.66"

/**
 * 服务器的配置信息
 */
struct ServerConf {

    ServerConf();

    enum Mode {
        FOREGROUND = 0,//前台运行
        DAEMON//守护进程
    };

    /**
     * 获取服务器运行路径
     * @return
     */
    const std::string &getServerPath();

    /**
     * 获取程序名称
     * @return
     */
    std::string getServerName();

    /**
     * 获取程序所在的目录
     * @return
     */
    std::string getServerDir();

    void setPort(int port);

    void setHost(const std::string &host);

    void setMode(short mode);

    int getPort() const;

    const std::string &getHost() const;

private:
    int _port; //服务器端口
    std::string _host;//服务器的监听地址
    short _mode;//启动模式

    std::string _serverPath;//服务器运行路径

};

extern struct ServerConf serverConf;

#endif //CANDY_SERVCONF_H
