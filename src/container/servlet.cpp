//
// Created by Jsiyong on 2021-02-20.
//

#include <iostream>
#include "servlet.h"
#include "../container/controller.h"
#include "../log/logger.h"

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
//        for (auto &p:request->getHeaders()) {
//            trace("[request headers] %s: %s", p.first, p.second);
//        }
        //如果没有业务可以处理这个请求，那么请求返回错误
        response->setNotFound();
    }
}
