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

    RequestMapping(index, "/1.png")
    void index(HttpRequest *request, HttpResponse *response) {
        trace("......home controller......");
        for (auto &p : request->getRequestParams()) {
            trace("[request params] key: %s, value: %s", p.first, p.second);
        }
        for (auto &p:request->getHeaders()) {
            trace("[request headers] key: %s, value: %s", p.first, p.second);
        }
        info("[body size] %lld/KB, %lld/B", request->getBody().size() / 1024, request->getBody().size());

        //设置回复信息
#if 0
        HelloWorld helloWorld;
        auto jsonObject = Serializable<HelloWorld>::serialize(helloWorld);
        auto resp = JsonParser::parse(jsonObject);
#endif
//        response->setBody(R"({"hello":"world","my":"name","is":666})");
//        response->setHeader("Content-Type", "application/json; charset=UTF-8");

        response->setHeader("Content-Type", "application/octet-stream");
        FileUtil::readFile("/tmp/tmp.mkWXUL9TBY/cmake-build-debug/壁纸-机械.png", response->getBody());
    }

};