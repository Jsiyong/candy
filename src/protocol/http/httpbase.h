//
// Created by Jsiyong on 2021-02-23.
//

#ifndef CANDY_HTTPBASE_H
#define CANDY_HTTPBASE_H

#include <string>
#include <vector>
#include <map>

/**
 * Http报文的基类
 */
struct HttpBase {

    const std::string &getProtocol() const;

    void setProtocol(const std::string &protocol);

    const std::string &getVersion() const;

    void setVersion(const std::string &version);

    const std::map<std::string, std::string> &getHeaders() const;

    void setHeaders(const std::map<std::string, std::string> &headers);

    void setHeader(const std::string &key, const std::string &value);

    const std::string &getBody() const;

    void setBody(const std::string &body);

    virtual void clear();

protected:
    std::string _protocol;//协议
    std::string _version;//版本

    std::map<std::string, std::string> _headers;//所有的请求头/响应头
    std::string _body;//请求体/响应体
};

#endif //CANDY_HTTPBASE_H
