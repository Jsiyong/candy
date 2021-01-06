//
// Created by jiangsiyong on 2020/12/23.
//

#ifndef CANDY_HTTP_H
#define CANDY_HTTP_H

#include <map>
#include <string>

#define CR '\r'
#define LF '\n'

/**
 * 字符串的buf，只存储对应的指针，不存储实际的内容
 */
struct StringBuffer {
    char *begin = NULL;//字符串开始位置
    char *end = NULL;//字符串结束位置

    operator std::string() const {
        return std::string(begin, end);
    }
};

/**
 * Http请求行的状态
 */
enum HttpRequestLineState {
    INVALID,//无效
    INVALID_METHOD,//无效请求方法
    INVALID_URI,//无效的请求路径
    INVALID_VERSION,//无效的协议版本号

    START,//请求行开始
    METHOD,//请求方法

    BEFORE_URI,//请求连接前的状态，需要'/'开头
    IN_URI,//url处理
    BEFORE_URI_PARAM_KEY,//URL请求参数键之前
    URI_PARAM_KEY,//URL请求参数键
    BEFORE_URI_PARAM_VALUE,//URL的参数值之前
    URI_PARAM_VALUE,//URL请求参数值

    BEFORE_PROTOCOL,//协议解析之前
    PROTOCOL,//协议

    BEFORE_VERSION,//版本开始前
    VERSION_SPLIT,//版本分隔符 '.'
    VERSION,//版本

    BEFORE_COMPLETE,//请求完成请 \r
    COMPLETE//结束
};

/**
 * http方法
 */
enum HttpMethod {
    UNKNOWN,
    GET,
    HEAD,
    POST
};

/**
 * http协议的基类
 */
struct HttpBase {

};

/**
 * http的请求类
 */
struct HttpRequest : public HttpBase {

    /**
     * 解析http协议
     * @param buf
     * @return
     */
    void tryDecode(const std::string &buf);

private:


    int parseRequestLine(const char *buf, int size);

    void parseInternal(const char *buf, int size);

private:

    std::string _method;//请求方法

    std::string _url;//请求路径[不包含请求参数]

    std::map<std::string, std::string> _requestParams;//请求参数

    std::string _protocol;//协议
    std::string _version;//版本

    std::map<std::string, std::string> _headers;//所有的请求头

    int _scannedPos = 0;//当前已经浏览到的位置
};

/**
 * Http的响应
 */
struct HttpResponse : public HttpBase {

};

#endif //CANDY_HTTP_H
