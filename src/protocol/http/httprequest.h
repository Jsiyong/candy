//
// Created by jiangsiyong on 2020/12/23.
//

#ifndef CANDY_HTTPREQUEST_H
#define CANDY_HTTPREQUEST_H

#include <map>
#include <string>
#include <vector>
#include "httpbase.h"

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

    HttpRequest();

    /**
     * 解析http协议
     * @param buf
     * @return
     */
    void tryDecode();

    const std::string &getMethod() const;

    const std::string &getUrl() const;

    const std::map<std::string, std::string> &getRequestParams() const;

    bool completed() const;

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
    void clear() override;

private:

    std::string _method;//请求方法

    std::string _url;//请求路径[不包含请求参数]

    std::map<std::string, std::string> _requestParams;//请求参数

    size_t _nextPos = 0;//下一个位置的
    size_t _contentLength = 0;//body的长度

    int _decodeState = 0;//解析状态

private:
    std::string _requestParamKey;
    std::string _requestParamValue;

    std::string _headerKey;
    std::string _headerValue;
};

#endif //CANDY_HTTPREQUEST_H
