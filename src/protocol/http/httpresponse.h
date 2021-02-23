//
// Created by Jsiyong on 2021-02-20.
//

#ifndef CANDY_HTTPRESPONSE_H
#define CANDY_HTTPRESPONSE_H

#include <vector>
#include "httpbase.h"

/**
 * Http的响应
 */
struct HttpResponse : public HttpBase {

    HttpResponse();

    operator std::vector<char> &();

    /**
     * 对回复的内容进行编码
     */
    void encode();

private:
    int _statusCode = 0;//状态码
    std::string _statusWord;//状态码描述
};

#endif //CANDY_HTTPRESPONSE_H
