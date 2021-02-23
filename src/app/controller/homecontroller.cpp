//
// Created by Jsiyong on 2021-02-22.
//

#include "../../container/controller.h"
#include "../../log/logger.h"

struct HomeController : public Controller<HomeController> {

    RequestMapping(index, "/home")
    void index(HttpRequest *request, HttpResponse *response) {
        trace("......home controller......");
        for (auto &p : request->getRequestParams()) {
            trace("[request params] key: %s, value: %s", p.first, p.second);
        }
        info("[body size] %lld", request->getBody().size());

    }

};