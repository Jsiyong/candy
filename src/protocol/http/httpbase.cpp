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

std::string &HttpBase::getBody() {
    return _body;
}

void HttpBase::clear() {
    _version.clear();
    _protocol.clear();
    _headers.clear();
    _body.clear();
}

void HttpBase::setHeader(const std::string &key, const std::string &value) {
    _headers[key] = value;
}

std::string HttpBase::getHeader(const std::string &key) const {
    auto it = _headers.find(key);
    if (it == _headers.end()) {
        return "";
    }
    return it->second;
}
