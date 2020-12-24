//
// Created by jiangsiyong on 2020/12/23.
//

#include "http.h"

#include <string.h>
#include <vector>

/*
GET / HTTP/1.1
Connection: keep-alive
Host: 192.168.66.66:8888
User-Agent: Apache-HttpClient/4.5.12 (Java/1.8.0_201)

request
*/

/*
HTTP/1.1 200 OK
Date: Mon, 27 Jul 2009 12:28:53 GMT
Server: Apache
Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
ETag: "34aa387-d-1568eb00"
Accept-Ranges: bytes
Content-Length: 51
Vary: Accept-Encoding
Content-Type: text/plain

response
*/

//首先解析请求行
//接着解析请求头
//最后解析请求体

/**
 * 字符串的buf，只存储对应的指针，不存储实际的内容
 */
struct StringBuffer {
    StringBuffer() {}

    StringBuffer(const char *from, const char *to) : _from(from), _to(to) {}

private:
    const char *_from;//字符串开始位置
    const char *_to;//字符串结束位置
};

void HttpRequest::parseInternal(const char *buf, int size) {

    StringBuffer requestLine;//请求行
    std::vector<StringBuffer> requestHeaders;//请求头
    StringBuffer requestBody;//请求体

    //先找到第一个有效的字符
    while (_scannedIndex < size &&
           (buf[_scannedIndex] == ' ' || buf[_scannedIndex] == '\r' || buf[_scannedIndex] == '\n')) {
        _scannedIndex++;
    }
    const char *p = buf + _scannedIndex;//第一个有效字符的位置

    //找出请求行出来
    while (_scannedIndex + 2 < size) {
        if (memcmp(buf + _scannedIndex, "\r\n", 2) == 0) {
            requestLine = StringBuffer(p, buf + _scannedIndex);
            break;
        }
        _scannedIndex++;
    }
    //找出请求头出来
    int preHeaderIndex = 0;
    while (_scannedIndex + 4 < size) {
        if (memcmp(buf + _scannedIndex, "\r\n\r\n", 4) == 0) {
            break;
        }

        if (memcmp(buf + _scannedIndex, "\r\n", 2) == 0) {

        }
        _scannedIndex++;
    }

    //找出body出来


}


HttpResult HttpRequest::tryDecode(const std::string &buf) {
    //每次读一行解析



    return COMPLETED;
}
