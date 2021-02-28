//
// Created by Jsiyong on 2021-02-28.
//

#ifndef CANDY_RESPONSEBODY_H
#define CANDY_RESPONSEBODY_H

#include "../protocol/json/serialization.h"

/**
 * body中返回json数据
 */
struct ResponseBody {

    template<typename T>
    ResponseBody(const T &t) {
        _json = JsonParser::parse(Serializable<T>::serialize(t));
    }

    const std::string &getJson() const {
        return _json;
    }

private:
    std::string _json;
};

#endif //CANDY_RESPONSEBODY_H
