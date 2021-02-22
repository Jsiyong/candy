//
// Created by Jsiyong on 2021-02-20.
//

#include <string>
#include "httpresponse.h"
#include "../../log/logger.h"

HttpResponse::HttpResponse() {
#if 0
    char resp[MAX_BUFF_SIZE] = {0};
    char *p = resp;
    const char *header = "HTTP/1.1 200 OK\r\n";
    const char *linefmt = "Content-Type:%s\r\nContent-Length:%d\r\n";
    //body
    const char *body = R"({"title":"hello","body":"world"})";
    char line[MAX_BUFF_SIZE] = {0};
    sprintf(line, linefmt, "application/json; charset=UTF-8", strlen(body));

    //头部
    strcat(p, header);
    p += strlen(header);

    //键值对
    strcat(p, line);
    p += strlen(line);

    //换行
    strcat(p, "\r\n");
    p += strlen("\r\n");


    strcat(p, body);
    trace("response data:%s", resp);
    trace("response ok!!");
#endif

    std::string body = R"({"title":"hello","body":"world"})";
    std::string str = "HTTP/1.1 200 OK\r\n";
    str += "Content-Type:application/json; charset=UTF-8\r\nContent-Length:" + std::to_string(body.size()) + "\r\n";
    str += "\r\n";
    str += body;
    trace("response ok!!");
}

HttpResponse::operator std::vector<char> &() {
    return _buf;
}
