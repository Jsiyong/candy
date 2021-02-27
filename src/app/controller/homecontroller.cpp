//
// Created by Jsiyong on 2021-02-22.
//

#include "../../container/controller.h"
#include "../../log/logger.h"
#include "../../protocol/json/serialization.h"
#include "../../util/fileutil.h"

struct HelloWorld {
    int code = 10;
    std::string msg = "hello-world";
    std::vector<int> ids = {1, 9, 2, 3, 4, 5};
};

Serialization(HelloWorld, code, msg, ids)

struct HomeController : public Controller<HomeController> {

    RequestMapping(index, "/123.png")
    void index(HttpRequest *request, HttpResponse *response) {
#if 0
        trace("......home controller......");
        for (auto &p : request->getRequestParams()) {
            trace("[request params] %s: %s", p.first, p.second);
        }
        for (auto &p:request->getHeaders()) {
            trace("[request headers] %s: %s", p.first, p.second);
        }
//        info("[body size] %lld/KB, %lld/B", request->getBody().size() / 1024, request->getBody().size());

        //设置回复信息

        HelloWorld helloWorld;
        auto jsonObject = Serializable<HelloWorld>::serialize(helloWorld);
        auto resp = JsonParser::parse(jsonObject);
#endif
        response->setHeader("Content-Type", "image/png");
        FileUtil::readFile("/tmp/tmp.mkWXUL9TBY/cmake-build-debug/b.png", response->getBody());
    }

};