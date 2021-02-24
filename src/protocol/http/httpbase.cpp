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

const std::string &HttpBase::getBody() const {
    return _body;
}

void HttpBase::setBody(const std::string &body) {
    _body = body;
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
