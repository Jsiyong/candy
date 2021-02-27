//
// Created by Jsiyong on 2021-02-20.
//

#ifndef CANDY_HTTPRESPONSE_H
#define CANDY_HTTPRESPONSE_H

#include <vector>
#include <unordered_map>
#include "httpbase.h"

/**
 * Http的响应
 */
struct HttpResponse : public HttpBase {

    HttpResponse();

    void setNotFound();

    /**
     * 通过资源路径设置内容类型
     * @param uri 访问的资源路径
     */
    void setContentTypeByUri(const std::string &uri);

    /**
     * 对回复的内容进行编码
     */
    void encode(std::string &dst);

    void clear() override;

private:
    int _statusCode = 0;//状态码
    std::string _statusWord;//状态码描述

    const static std::unordered_map<std::string, std::string> _fileContentTypeMap;//存放文件和content-type返回类型的匹配
};

#endif //CANDY_HTTPRESPONSE_H
