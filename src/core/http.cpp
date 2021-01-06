//
// Created by jiangsiyong on 2020/12/23.
//

#include "http.h"

#include <string.h>
#include <vector>

/*
GET /search?hl=zh-CN&source=hp&q=domety&aq=f&oq= HTTP/1.1
Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-excel, application/vnd.ms-powerpoint,
application/msword, application/x-silverlight, application/x-shockwave-flash
Referer: http://www.google.cn/
Accept-Language: zh-cn
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; TheWorld)
Host: www.google.cn
Connection: Keep-Alive
Cookie: PREF=ID=80a06da87be9ae3c:U=f7167333e2c3b714:NW=1:TM=1261551909:LM=1261551917:S=ybYcq2wpfefs4V9g;
NID=31=ojj8d-IygaEtSxLgaJmqSjVhCspkviJrB6omjamNrSm8lZhKy_yMfO2M4QMRKcH1g0iQv9u-2hfBW7bUFwVh7pGaRUb0RnHcJU37y-FxlRugatx63JLv7CWMD6UB_O_r

*/

/*
POST /search HTTP/1.1
Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-silverlight, application/x-shockwave-flash
Referer: http://www.google.cn
Accept-Language: zh-cn
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; TheWorld)
Host: www.google.cn
Connection: Keep-Alive
Cookie: PREF=ID=80a06da87be9ae3c:U=f7167333e2c3b714:NW=1:TM=1261551909:LM=1261551917:S=ybYcq2wpfefs4V9g; NID=31=ojj8d-IygaEtSxLgaJmqSjVhCspkviJrB6omjamNrSm8lZhKy_yMfO2M4QMRKcH1g0iQv9u-2hfBW7bUFwVh7pGaRUb0RnHcJU37y-FxlRugatx63JLv7CWMD6UB_O_r

hl=zh-CN&source=hp&q=domety
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

void HttpRequest::parseInternal(const char *buf, int size) {

    StringBuffer requestLine;//请求行
    std::vector<StringBuffer> requestHeaders;//请求头
    StringBuffer requestBody;//请求体

    std::string str = "POST /audiolibrary/music?ar=1595301089068&nv=1 HTTP/1.1\r\n"
                      "Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-silverlight, application/x-shockwave-flash\r\n"
                      "Referer: http://www.google.cn\r\n"
                      "Accept-Language: zh-cn\r\n"
                      "Accept-Encoding: gzip, deflate\r\n"
                      "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; TheWorld)\r\n"
                      "Host: www.google.cn\r\n"
                      "Connection: Keep-Alive\r\n"
                      "Cookie: PREF=ID=80a06da87be9ae3c:U=f7167333e2c3b714:NW=1:TM=1261551909:LM=1261551917:S=ybYcq2wpfefs4V9g; NID=31=ojj8d-IygaEtSxLgaJmqSjVhCspkviJrB6omjamNrSm8lZhKy_yMfO2M4QMRKcH1g0iQv9u-2hfBW7bUFwVh7pGaRUb0RnHcJU37y-FxlRugatx63JLv7CWMD6UB_O_r\r\n"
                      "\r\n"
                      "hl=zh-CN&source=hp&q=domety";

}


void HttpRequest::tryDecode(const std::string &buf) {
    //每次读一行解析

}

//解析请求行
int HttpRequest::parseRequestLine(const char *buf, int size) {

    StringBuffer method;
    StringBuffer url;

    StringBuffer requestParamKey;
    StringBuffer requestParamValue;

    StringBuffer protocol;
    StringBuffer version;

    int state = HttpRequestLineState::START;
    char *p = const_cast<char *>(buf);//去掉const限制

    while (state != HttpRequestLineState::INVALID
           && state != HttpRequestLineState::INVALID_METHOD
           && state != HttpRequestLineState::INVALID_URI
           && state != HttpRequestLineState::INVALID_VERSION
           && state != HttpRequestLineState::COMPLETE
           && p - buf < size) {

        char ch = *p;

        switch (state) {
            case HttpRequestLineState::START: {
                //空格 换行 回车都继续
                if (ch == CR || ch == LF || isblank(ch)) {
                    //do nothing
                } else if (isupper(ch)) {//判断是不是大写字符，不是的话是无效的
                    method.begin = p;//方法的起始点
                    state = HttpRequestLineState::METHOD;//如果遇到第一个字符，开始解析方法
                } else {
                    state = HttpRequestLineState::INVALID;
                }
                break;
            }
            case HttpRequestLineState::METHOD: {
                //方法需要大写字母，大写字母就继续
                if (isupper(ch)) {
                    //do nothing
                } else if (isblank(ch)) {//空格，说明方法解析结束，下一步开始解析URI
                    method.end = p;//方法解析结束
                    _method = method;
                    state = HttpRequestLineState::BEFORE_URI;
                } else {
                    state = HttpRequestLineState::INVALID_METHOD;//其他情况是无效的请求方法
                }
                break;
            }
            case HttpRequestLineState::BEFORE_URI: {
                //请求连接前的处理，需要'/'开头
                if (ch == '/') {
                    url.begin = p;
                    state = HttpRequestLineState::IN_URI;//下一步就是开始处理连接
                } else if (isblank(ch)) {
                    //do nothing
                } else {
                    state = HttpRequestLineState::INVALID;//无效的
                }
                break;
            }
            case HttpRequestLineState::IN_URI: {
                //开始处理请求路径的字符串
                if (ch == '?') {//转为处理请求的key值
                    url.end = p;
                    _url = url;
                    state = HttpRequestLineState::BEFORE_URI_PARAM_KEY;
                } else if (isblank(ch)) {//遇到空格，请求路径解析完成，开始解析协议
                    url.end = p;
                    _url = url;
                    state = HttpRequestLineState::PROTOCOL;
                } else {
                    //do nothing
                }
                break;
            }
            case HttpRequestLineState::BEFORE_URI_PARAM_KEY: {
                if (isblank(ch)) {
                    state = HttpRequestLineState::INVALID_URI;
                } else {
                    requestParamKey.begin = p;
                    state = HttpRequestLineState::URI_PARAM_KEY;
                }
                break;
            }
            case HttpRequestLineState::URI_PARAM_KEY: {
                if (ch == '=') {
                    requestParamKey.end = p;
                    state = HttpRequestLineState::BEFORE_URI_PARAM_VALUE;//开始解析参数值
                } else if (isblank(ch)) {
                    state = HttpRequestLineState::INVALID_URI;//无效的请求参数
                } else {
                    //do nothing
                }
                break;
            }
            case HttpRequestLineState::BEFORE_URI_PARAM_VALUE: {
                if (isblank(ch)) {
                    state = HttpRequestLineState::INVALID_URI;
                } else {
                    requestParamValue.begin = p;
                    state = HttpRequestLineState::URI_PARAM_VALUE;
                }
                break;
            }
            case HttpRequestLineState::URI_PARAM_VALUE: {
                if (ch == '&') {
                    requestParamValue.end = p;
                    //收获一个请求参数
                    _requestParams.insert({requestParamKey, requestParamValue});
                    state = HttpRequestLineState::BEFORE_URI_PARAM_KEY;
                } else if (isblank(ch)) {
                    requestParamValue.end = p;
                    //空格也收获一个请求参数
                    _requestParams.insert({requestParamKey, requestParamValue});
                    state = HttpRequestLineState::BEFORE_PROTOCOL;
                } else {
                    //do nothing
                }
                break;
            }
            case HttpRequestLineState::BEFORE_PROTOCOL: {
                if (isblank(ch)) {
                    //do nothing
                } else {
                    protocol.begin = p;
                    state = HttpRequestLineState::PROTOCOL;
                }
                break;
            }
            case HttpRequestLineState::PROTOCOL: {
                //解析请求协议
                if (ch == '/') {
                    protocol.end = p;
                    _protocol = protocol;
                    state = HttpRequestLineState::BEFORE_VERSION;
                } else {
                    //do nothing
                }
                break;
            }
            case HttpRequestLineState::BEFORE_VERSION: {
                if (isdigit(ch)) {
                    version.begin = p;
                    state = HttpRequestLineState::VERSION;
                } else {
                    state = HttpRequestLineState::INVALID_VERSION;
                }
                break;
            }
            case HttpRequestLineState::VERSION: {
                //协议解析，如果不是数字或者. 就不对
                if (ch == CR) {
                    version.end = p;
                    _version = version;
                    state = HttpRequestLineState::BEFORE_COMPLETE;
                } else if (ch == '.') {
                    //遇到版本分割
                    state = HttpRequestLineState::VERSION_SPLIT;
                } else if (isdigit(ch)) {
                    //do nothing
                } else {
                    state = HttpRequestLineState::INVALID_VERSION;//不能不是数字
                }
                break;
            }
            case HttpRequestLineState::VERSION_SPLIT: {
                //遇到版本分割符，字符必须是数字，其他情况都是错误
                if (isdigit(ch)) {
                    state = HttpRequestLineState::VERSION;
                } else {
                    state = HttpRequestLineState::INVALID_VERSION;
                }
                break;
            }
            case HttpRequestLineState::BEFORE_COMPLETE: {
                if (ch == LF) {
                    state = HttpRequestLineState::COMPLETE;
                } else {
                    state = HttpRequestLineState::INVALID;
                }
                break;
            }
            default:
                break;
        }

        p++;//指针偏移
        _scannedPos++;//浏览位置++
    }
    return state;
}
