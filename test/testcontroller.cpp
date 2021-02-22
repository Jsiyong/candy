//
// Created by Jsiyong on 2021-02-21.
//
#include <iostream>
#include "../src/container/controller.h"

struct MyController : public Controller<MyController> {

    RequestMapping(pp, "/user/name")
    void pp(HttpRequest *request, HttpResponse *response) {
        std::cout << "get username..." << std::endl;
    }

    RequestMapping(pp2, "/user/index")
    void pp2(HttpRequest *request, HttpResponse *response) {
        std::cout << "go to index page..." << std::endl;
    }
};

int main() {

    EventDispatcher::getInstance()->getHandlerByPath("/user/index")(NULL, NULL);

    EventDispatcher::getInstance()->getHandlerByPath("/user/name")(NULL, NULL);

    return 0;
}
