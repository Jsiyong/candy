//
// Created by Jsiyong on 2021-02-20.
//

#include <string>
#include "httpresponse.h"
#include "../../log/logger.h"

constexpr int MaxStatusLine = 1024;//最大的状态行长度

HttpResponse::HttpResponse() {
#if 0
    char resp[MAX_BUFF_SIZE] = {0};
    char *p = resp;
    const char *header = "HTTP/1.1 200 OK\r\n";
    const char *linefmt = "Content-Type:%s\r\nContent-Length:%d\r\n";
    //body
    const char *body = R"({"title":"hello","body":"world"})";
    char line[MAX_BUFF_SIZE] = {0};
    sprintf(line, linefmt, "application/json; charset=UTF-8", strlen(body));

    //头部
    strcat(p, header);
    p += strlen(header);

    //键值对
    strcat(p, line);
    p += strlen(line);

    //换行
    strcat(p, "\r\n");
    p += strlen("\r\n");


    strcat(p, body);
    trace("response data:%s", resp);
    trace("response ok!!");


    std::string body = R"({"title":"hello","body":"world"})";
    std::string str = "HTTP/1.1 200 OK\r\n";
    str += "Content-Type:application/json; charset=UTF-8\r\nContent-Length:" + std::to_string(body.size()) + "\r\n";
    str += "\r\n";
    str += body;
    _buf.insert(_buf.end(), str.begin(), str.end());
#endif
    _version = "1.1";
    _protocol = "HTTP";
    _statusCode = 200;
    _statusWord = "OK";
}

HttpResponse::operator std::vector<char> &() {
    return _buf;
}

void HttpResponse::encode() {

    //先添加请求体长度的请求头
    _headers["Content-Length"] = std::to_string(_body.size());

    //首先先拼接状态行
    char statusLine[MaxStatusLine];
    int size = snprintf(statusLine, MaxStatusLine, "%s/%s %d %s\r\n", _protocol.c_str(), _version.c_str(), _statusCode,
                        _statusWord.c_str());
    error_if(size < 0, "gen status line error");
    _buf.insert(_buf.end(), statusLine, statusLine + size);

    //再拼接响应头
    for (const auto &header : _headers) {
        _buf.insert(_buf.end(), header.first.begin(), header.first.end());
        _buf.insert(_buf.end(), {':', ' '});
        _buf.insert(_buf.end(), header.second.begin(), header.second.end());
        _buf.insert(_buf.end(), {'\r', '\n'});
    }
    // \r\n
    _buf.insert(_buf.end(), {'\r', '\n'});

    //最后再拼接响应体
    _buf.insert(_buf.end(), _body.begin(), _body.end());
}
