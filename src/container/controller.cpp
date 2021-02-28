//
// Created by Jsiyong on 2021-02-22.
//

#include "controller.h"
#include "../log/logger.h"

bool EventDispatcher::registerHandler(const std::string &path, const EventDispatcher::StandardHandler &handler) {
    //已经注册过的不能再注册
    if (isContains(path)) {
        error("register error! the path[%] has been registered!!", path);
        return false;
    }
    this->_standardHandlerMap.emplace(path, handler);
    return true;
}

bool EventDispatcher::registerHandler(const std::string &path, const EventDispatcher::FileHandler &handler) {
    //已经注册过的不能再注册
    if (isContains(path)) {
        error("register error! the path[%] has been registered!!", path);
        return false;
    }
    this->_fileHandlerMap.emplace(path, handler);
    return true;
}

bool EventDispatcher::registerHandler(const std::string &path, const EventDispatcher::JsonHandler &handler) {
    //已经注册过的不能再注册
    if (isContains(path)) {
        error("register error! the path[%] has been registered!!", path);
        return false;
    }
    this->_jsonHandlerMap.emplace(path, handler);
    return true;
}


EventDispatcher::StandardHandler EventDispatcher::getStandardHandlerByPath(const std::string &path) {
    if (this->_standardHandlerMap.find(path) == _standardHandlerMap.end()) {
        return EventDispatcher::StandardHandler();
    }
    return this->_standardHandlerMap[path];
}

EventDispatcher::JsonHandler EventDispatcher::getJsonHandlerByPath(const std::string &path) {
    if (this->_jsonHandlerMap.find(path) == _jsonHandlerMap.end()) {
        return EventDispatcher::JsonHandler();
    }
    return this->_jsonHandlerMap[path];
}

EventDispatcher::FileHandler EventDispatcher::getFileHandlerByPath(const std::string &path) {
    if (this->_fileHandlerMap.find(path) == _fileHandlerMap.end()) {
        return EventDispatcher::FileHandler();
    }
    return this->_fileHandlerMap[path];
}

bool EventDispatcher::isContains(const std::string &path) {
    return this->_standardHandlerMap.find(path) != _standardHandlerMap.end()
           || this->_fileHandlerMap.find(path) != _fileHandlerMap.end()
           || this->_jsonHandlerMap.find(path) != _jsonHandlerMap.end();
}
