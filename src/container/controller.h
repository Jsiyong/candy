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
#include "./responsebody.h"

//拼接
#define M_CONCAT_(a, b) a##b
#define M_CONCAT(a, b)  M_CONCAT_(a, b)

/**
 * 路径请求映射，第一个参数是函数名，第二个参数是请求路径
 */
#define RequestMapping(func, url)                                                                                                                                                 \
const Handler M_CONCAT(_ph_, __LINE__) = {                  \
    (decltype(this->func(NULL, NULL))*)NULL,                \
    url,                                                    \
    [this](HttpRequest *request, HttpResponse *response) {  \
        this->smartRegister.doNothing();                    \
        return this->func(request,response);                \
    }                                                       \
};                                                          \
template<bool B = true>


/**
 * 事件分发器：单例
 * 主要负责请求的URL的分发
 */
struct EventDispatcher : public Singleton<EventDispatcher> {

    typedef std::function<void(HttpRequest *request, HttpResponse *response)> StandardHandler;//返回值是空
    typedef std::function<std::string(HttpRequest *request, HttpResponse *response)> FileHandler;//返回值是文件
    typedef std::function<ResponseBody(HttpRequest *request, HttpResponse *response)> JsonHandler;//返回值是json

    /**
     * 注册一个处理函数
     * @param path
     * @param handler
     */
    bool registerHandler(const std::string &path, const StandardHandler &handler);

    bool registerHandler(const std::string &path, const FileHandler &handler);

    bool registerHandler(const std::string &path, const JsonHandler &handler);

    /**
     * 通过路径获取处理函数
     * @return
     */
    StandardHandler getStandardHandlerByPath(const std::string &path);

    JsonHandler getJsonHandlerByPath(const std::string &path);

    FileHandler getFileHandlerByPath(const std::string &path);

    /**
     * 判断路径是否包含在映射中
     * @param path
     * @return
     */
    bool isContains(const std::string &path);

private:
    //主要负责存储路径对应的处理方法
    std::unordered_map<std::string, StandardHandler> _standardHandlerMap;
    std::unordered_map<std::string, FileHandler> _fileHandlerMap;
    std::unordered_map<std::string, JsonHandler> _jsonHandlerMap;
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
    struct Handler {
        Handler(void *, const std::string &path, const EventDispatcher::StandardHandler &handler) {
            EventDispatcher::getInstance()->registerHandler(path, handler);
        }

        Handler(ResponseBody *, const std::string &path, const EventDispatcher::JsonHandler &handler) {
            EventDispatcher::getInstance()->registerHandler(path, handler);
        }

        Handler(std::string *, const std::string &path, const EventDispatcher::FileHandler &handler) {
            EventDispatcher::getInstance()->registerHandler(path, handler);
        }
    };
};

template<typename T>
typename Controller<T>::SmartRegister Controller<T>::smartRegister;

#endif //CANDY_CONTROLLER_H
