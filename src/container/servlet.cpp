//
// Created by Jsiyong on 2021-02-20.
//

#include <iostream>
#include "servlet.h"
#include "../container/controller.h"
#include "../log/logger.h"
#include "../util/fileutil.h"
#include "../conf/servconf.h"

bool StandardDispatcherServlet::service(HttpRequest *request, HttpResponse *response) {
    trace("[request] %s %s", request->getMethod(), request->getUrl());

    //根据路径获取对应的处理方法
    const std::string &url = request->getUrl();
    //根据请求路径获取业务处理方法的指针对象
    auto event = EventDispatcher::getInstance()->getHandlerByPath(url);
    if (!event) {
        return false;
    }
    //调用业务处理方法
    event(request, response);
}

void Servlet::setNextServlet(Servlet *nextServlet) {
    _nextServlet = nextServlet;
}

void Servlet::doService(HttpRequest *request, HttpResponse *response) {
    if (service(request, response)) {
        //如果当前服务已经处理成功，直接结束
        return;
    }
    //如果当前服务无法处理，交给下一个服务处理
    if (_nextServlet) {
        _nextServlet->doService(request, response);
    }
}

Servlet *Servlet::getNextServlet() const {
    return _nextServlet;
}

bool FileDispatcherServlet::service(HttpRequest *request, HttpResponse *response) {
    //判断路径是不是网站的公共路径中的某个路径,如果是，就之际防护数据
    std::string resourcePath = serverConf.getWebRoot() + request->getUrl();
    if (!FileUtil::isRegularFile(resourcePath)) {
        return false;
    }
    response->setContentTypeByUri(request->getUrl());
    FileUtil::readFile(resourcePath, response->getBody());
    return true;
}

bool ErrorDispatcherServlet::service(HttpRequest *request, HttpResponse *response) {
    //错误的时候，显示文件没找到
    response->setNotFound();
    return true;
}
