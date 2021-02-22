//
// Created by Jsiyong on 2021-02-22.
//

#include "controller.h"
#include "../log/logger.h"

bool EventDispatcher::registerPathHandler(const std::string &path, const EventDispatcher::PathHandler &pathHandler) {
    //已经注册过的不能再注册
    if (this->_pathHandlerMap.find(path) != _pathHandlerMap.end()) {
        error("register error! the path[%] has been registered!!", path);
        return false;
    }
    this->_pathHandlerMap.emplace(path, pathHandler);
    return true;
}

EventDispatcher::PathHandler EventDispatcher::getHandlerByPath(const std::string &path) {
    if (this->_pathHandlerMap.find(path) == _pathHandlerMap.end()) {
        return EventDispatcher::PathHandler();
    }
    return this->_pathHandlerMap[path];
}
