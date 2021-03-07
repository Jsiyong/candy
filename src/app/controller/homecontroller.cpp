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
    std::vector<HelloWorld> hello3;
};

Serialization(HelloWorld, code, msg, ids, hello3)

struct HomeController : public Controller<HomeController> {

    RequestMapping(home, "/")
    std::string home(HttpRequest *request, HttpResponse *response) {
        return "/index.html";
    }

    RequestMapping(name, "/getFolder")
    ResponseBody name(HttpRequest *request, HttpResponse *response) {
        auto params = request->getRequestParams();
        std::string path = params["path"];

        FolderVO folderVo;
        //递归获取
        FileService::getFolderByPath(folderVo, path, false);
        return folderVo;
    }

    RequestMapping(hello, "/hello")
    ResponseBody hello(HttpRequest *request, HttpResponse *response) {
        HelloWorld helloWorld;
        helloWorld.hello3.emplace_back();
        return helloWorld;
    }
};