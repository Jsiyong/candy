//
// Created by Jsiyong on 2021-02-20.
//

#include <iostream>
#include "servlet.h"
#include "../log/logger.h"

void DispatcherServlet::service(HttpRequest *request, HttpResponse *response) {

    info("[method]%s", request->getMethod());
    info("[url]%s", request->getUrl());
    info("[request params]");
    for (auto &p : request->getRequestParams()) {
        trace("++ key: %s", p.first);
        trace("-- value: %s", p.second);
    }

    info("[protocol]%s", request->getProtocol());
    info("[version]%s", request->getVersion());
    info("[request headers]");
    for (auto &h : request->getHeaders()) {
        trace("++ key: %s", h.first);
        trace("-- value: %s", h.second);
    }
    info("[body]%s", request->getBody());
    warn("[body size]%lld", request->getBody().size());
}
