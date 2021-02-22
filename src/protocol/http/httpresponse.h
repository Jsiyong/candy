//
// Created by Jsiyong on 2021-02-20.
//

#ifndef CANDY_HTTPRESPONSE_H
#define CANDY_HTTPRESPONSE_H

#include <vector>

/**
 * Http的响应
 */
struct HttpResponse {

    HttpResponse();

    operator std::vector<char> &();

private:
    std::vector<char> _buf;
};

#endif //CANDY_HTTPRESPONSE_H
