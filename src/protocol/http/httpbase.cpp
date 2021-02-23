//
// Created by Jsiyong on 2021-02-23.
//
#include "httpbase.h"

const std::string &HttpBase::getProtocol() const {
    return _protocol;
}

void HttpBase::setProtocol(const std::string &protocol) {
    _protocol = protocol;
}

const std::string &HttpBase::getVersion() const {
    return _version;
}

void HttpBase::setVersion(const std::string &version) {
    _version = version;
}

const std::map<std::string, std::string> &HttpBase::getHeaders() const {
    return _headers;
}

void HttpBase::setHeaders(const std::map<std::string, std::string> &headers) {
    _headers = headers;
}

const std::vector<char> &HttpBase::getBody() const {
    return _body;
}

void HttpBase::setBody(const std::vector<char> &body) {
    _body = body;
}

void HttpBase::clear() {

}

std::vector<char> &HttpBase::getBuffer() {
    return _buf;
}
