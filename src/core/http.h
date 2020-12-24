//
// Created by jiangsiyong on 2020/12/23.
//

#ifndef CANDY_HTTP_H
#define CANDY_HTTP_H

#include <map>
#include <string>

/**
 * Http解析结果
 */
enum HttpResult {
    ERROR,
    COMPLETED,
    UNCOMPLETED
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
    HttpResult tryDecode(const std::string &buf);

private:

    void parseInternal(const char *buf, int size);

private:

    std::map<std::string, std::string> _headers;//所有的请求头


    int _scannedIndex;//已经浏览过的索引

};

/**
 * Http的响应
 */
struct HttpResponse : public HttpBase {

};

#endif //CANDY_HTTP_H
