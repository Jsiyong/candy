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
void HttpRequest::tryDecode(const std::vector<char> &buf) {
    this->parseInternal(buf.data(), buf.size());
}

//解析请求行
void HttpRequest::parseInternal(const char *buf, int size) {

    StringBuffer method;
    StringBuffer url;

    StringBuffer requestParamKey;
    StringBuffer requestParamValue;

    StringBuffer protocol;
    StringBuffer version;

    StringBuffer headerKey;
    StringBuffer headerValue;

    int bodyLength = 0;

    char *p0 = const_cast<char *>(buf + _nextPos);//去掉const限制

    while (_decodeState != HttpRequestDecodeState::INVALID
           && _decodeState != HttpRequestDecodeState::INVALID_METHOD
           && _decodeState != HttpRequestDecodeState::INVALID_URI
           && _decodeState != HttpRequestDecodeState::INVALID_VERSION
           && _decodeState != HttpRequestDecodeState::INVALID_HEADER
           && _decodeState != HttpRequestDecodeState::COMPLETE
           && _nextPos < size) {

        char ch = *p0;//当前的字符
        char *p = p0++;//指针偏移
        int scanPos = _nextPos++;//下一个指针往后偏移

        switch (_decodeState) {
            case HttpRequestDecodeState::START: {
                //空格 换行 回车都继续
                if (ch == CR || ch == LF || isblank(ch)) {
                    //do nothing
                } else if (isupper(ch)) {//判断是不是大写字符，不是的话是无效的
                    method.begin = p;//方法的起始点
                    _decodeState = HttpRequestDecodeState::METHOD;//如果遇到第一个字符，开始解析方法
                } else {
                    _decodeState = HttpRequestDecodeState::INVALID;
                }
                break;
            }
            case HttpRequestDecodeState::METHOD: {
                //方法需要大写字母，大写字母就继续
                if (isupper(ch)) {
                    //do nothing
                } else if (isblank(ch)) {//空格，说明方法解析结束，下一步开始解析URI
                    method.end = p;//方法解析结束
                    _method = method;
                    _decodeState = HttpRequestDecodeState::BEFORE_URI;
                } else {
                    _decodeState = HttpRequestDecodeState::INVALID_METHOD;//其他情况是无效的请求方法
                }
                break;
            }
            case HttpRequestDecodeState::BEFORE_URI: {
                //请求连接前的处理，需要'/'开头
                if (ch == '/') {
                    url.begin = p;
                    _decodeState = HttpRequestDecodeState::IN_URI;//下一步就是开始处理连接
                } else if (isblank(ch)) {
                    //do nothing
                } else {
                    _decodeState = HttpRequestDecodeState::INVALID;//无效的
                }
                break;
            }
            case HttpRequestDecodeState::IN_URI: {
                //开始处理请求路径的字符串
                if (ch == '?') {//转为处理请求的key值
                    url.end = p;
                    _url = url;
                    _decodeState = HttpRequestDecodeState::BEFORE_URI_PARAM_KEY;
                } else if (isblank(ch)) {//遇到空格，请求路径解析完成，开始解析协议
                    url.end = p;
                    _url = url;
                    _decodeState = HttpRequestDecodeState::BEFORE_PROTOCOL;
                } else {
                    //do nothing
                }
                break;
            }
            case HttpRequestDecodeState::BEFORE_URI_PARAM_KEY: {
                if (isblank(ch) || ch == LF || ch == CR) {
                    _decodeState = HttpRequestDecodeState::INVALID_URI;
                } else {
                    requestParamKey.begin = p;
                    _decodeState = HttpRequestDecodeState::URI_PARAM_KEY;
                }
                break;
            }
            case HttpRequestDecodeState::URI_PARAM_KEY: {
                if (ch == '=') {
                    requestParamKey.end = p;
                    _decodeState = HttpRequestDecodeState::BEFORE_URI_PARAM_VALUE;//开始解析参数值
                } else if (isblank(ch)) {
                    _decodeState = HttpRequestDecodeState::INVALID_URI;//无效的请求参数
                } else {
                    //do nothing
                }
                break;
            }
            case HttpRequestDecodeState::BEFORE_URI_PARAM_VALUE: {
                if (isblank(ch) || ch == LF || ch == CR) {
                    _decodeState = HttpRequestDecodeState::INVALID_URI;
                } else {
                    requestParamValue.begin = p;
                    _decodeState = HttpRequestDecodeState::URI_PARAM_VALUE;
                }
                break;
            }
            case HttpRequestDecodeState::URI_PARAM_VALUE: {
                if (ch == '&') {
                    requestParamValue.end = p;
                    //收获一个请求参数
                    _requestParams.insert({requestParamKey, requestParamValue});
                    _decodeState = HttpRequestDecodeState::BEFORE_URI_PARAM_KEY;
                } else if (isblank(ch)) {
                    requestParamValue.end = p;
                    //空格也收获一个请求参数
                    _requestParams.insert({requestParamKey, requestParamValue});
                    _decodeState = HttpRequestDecodeState::BEFORE_PROTOCOL;
                } else {
                    //do nothing
                }
                break;
            }
            case HttpRequestDecodeState::BEFORE_PROTOCOL: {
                if (isblank(ch)) {
                    //do nothing
                } else {
                    protocol.begin = p;
                    _decodeState = HttpRequestDecodeState::PROTOCOL;
                }
                break;
            }
            case HttpRequestDecodeState::PROTOCOL: {
                //解析请求协议
                if (ch == '/') {
                    protocol.end = p;
                    _protocol = protocol;
                    _decodeState = HttpRequestDecodeState::BEFORE_VERSION;
                } else {
                    //do nothing
                }
                break;
            }
            case HttpRequestDecodeState::BEFORE_VERSION: {
                if (isdigit(ch)) {
                    version.begin = p;
                    _decodeState = HttpRequestDecodeState::VERSION;
                } else {
                    _decodeState = HttpRequestDecodeState::INVALID_VERSION;
                }
                break;
            }
            case HttpRequestDecodeState::VERSION: {
                //协议解析，如果不是数字或者. 就不对
                if (ch == CR) {
                    version.end = p;
                    _version = version;
                    _decodeState = HttpRequestDecodeState::WHEN_CR;
                } else if (ch == '.') {
                    //遇到版本分割
                    _decodeState = HttpRequestDecodeState::VERSION_SPLIT;
                } else if (isdigit(ch)) {
                    //do nothing
                } else {
                    _decodeState = HttpRequestDecodeState::INVALID_VERSION;//不能不是数字
                }
                break;
            }
            case HttpRequestDecodeState::VERSION_SPLIT: {
                //遇到版本分割符，字符必须是数字，其他情况都是错误
                if (isdigit(ch)) {
                    _decodeState = HttpRequestDecodeState::VERSION;
                } else {
                    _decodeState = HttpRequestDecodeState::INVALID_VERSION;
                }
                break;
            }
            case HttpRequestDecodeState::HEADER_KEY: {
                //冒号前后可能有空格
                if (isblank(ch)) {
                    headerKey.end = p;
                    _decodeState = HttpRequestDecodeState::HEADER_BEFORE_COLON;//冒号之前的状态
                } else if (ch == ':') {
                    headerKey.end = p;
                    _decodeState = HttpRequestDecodeState::HEADER_AFTER_COLON;//冒号之后的状态
                } else {
                    //do nothing
                }
                break;
            }
            case HttpRequestDecodeState::HEADER_BEFORE_COLON: {
                if (isblank(ch)) {
                    //do nothing
                } else if (ch == ':') {
                    _decodeState = HttpRequestDecodeState::HEADER_AFTER_COLON;
                } else {
                    //冒号之前的状态不能是空格之外的其他字符
                    _decodeState = HttpRequestDecodeState::INVALID_HEADER;
                }
                break;
            }
            case HttpRequestDecodeState::HEADER_AFTER_COLON: {
                if (isblank(ch)) {//值之前遇到空格都是正常的
                    //do nothing
                } else {
                    headerValue.begin = p;
                    _decodeState = HttpRequestDecodeState::HEADER_VALUE;//开始处理值
                }
                break;
            }
            case HttpRequestDecodeState::HEADER_VALUE: {
                if (ch == CR) {
                    headerValue.end = p;
                    _headers.insert({headerKey, headerValue});
                    _decodeState = HttpRequestDecodeState::WHEN_CR;
                }
                break;
            }
            case HttpRequestDecodeState::WHEN_CR: {
                if (ch == LF) {
                    //如果是回车，可换成下一个
                    _decodeState = HttpRequestDecodeState::CR_LF;
                } else {
                    _decodeState = HttpRequestDecodeState::INVALID;
                }
                break;
            }
            case HttpRequestDecodeState::CR_LF: {
                if (ch == CR) {
                    //如果在CR_LF状态之后还有CR，那么便是有点结束的味道了
                    _decodeState = HttpRequestDecodeState::CR_LF_CR;
                } else if (isblank(ch)) {
                    _decodeState = HttpRequestDecodeState::INVALID;
                } else {
                    //如果不是，那么就可能又是一个请求头了，那就开始解析请求头
                    headerKey.begin = p;
                    _decodeState = HttpRequestDecodeState::HEADER_KEY;
                }
                break;
            }
            case HttpRequestDecodeState::CR_LF_CR: {
                if (ch == LF) {
                    //如果是\r接着\n 那么判断是不是需要解析请求体
                    if (_headers.count("Content-Length") > 0) {
                        bodyLength = atoi(_headers["Content-Length"].c_str());
                        if (bodyLength > 0) {
                            _decodeState = HttpRequestDecodeState::BODY;//解析请求体
                        } else {
                            _decodeState = HttpRequestDecodeState::COMPLETE;//完成了
                        }
                    } else {
                        if (scanPos < size) {
                            bodyLength = size - scanPos;
                            _decodeState = HttpRequestDecodeState::BODY;//解析请求体
                        } else {
                            _decodeState = HttpRequestDecodeState::COMPLETE;
                        }
                    }
                } else {
                    _decodeState = HttpRequestDecodeState::INVALID_HEADER;
                }
                break;
            }
            case HttpRequestDecodeState::BODY: {
                //解析请求体
                _body.assign(p, p + bodyLength);
                _decodeState = HttpRequestDecodeState::COMPLETE;
                break;
            }
            default:
                break;
        }
    }
}

const std::string &HttpRequest::getMethod() const {
    return _method;
}

const std::string &HttpRequest::getUrl() const {
    return _url;
}

const std::map<std::string, std::string> &HttpRequest::getRequestParams() const {
    return _requestParams;
}

const std::string &HttpRequest::getProtocol() const {
    return _protocol;
}

const std::string &HttpRequest::getVersion() const {
    return _version;
}

const std::map<std::string, std::string> &HttpRequest::getHeaders() const {
    return _headers;
}

const std::vector<char> &HttpRequest::getBody() const {
    return _body;
}

void HttpRequest::setData(const std::vector<char> *data) {
    _data = data;
}

void HttpRequest::tryDecode() {
    if (_data) {
        this->tryDecode(*_data);
    }
}
