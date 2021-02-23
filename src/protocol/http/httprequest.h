//
// Created by jiangsiyong on 2020/12/23.
//

#ifndef CANDY_HTTPREQUEST_H
#define CANDY_HTTPREQUEST_H

#include <map>
#include <string>
#include <vector>
#include "httpbase.h"

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
enum class HttpRequestDecodeState {
    INVALID,//无效

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

    HEADER_KEY,

    HEADER_BEFORE_COLON,//冒号之前
    HEADER_AFTER_COLON,//冒号
    HEADER_VALUE,//值

    WHEN_CR,//遇到一个回车之后
    CR_LF,//回车换行
    CR_LF_CR,//回车换行之后的状态

    BODY,//请求体

    COMPLETE//完成
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
 * http的请求类
 */
struct HttpRequest : public HttpBase {

    /**
     * 解析http协议
     * @param buf
     * @return
     */
    void tryDecode();

    const std::string &getMethod() const;

    const std::string &getUrl() const;

    const std::map<std::string, std::string> &getRequestParams() const;

    HttpRequestDecodeState getDecodeState() const;

    operator std::vector<char> &();

private:

    /**
     * 内部解析http协议
     * @param buf
     * @param end
     */
    void parseInternal(char *buf, size_t end);

    /**
     * 清空操作
     */
    void clear();

private:

    std::string _method;//请求方法

    std::string _url;//请求路径[不包含请求参数]

    std::map<std::string, std::string> _requestParams;//请求参数

    size_t _nextPos = 0;//下一个位置的
    size_t _contentLength = 0;//body的长度

    HttpRequestDecodeState _decodeState = HttpRequestDecodeState::START;//解析状态
};

#endif //CANDY_HTTPREQUEST_H
