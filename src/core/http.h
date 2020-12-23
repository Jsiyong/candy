//
// Created by jiangsiyong on 2020/12/23.
//

#ifndef CANDY_HTTP_H
#define CANDY_HTTP_H


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
    HttpResult tryDecode(const char *buf);

private:

};

/**
 * Http的响应
 */
struct HttpResponse : public HttpBase {

};

#endif //CANDY_HTTP_H
