//
// Created by Jsiyong on 2021-03-04.
//

#ifndef CANDY_RESULTVO_H
#define CANDY_RESULTVO_H

#include <string>
#include "../../protocol/json/serialization.h"

/**
 * 结果集存放地方
 */
struct ResultVO {

    ResultVO(int code, const std::string &msg) : code(code), msg(msg) {}

    ResultVO() {};

    int code = 0;
    std::string msg;
};

Serialization(ResultVO, code, msg)

#endif //CANDY_RESULTVO_H
