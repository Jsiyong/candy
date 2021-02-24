//
// Created by Jsiyong on 2021-02-20.
//

#include <string>
#include "httpresponse.h"
#include "../../log/logger.h"

constexpr int MaxStatusLine = 1024;//最大的状态行长度

void HttpResponse::encode(std::string &dst) {
    _version = "1.1";
    _protocol = "HTTP";

    //先添加请求体长度的请求头
    _headers["Content-Length"] = std::to_string(_body.size());

    //首先先拼接状态行
    char statusLine[MaxStatusLine];
    int size = snprintf(statusLine, MaxStatusLine, "%s/%s %d %s\r\n", _protocol.c_str(), _version.c_str(), _statusCode,
                        _statusWord.c_str());
    error_if(size < 0, "gen status line error");
    dst.append(statusLine, statusLine + size);

    //再拼接响应头
    for (const auto &header : _headers) {
        dst.append(header.first).append(": ").append(header.second).append("\r\n");
    }
    // \r\n
    dst.append("\r\n");

    //最后再拼接响应体
    dst.append(_body);
}

void HttpResponse::setNotFound() {
    _statusCode = 404;
    _statusWord = "Not Found";
    _body.assign(_statusWord.begin(), _statusWord.end());
}

void HttpResponse::clear() {
    HttpBase::clear();
    _statusCode = 200;
    _statusWord.assign("OK");
}

HttpResponse::HttpResponse() {
    _statusCode = 200;
    _statusWord.assign("OK");
}
