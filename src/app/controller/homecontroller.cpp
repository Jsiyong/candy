//
// Created by Jsiyong on 2021-02-22.
//

#include "../../container/controller.h"
#include "../../log/logger.h"
#include "../../protocol/json/serialization.h"

struct HelloWorld {
    int code = 10;
    std::string msg = "hello-world";
    std::vector<int> ids = {1, 9, 2, 3, 4, 5};
};

Serialization(HelloWorld, code, msg, ids)

struct HomeController : public Controller<HomeController> {

    RequestMapping(index, "/home")
    void index(HttpRequest *request, HttpResponse *response) {
        trace("......home controller......");
        for (auto &p : request->getRequestParams()) {
            trace("[request params] key: %s, value: %s", p.first, p.second);
        }
        for (auto &p:request->getHeaders()) {
            trace("[request headers] key: %s, value: %s", p.first, p.second);
        }
        info("[body size/KB] %lld", request->getBody().size() / 1024);

        //设置回复信息
        HelloWorld helloWorld;
        auto jsonObject = Serializable<HelloWorld>::serialize(helloWorld);
        auto resp = JsonParser::parse(jsonObject);
        response->setBody(resp);
        response->setHeader("Content-Type", "application/json; charset=UTF-8");
    }

};