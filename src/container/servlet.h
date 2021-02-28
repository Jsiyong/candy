//
// Created by Jsiyong on 2021-02-20.
//

#ifndef CANDY_SERVLET_H
#define CANDY_SERVLET_H

#include "../protocol/http/httprequest.h"
#include "../protocol/http/httpresponse.h"

/**
 * 用户应用的基类
 */
struct Servlet {

    virtual ~Servlet() = default;

    /**
     * 做服务的基类
     */
    void doService(HttpRequest *request, HttpResponse *response);

    /**
     * 责任链的下一个责任
     */
    void setNextServlet(Servlet *nextServlet);

    /**
     * servlet的服务接口
     * @param request 封装请求信息
     * @param response 封装响应信息
     * @return 返回是否能够处理
     */
    virtual bool service(HttpRequest *request, HttpResponse *response) = 0;

    Servlet *getNextServlet() const;

private:
    Servlet *_nextServlet = NULL;
};

/**
 * 分发请求
 */
struct StandardDispatcherServlet : public Servlet {
    bool service(HttpRequest *request, HttpResponse *response) override;
};


/**
 * 主要是处理文件路径的
 */
struct FileDispatcherServlet : public Servlet {
    bool service(HttpRequest *request, HttpResponse *response) override;
};

struct ErrorDispatcherServlet : public Servlet {
    bool service(HttpRequest *request, HttpResponse *response) override;
};

#endif //CANDY_SERVLET_H
