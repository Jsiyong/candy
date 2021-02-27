//
// Created by Jsiyong on 2021-02-20.
//

#include <iostream>
#include "servlet.h"
#include "../container/controller.h"
#include "../log/logger.h"
#include "../util/fileutil.h"
#include "../conf/servconf.h"

void DispatcherServlet::service(HttpRequest *request, HttpResponse *response) {
    trace("[method] %s, [url] %s", request->getMethod(), request->getUrl());

    //根据路径获取对应的处理方法
    const std::string &url = request->getUrl();
    //根据请求路径获取业务处理方法的指针对象
    auto event = EventDispatcher::getInstance()->getHandlerByPath(url);
    if (event) {
        //调用业务处理方法
        event(request, response);
    } else {
        std::string requestURL = request->getUrl();
        if (requestURL == "/") {
            requestURL += "index.html";
        }

        //找出文件名
        std::string fileName = requestURL.substr(requestURL.rfind('/') + 1);
        //找到后缀
        std::string ext = fileName.substr(fileName.rfind('.') + 1);
        if (ext == "png") {
            response->setHeader("Content-Type", "image/png");
        } else if (ext == "html") {
            response->setHeader("Content-Type", "text/html");
        } else if (ext == "jpg") {
            response->setHeader("Content-Type", "image/jpeg");
        } else if (ext == "txt") {
            response->setHeader("Content-Type", "text/plain");
        }

        requestURL = serverConf.getWebRoot() + requestURL;
        FileUtil::readFile(requestURL, response->getBody());



//        //如果没有业务可以处理这个请求，那么请求返回错误
//        response->setNotFound();
    }
}
