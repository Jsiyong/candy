//
// Created by Jsiyong on 2021-02-28.
//

#ifndef CANDY_PIPELINE_H
#define CANDY_PIPELINE_H

#include "./servlet.h"

/**
 * 请求处理的通道
 */
struct Pipeline {
    Pipeline();

    ~Pipeline();

    Servlet *getServlet();

private:
    Servlet *_servlet = NULL;
};

#endif //CANDY_PIPELINE_H
