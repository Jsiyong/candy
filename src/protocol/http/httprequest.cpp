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
enum DecodeState {
    INVALID = 0,//无效

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

void HttpRequest::tryDecode(const std::string &src) {
    //解析完成或者buf为空都直接返回
    if (src.empty() || completed()) {
        return;
    }

    size_t i = 0;
    for (; i < src.size() && _decodeState != DecodeState::INVALID && _decodeState != DecodeState::COMPLETE; ++i) {
        char ch = src.at(i);//当前的字符
        switch (_decodeState) {
            case DecodeState::START: {
                //空格 换行 回车都继续
                if (ch == CR || ch == LF || isblank(ch)) {
                    //do nothing
                } else if (isupper(ch)) {//判断是不是大写字符，不是的话是无效的
                    _method.assign(1, ch);//方法的起始点

                    _decodeState = DecodeState::METHOD;//如果遇到第一个字符，开始解析方法
                } else {
                    _decodeState = DecodeState::INVALID;
                }
                break;
            }
            case DecodeState::METHOD: {
                //方法需要大写字母，大写字母就继续
                if (isupper(ch)) {
                    _method.push_back(ch);
                } else if (isblank(ch)) {//空格，说明方法解析结束，下一步开始解析URI
                    _decodeState = DecodeState::BEFORE_URI;
                } else {
                    _decodeState = DecodeState::INVALID;//其他情况是无效的请求方法
                }
                break;
            }
            case DecodeState::BEFORE_URI: {
                //请求连接前的处理，需要'/'开头
                if (ch == '/') {
                    _url.assign(1, ch);
                    _decodeState = DecodeState::IN_URI;//下一步就是开始处理连接
                } else if (isblank(ch)) {
                    //do nothing
                } else {
                    _decodeState = DecodeState::INVALID;//无效的
                }
                break;
            }
            case DecodeState::IN_URI: {
                //开始处理请求路径的字符串
                if (ch == '?') {//转为处理请求的key值
                    _decodeState = DecodeState::BEFORE_URI_PARAM_KEY;
                } else if (isblank(ch)) {//遇到空格，请求路径解析完成，开始解析协议
                    _decodeState = DecodeState::BEFORE_PROTOCOL;
                } else {
                    _url.push_back(ch);
                }
                break;
            }
            case DecodeState::BEFORE_URI_PARAM_KEY: {
                if (isblank(ch) || ch == LF || ch == CR) {
                    _decodeState = DecodeState::INVALID;
                } else {
                    _requestParamKey.assign(1, ch);
                    _decodeState = DecodeState::URI_PARAM_KEY;
                }
                break;
            }
            case DecodeState::URI_PARAM_KEY: {
                if (ch == '=') {
                    _decodeState = DecodeState::BEFORE_URI_PARAM_VALUE;//开始解析参数值
                } else if (isblank(ch)) {
                    _decodeState = DecodeState::INVALID;//无效的请求参数
                } else {
                    _requestParamKey.push_back(ch);
                }
                break;
            }
            case DecodeState::BEFORE_URI_PARAM_VALUE: {
                if (isblank(ch) || ch == LF || ch == CR) {
                    _decodeState = DecodeState::INVALID;
                } else {
                    _requestParamValue.assign(1, ch);
                    _decodeState = DecodeState::URI_PARAM_VALUE;
                }
                break;
            }
            case DecodeState::URI_PARAM_VALUE: {
                if (ch == '&') {
                    //收获一个请求参数
                    _requestParams.insert({_requestParamKey, _requestParamValue});
                    _decodeState = DecodeState::BEFORE_URI_PARAM_KEY;
                } else if (isblank(ch)) {
                    //空格也收获一个请求参数
                    _requestParams.insert({_requestParamKey, _requestParamValue});
                    _decodeState = DecodeState::BEFORE_PROTOCOL;
                } else {
                    _requestParamValue.push_back(ch);
                }
                break;
            }
            case DecodeState::BEFORE_PROTOCOL: {
                if (isblank(ch)) {
                    //do nothing
                } else {
                    _protocol.assign(1, ch);
                    _decodeState = DecodeState::PROTOCOL;
                }
                break;
            }
            case DecodeState::PROTOCOL: {
                //解析请求协议
                if (ch == '/') {
                    _decodeState = DecodeState::BEFORE_VERSION;
                } else {
                    _protocol.push_back(ch);
                }
                break;
            }
            case DecodeState::BEFORE_VERSION: {
                if (isdigit(ch)) {
                    _version.assign(1, ch);
                    _decodeState = DecodeState::VERSION;
                } else {
                    _decodeState = DecodeState::INVALID;
                }
                break;
            }
            case DecodeState::VERSION: {
                //协议解析，如果不是数字或者. 就不对
                if (ch == CR) {
                    _decodeState = DecodeState::WHEN_CR;
                } else if (ch == '.') {
                    //遇到版本分割
                    _version.push_back(ch);
                    _decodeState = DecodeState::VERSION_SPLIT;
                } else if (isdigit(ch)) {
                    _version.push_back(ch);
                } else {
                    _decodeState = DecodeState::INVALID;//不能不是数字
                }
                break;
            }
            case DecodeState::VERSION_SPLIT: {
                //遇到版本分割符，字符必须是数字，其他情况都是错误
                if (isdigit(ch)) {
                    _version.push_back(ch);
                    _decodeState = DecodeState::VERSION;
                } else {
                    _decodeState = DecodeState::INVALID;
                }
                break;
            }
            case DecodeState::HEADER_KEY: {
                //冒号前后可能有空格
                if (isblank(ch)) {
                    _decodeState = DecodeState::HEADER_BEFORE_COLON;//冒号之前的状态
                } else if (ch == ':') {
                    _decodeState = DecodeState::HEADER_AFTER_COLON;//冒号之后的状态
                } else {
                    _headerKey.push_back(ch);
                }
                break;
            }
            case DecodeState::HEADER_BEFORE_COLON: {
                if (isblank(ch)) {
                    //do nothing
                } else if (ch == ':') {
                    _decodeState = DecodeState::HEADER_AFTER_COLON;
                } else {
                    //冒号之前的状态不能是空格之外的其他字符
                    _decodeState = DecodeState::INVALID;
                }
                break;
            }
            case DecodeState::HEADER_AFTER_COLON: {
                if (isblank(ch)) {//值之前遇到空格都是正常的
                    //do nothing
                } else {
                    _headerValue.assign(1, ch);
                    _decodeState = DecodeState::HEADER_VALUE;//开始处理值
                }
                break;
            }
            case DecodeState::HEADER_VALUE: {
                if (ch == CR) {
                    _headers.insert({_headerKey, _headerValue});
                    _decodeState = DecodeState::WHEN_CR;
                }
                _headerValue.push_back(ch);
                break;
            }
            case DecodeState::WHEN_CR: {
                if (ch == LF) {
                    //如果是回车，可换成下一个
                    _decodeState = DecodeState::CR_LF;
                } else {
                    _decodeState = DecodeState::INVALID;
                }
                break;
            }
            case DecodeState::CR_LF: {
                if (ch == CR) {
                    //如果在CR_LF状态之后还有CR，那么便是有点结束的味道了
                    _decodeState = DecodeState::CR_LF_CR;
                } else if (isblank(ch)) {
                    _decodeState = DecodeState::INVALID;
                } else {
                    //如果不是，那么就可能又是一个请求头了，那就开始解析请求头
                    _headerKey.assign(1, ch);
                    _decodeState = DecodeState::HEADER_KEY;
                }
                break;
            }
            case DecodeState::CR_LF_CR: {
                if (ch == LF) {
                    //如果是\r接着\n 那么判断是不是需要解析请求体
                    if (_headers.count("Content-Length") > 0) {
                        _contentLength = atoll(_headers["Content-Length"].c_str());
                        if (_contentLength > 0) {
                            _body.reserve(_contentLength);//预分配body的大小,提高下面构造数组的效率
                            _decodeState = DecodeState::BODY;//解析请求体
                        } else {
                            _decodeState = DecodeState::COMPLETE;
                        }
                    } else {
                        _decodeState = DecodeState::COMPLETE;
                    }
                } else {
                    _decodeState = DecodeState::INVALID;
                }
                break;
            }
            case DecodeState::BODY: {
                //解析请求体
                _body.push_back(ch);
                if (_body.size() < _contentLength) {
                    //如果还没满足大小要求，则继续解析请求体
                } else {
                    _decodeState = DecodeState::COMPLETE;
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
    HttpBase::clear();//基类的删除

    _method.clear();//请求方法

    _url.clear();//请求路径[不包含请求参数]

    _requestParams.clear();//请求参数

    _contentLength = 0;//body的长度
    _decodeState = DecodeState::START;//解析状态
}

bool HttpRequest::completed() const {
    return _decodeState == DecodeState::COMPLETE;
}

HttpRequest::HttpRequest() {
    _decodeState = DecodeState::START;
}

bool HttpRequest::valid() const {
    //状态不为无效便是有效
    return _decodeState != DecodeState::INVALID;
}
