//
// Created by Jsiyong on 2021-03-14.
//
#include "urlutil.h"
#include "convertutil.h"
#include <string.h>

std::string UrlUtil::decode(const std::string &src) {
    std::string to;
    for (int i = 0; i < src.size(); ++i) {
        if (src[i] == '%' && i + 2 < src.size() && isxdigit(src[i + 1]) && isxdigit(src[i + 2])) {
            //依次判断src中的%20字符等三个字符
            to.push_back(ConvertUtil::convertHexitToDecimal(src[i + 1]) * 16 +
                         ConvertUtil::convertHexitToDecimal(src[i + 2]));
            //移过已经处理的两个字符(%21指针指向1),表达式3的++from还会再后移一个字符
            i += 2;
        } else {
            to.push_back(src[i]);
        }
    }
    return to;
}

std::string UrlUtil::encode(const std::string &src) {
    std::string to;
    char buf[5] = {0};
    for (int i = 0; i < src.size(); ++i) {
        //isalnum 判断字符是否是数字[不是中文不需要编码]
        ///_.-~这几个字符不需要转
        if (isalnum(src[i]) || strchr("/_.-~", src[i]) != (char *) NULL) {
            to.push_back(src[i]);
        } else {
            // %23等
            int size = sprintf(buf, "%%%02x", src[i] & 0xff);
            to.append(buf, size);
        }
    }
    return to;
}


