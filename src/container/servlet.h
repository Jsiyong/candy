//
// Created by Jsiyong on 2021-02-20.
//

#ifndef CANDY_SERVLET_H
#define CANDY_SERVLET_H

#include "../protocol/http/httprequest.h"

/**
 * 用户应用的基类
 */
struct Servlet {
    /**
     * servlet的服务接口
     * @param request 封装请求信息
     * @param response 封装响应信息
     */
    virtual void service(HttpRequest *request, HttpResponse *response) = 0;
};

/**
 * 分发请求
 */
struct DispatcherServlet : public Servlet {
    void service(HttpRequest *request, HttpResponse *response) override;
};

#endif //CANDY_SERVLET_H
