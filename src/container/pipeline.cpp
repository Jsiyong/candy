//
// Created by Jsiyong on 2021-02-28.
//

#include "pipeline.h"

Servlet *Pipeline::getServlet() {
    return _servlet;
}

Pipeline::Pipeline() {
    //首先先进行路径处理，有那个路径，就直接处理了
    _servlet = new FileDispatcherServlet();
    //然后是路径分发处理
    auto pStandardDispatcherServlet = new StandardDispatcherServlet();
    //最后是错误页处理
    auto pErrorDispatcherServlet = new ErrorDispatcherServlet();

    _servlet->setNextServlet(pStandardDispatcherServlet);
    pStandardDispatcherServlet->setNextServlet(pErrorDispatcherServlet);
}

Pipeline::~Pipeline() {
    //循环删除责任链
    Servlet *pre = NULL;//上一个删除的节点
    Servlet *next = _servlet;//下一个需要被删除的节点
    while (next != NULL) {
        pre = next;
        next = next->getNextServlet();
        delete pre;
    }
    _servlet = NULL;
}

