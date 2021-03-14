//
// Created by Jsiyong on 2021-03-13.
//

#ifndef CANDY_HTTPMULTIPART_H
#define CANDY_HTTPMULTIPART_H

#include <string>
#include <map>
#include <vector>
#include "httprequest.h"

struct FormDataItem {
    std::map<std::string, std::string> header;//表单数据他的头
    std::string body;//表单携带的数据体
};

/**
 * mutipart格式的
 */
struct HttpMultiPart {

    /**
     * 解析ContentDisposition值
     * @param contentDisposition
     * @return
     */
    static std::map<std::string, std::string> parseContentDisposition(const std::string &contentDisposition);

    /**
     * 从httpRequest转换过来
     * @param request
     */
    void fromHttpRequest(HttpRequest *request);

    /**
     * 解析mutipart格式
     * @param src
     */
    void tryDecode(const std::string &src);

    /**
     * 设置分割线
     * @param boundary
     */
    void setBoundary(const std::string &boundary);

    bool completed();

    const std::vector<FormDataItem> &getFormData() const;

private:
    /**
     * 判断ch是不是匹配当前Boundary字符
     * @param ch
     * @return
     */
    bool matchBoundaryChar(char ch);

    /**
     * 是不是分割符的最终结束状态
     * @return
     */
    bool isBoundaryFinallyEnd();

    /**
     * 是不是一个完整的分割符
     * @return
     */
    bool isFullBoundary();

private:


    std::string _boundary;//分割线的值
    int _decodeState = 0;//解码状态

    std::string _matchedBoundaryBuffer;//保存已经匹配到的分割线的buffer

    std::vector<FormDataItem> _formData;//表单数据项

    std::string _headerKey;//保存键
    std::string _headerValue;//保存值

};

#endif //CANDY_HTTPMULTIPART_H
