//
// Created by Jsiyong on 2021-02-22.
//

#include "../../container/controller.h"
#include "../../log/logger.h"
#include "../../util/fileutil.h"
#include "../service/fileservice.h"

struct HelloWorld {
    int code = 10;
    std::string msg = "hello-world";
    std::vector<int> ids = {1, 9, 2, 3, 4, 5};
};

Serialization(HelloWorld, code, msg, ids)

struct HomeController : public Controller<HomeController> {

    RequestMapping(home, "/")
    std::string home(HttpRequest *request, HttpResponse *response) {
        return "/index.html";
    }

    RequestMapping(name, "/getFolder")
    ResponseBody name(HttpRequest *request, HttpResponse *response) {
        auto params = request->getRequestParams();
        std::string path = params["path"];
        return FileService::getFolderByPath(path);
    }

    RequestMapping(hello, "/hello")
    ResponseBody hello(HttpRequest *request, HttpResponse *response) {
        return HelloWorld();
    }
};