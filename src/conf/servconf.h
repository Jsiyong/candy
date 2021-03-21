//
// Created by jiangsiyong on 2020/12/15.
//

#ifndef CANDY_SERVCONF_H
#define CANDY_SERVCONF_H

#include <string>
#include "../protocol/json/serialization.h"

#define DEFAULT_SERV_PORT 8888
#define DEFAULT_SERV_ADDR "0.0.0.0"
#define DEFAULT_WEB_ROOT "webapp"
//默认的内置线程数有1条：epoll线程
#define DEFAULT_THREAD_COUNT 1
#define DEFAULT_CORE_THREAD_COUNT DEFAULT_THREAD_COUNT + 5
#define DEFAULT_MAX_THREAD_COUNT 30
#define DEFAULT_EXPIRY_TIME_OUT 30
/**
 * 线程池参数
 */
struct ThreadPoolConf {
    int coreCount = DEFAULT_CORE_THREAD_COUNT;//线程池最小数量[核心线程数]
    int maxCount = DEFAULT_MAX_THREAD_COUNT;//线程池最大数量
    int expiryTimeout = DEFAULT_EXPIRY_TIME_OUT;//失效的时间
};

Serialization(ThreadPoolConf, coreCount, maxCount, expiryTimeout)

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
     * 获取webRoot的全路径
     * @return
     */
    std::string getAbsoulteWebRoot() const;

    /**
     * 获取程序名称
     * @return
     */
    std::string getServerName() const;

    /**
     * 获取程序所在的目录
     * @return
     */
    std::string getServerDir() const;

    bool isToStop() const;

    void setToStop(bool toStop);

    const std::string &getConfigPath() const;

    void setConfigPath(const std::string &configPath);

public:
    int port = DEFAULT_SERV_PORT; //服务器端口
    std::string host = DEFAULT_SERV_ADDR;//服务器的监听地址
    short mode = Mode::FOREGROUND;//启动模式
    std::string webRoot = DEFAULT_WEB_ROOT;//对外网站的根路径

    ThreadPoolConf threadPoolConf;//线程池配置属性
private:
    std::string _serverPath;//服务器运行路径
    bool _toStop = false;//让服务器停止的参数
    std::string _configPath;//配置文件的位置
};

Serialization(ServerConf, port, host, mode, webRoot, threadPoolConf)

extern struct ServerConf serverConf;

#endif //CANDY_SERVCONF_H
