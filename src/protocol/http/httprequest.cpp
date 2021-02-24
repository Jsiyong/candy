//
// Created by jiangsiyong on 2020/12/23.
//

#include "httprequest.h"

#include <string.h>
#include <vector>

#define CR '\r'
#define LF '\n'

/**
 * Http请求行的状态
 */
enum HttpRequestDecodeState {
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

void HttpRequest::tryDecode() {
    if (_buf.empty()) {
        return;
    }
    if (_decodeState == HttpRequestDecodeState::COMPLETE) {
        //如果解析完成，则再解析一遍
        this->clear();
    }
    this->parseInternal(_buf.data() + _nextPos, _buf.size());
#if 0
    //如果是长连接，可能会出现上一个请求解析完了，下面会解析下一个请求，所以清空之前解析玩的数据
    buf.assign(buf.begin() + _nextPos, buf.end());
#endif
}

//解析请求行
void HttpRequest::parseInternal(char *buf, size_t end) {

    while (_decodeState != HttpRequestDecodeState::INVALID
           && _decodeState != HttpRequestDecodeState::COMPLETE
           && _nextPos < end) {

        _nextPos++;//比较完之后下一个指针往后偏移
        char ch = *buf;//当前的字符
        char *p = buf++;//指针偏移

        switch (_decodeState) {
            case HttpRequestDecodeState::START: {
                //空格 换行 回车都继续
                if (ch == CR || ch == LF || isblank(ch)) {
                    //do nothing
                } else if (isupper(ch)) {//判断是不是大写字符，不是的话是无效的
                    _method.assign(1, ch);//方法的起始点

                    _decodeState = HttpRequestDecodeState::METHOD;//如果遇到第一个字符，开始解析方法
                } else {
                    _decodeState = HttpRequestDecodeState::INVALID;
                }
                break;
            }
            case HttpRequestDecodeState::METHOD: {
                //方法需要大写字母，大写字母就继续
                if (isupper(ch)) {
                    _method.push_back(ch);
                } else if (isblank(ch)) {//空格，说明方法解析结束，下一步开始解析URI
                    _decodeState = HttpRequestDecodeState::BEFORE_URI;
                } else {
                    _decodeState = HttpRequestDecodeState::INVALID;//其他情况是无效的请求方法
                }
                break;
            }
            case HttpRequestDecodeState::BEFORE_URI: {
                //请求连接前的处理，需要'/'开头
                if (ch == '/') {
                    _url.assign(1, ch);
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
                    _decodeState = HttpRequestDecodeState::BEFORE_URI_PARAM_KEY;
                } else if (isblank(ch)) {//遇到空格，请求路径解析完成，开始解析协议
                    _decodeState = HttpRequestDecodeState::BEFORE_PROTOCOL;
                } else {
                    _url.push_back(ch);
                }
                break;
            }
            case HttpRequestDecodeState::BEFORE_URI_PARAM_KEY: {
                if (isblank(ch) || ch == LF || ch == CR) {
                    _decodeState = HttpRequestDecodeState::INVALID;
                } else {
                    _requestParamKey.assign(1, ch);
                    _decodeState = HttpRequestDecodeState::URI_PARAM_KEY;
                }
                break;
            }
            case HttpRequestDecodeState::URI_PARAM_KEY: {
                if (ch == '=') {
                    _decodeState = HttpRequestDecodeState::BEFORE_URI_PARAM_VALUE;//开始解析参数值
                } else if (isblank(ch)) {
                    _decodeState = HttpRequestDecodeState::INVALID;//无效的请求参数
                } else {
                    _requestParamKey.push_back(ch);
                }
                break;
            }
            case HttpRequestDecodeState::BEFORE_URI_PARAM_VALUE: {
                if (isblank(ch) || ch == LF || ch == CR) {
                    _decodeState = HttpRequestDecodeState::INVALID;
                } else {
                    _requestParamValue.assign(1, ch);
                    _decodeState = HttpRequestDecodeState::URI_PARAM_VALUE;
                }
                break;
            }
            case HttpRequestDecodeState::URI_PARAM_VALUE: {
                if (ch == '&') {
                    //收获一个请求参数
                    _requestParams.insert({_requestParamKey, _requestParamValue});
                    _decodeState = HttpRequestDecodeState::BEFORE_URI_PARAM_KEY;
                } else if (isblank(ch)) {
                    //空格也收获一个请求参数
                    _requestParams.insert({_requestParamKey, _requestParamValue});
                    _decodeState = HttpRequestDecodeState::BEFORE_PROTOCOL;
                } else {
                    _requestParamValue.push_back(ch);
                }
                break;
            }
            case HttpRequestDecodeState::BEFORE_PROTOCOL: {
                if (isblank(ch)) {
                    //do nothing
                } else {
                    _protocol.assign(1, ch);
                    _decodeState = HttpRequestDecodeState::PROTOCOL;
                }
                break;
            }
            case HttpRequestDecodeState::PROTOCOL: {
                //解析请求协议
                if (ch == '/') {
                    _decodeState = HttpRequestDecodeState::BEFORE_VERSION;
                } else {
                    _protocol.push_back(ch);
                }
                break;
            }
            case HttpRequestDecodeState::BEFORE_VERSION: {
                if (isdigit(ch)) {
                    _version.assign(1, ch);
                    _decodeState = HttpRequestDecodeState::VERSION;
                } else {
                    _decodeState = HttpRequestDecodeState::INVALID;
                }
                break;
            }
            case HttpRequestDecodeState::VERSION: {
                //协议解析，如果不是数字或者. 就不对
                if (ch == CR) {
                    _decodeState = HttpRequestDecodeState::WHEN_CR;
                } else if (ch == '.') {
                    //遇到版本分割
                    _decodeState = HttpRequestDecodeState::VERSION_SPLIT;
                } else if (isdigit(ch)) {
                    _version.push_back(ch);
                } else {
                    _decodeState = HttpRequestDecodeState::INVALID;//不能不是数字
                }
                break;
            }
            case HttpRequestDecodeState::VERSION_SPLIT: {
                //遇到版本分割符，字符必须是数字，其他情况都是错误
                if (isdigit(ch)) {
                    _decodeState = HttpRequestDecodeState::VERSION;
                } else {
                    _decodeState = HttpRequestDecodeState::INVALID;
                }
                break;
            }
            case HttpRequestDecodeState::HEADER_KEY: {
                //冒号前后可能有空格
                if (isblank(ch)) {
                    _decodeState = HttpRequestDecodeState::HEADER_BEFORE_COLON;//冒号之前的状态
                } else if (ch == ':') {
                    _decodeState = HttpRequestDecodeState::HEADER_AFTER_COLON;//冒号之后的状态
                } else {
                    _headerKey.push_back(ch);
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
                    _decodeState = HttpRequestDecodeState::INVALID;
                }
                break;
            }
            case HttpRequestDecodeState::HEADER_AFTER_COLON: {
                if (isblank(ch)) {//值之前遇到空格都是正常的
                    //do nothing
                } else {
                    _headerValue.assign(1, ch);
                    _decodeState = HttpRequestDecodeState::HEADER_VALUE;//开始处理值
                }
                break;
            }
            case HttpRequestDecodeState::HEADER_VALUE: {
                if (ch == CR) {
                    _headers.insert({_headerKey, _headerValue});
                    _decodeState = HttpRequestDecodeState::WHEN_CR;
                }
                _headerValue.push_back(ch);
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
                    _headerKey.assign(1, ch);
                    _decodeState = HttpRequestDecodeState::HEADER_KEY;
                }
                break;
            }
            case HttpRequestDecodeState::CR_LF_CR: {
                if (ch == LF) {
                    //如果是\r接着\n 那么判断是不是需要解析请求体
                    if (_headers.count("Content-Length") > 0) {
                        _contentLength = atoll(_headers["Content-Length"].c_str());
                        if (_contentLength > 0) {
                            _body.reserve(_contentLength);//预分配body的大小,提高下面构造数组的效率
                            _decodeState = HttpRequestDecodeState::BODY;//解析请求体
                        } else {
                            _decodeState = HttpRequestDecodeState::COMPLETE;
                        }
                    } else {
                        _decodeState = HttpRequestDecodeState::COMPLETE;
                    }
                } else {
                    _decodeState = HttpRequestDecodeState::INVALID;
                }
                break;
            }
            case HttpRequestDecodeState::BODY: {
                //解析请求体
                _body.push_back(ch);
                if (_body.size() < _contentLength) {
                    //如果还没满足大小要求，则继续解析请求体
                } else {
                    _decodeState = HttpRequestDecodeState::COMPLETE;
                }
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

void HttpRequest::clear() {
    _method.clear();//请求方法

    _url.clear();//请求路径[不包含请求参数]

    _requestParams.clear();//请求参数

    _protocol.clear();//协议
    _version.clear();//版本

    _headers.clear();//所有的请求头

    _body.clear();//请求体

//    _nextPos = 0;//下一个位置的..这个不能清除，否则解析出错
    _contentLength = 0;//body的长度

    _decodeState = HttpRequestDecodeState::START;//解析状态
}

bool HttpRequest::completed() const {
    return _decodeState == HttpRequestDecodeState::COMPLETE;
}

HttpRequest::HttpRequest() {
    _decodeState = HttpRequestDecodeState::START;
}
