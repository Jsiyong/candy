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
    EventDispatcher *pDispatcher = EventDispatcher::getInstance();
    if (!pDispatcher->isContains(url)) {
        return false;
    }
    //判断是不是在标准的处理中
    const auto &standardHandler = pDispatcher->getStandardHandlerByPath(url);
    if (standardHandler) {
        standardHandler(request, response);
        return true;
    }

    //判断是不是在路径处理的map中
    const auto &fileHandler = pDispatcher->getFileHandlerByPath(url);
    if (fileHandler) {
        //如果是文件的handler
        std::string filePath = fileHandler(request, response);
        readFileToResponseBodyByUrl(filePath, response);
        return true;
    }

    //不在路径处理的map中，在看看是不是最后在json处理map中
    const auto &jsonHandler = pDispatcher->getJsonHandlerByPath(url);
    if (jsonHandler) {
        response->setHeader("Content-Type", "application/json;charset=UTF-8");
        response->getBody().assign(jsonHandler(request, response).getJson());
        return true;
    }
    //返回下一个环节
    return false;
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

bool Servlet::readFileToResponseBodyByUrl(const std::string &url, HttpResponse *response) {
    //判断路径是不是网站的公共路径中的某个路径,如果是，就之际防护数据
    std::string resourcePath = serverConf.getAbsoulteWebRoot() + url;
    if (!FileUtil::isRegularFile(resourcePath)) {
        return false;
    }
    response->setContentTypeByUri(url);
    FileUtil::readFile(resourcePath, response->getBody());
    return true;
}

bool FileDispatcherServlet::service(HttpRequest *request, HttpResponse *response) {
    return readFileToResponseBodyByUrl(request->getUrl(), response);
}

bool ErrorDispatcherServlet::service(HttpRequest *request, HttpResponse *response) {
    //错误的时候，显示文件没找到
    response->setNotFound();
    return true;
}
