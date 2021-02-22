//
// Created by Jsiyong on 2021-02-22.
//

#ifndef CANDY_CONTROLLER_H
#define CANDY_CONTROLLER_H

#include "../util/singleton.h"
#include <unordered_map>
#include <functional>
#include "../protocol/http/httprequest.h"
#include "../protocol/http/httpresponse.h"

//拼接
#define M_CONCAT_(a, b) a##b
#define M_CONCAT(a, b)  M_CONCAT_(a, b)

/**
 * 路径请求映射，第一个参数是函数名，第二个参数是请求路径
 */
#define RequestMapping(func, url)                                                                                                                                              \
const PathHandler M_CONCAT(_ph_, __LINE__) = { url, [this](HttpRequest *request, HttpResponse *response) { this->smartRegister.doNothing(); this->func(request,response); } }; \
template<bool B = true>


/**
 * 事件分发器：单例
 * 主要负责请求的URL的分发
 */
struct EventDispatcher : public Singleton<EventDispatcher> {

    typedef std::function<void(HttpRequest *request, HttpResponse *response)> PathHandler;

    /**
     * 注册一个路径处理函数
     * @param path
     * @param pathHandler
     */
    bool registerPathHandler(const std::string &path, const PathHandler &pathHandler);

    /**
     * 通过路径获取处理函数
     * @return
     */
    PathHandler getHandlerByPath(const std::string &path);

private:
    //主要负责存储路径对应的处理方法
    std::unordered_map<std::string, PathHandler> _pathHandlerMap;
};

/**
 * 所有的Controller的基类，也是单例
 * @tparam T
 */
template<typename T>
struct Controller : public Singleton<T> {

    /**
     * 自动注册机
     */
    static struct SmartRegister {
        /**
         * 空函数，因为模板静态成员的需要
         */
        void doNothing() const {}

        explicit SmartRegister() {
            //单实例
            T::getInstance();
        }
    } smartRegister;

    /**
     * 路径对应的处理类
     */
    struct PathHandler {
        PathHandler(const std::string &path, const EventDispatcher::PathHandler &handler) {
            EventDispatcher::getInstance()->registerPathHandler(path, handler);
        }
    };
};

template<typename T>
typename Controller<T>::SmartRegister Controller<T>::smartRegister;

#endif //CANDY_CONTROLLER_H
