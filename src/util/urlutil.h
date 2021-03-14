//
// Created by Jsiyong on 2021-03-14.
//

#ifndef CANDY_URLUTIL_H
#define CANDY_URLUTIL_H

#include <string>

/**
 * url的工具类
 */
struct UrlUtil {
    /**
     * 解码url[可能有中文]
     * 编码的时候是十进制转十六进制，解码的时候就是十六进制转十进制
     * @param src
     * @return
     */
    static std::string decode(const std::string &src);


    /**
     * url编码
     * @param src
     * @return
     */
    static std::string encode(const std::string &src);

};

#endif //CANDY_URLUTIL_H
