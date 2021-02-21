//
// Created by Jsiyong on 2021-02-21.
//
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

struct HttpServletRequest {
};
struct HttpServletResponse {
};

static std::unordered_map<std::string,
        std::function<void(HttpServletRequest *request, HttpServletResponse *response)>> funcPathMap;

//////////////////////////////////////////////////////////
template<typename T>
struct BaseController {

    static struct AutoInit {
        void doNothing() const {}

        explicit AutoInit() {
            //单实例
            static T t;
        }
    } init;

    struct PathFunction {
        PathFunction(const std::string &path,
                     const std::function<void(HttpServletRequest *request, HttpServletResponse *response)> &func) {
            funcPathMap.emplace(path, func);
        }
    };
};

template<typename T>
typename BaseController<T>::AutoInit BaseController<T>::init;
//////////////////////////////////////////////////////////

#define M_CONCAT_(a, b) a##b
#define M_CONCAT(a, b) M_CONCAT_(a, b)
#define RequestMapping(func, url) \
const PathFunction M_CONCAT(p, __LINE__) = { url,[this](HttpServletRequest *request, HttpServletResponse *response) {init.doNothing();this->func(request,response);} };\
template<bool B = true>\



struct MyController : public BaseController<MyController> {

    RequestMapping(pp, "user/name")
    void pp(HttpServletRequest *request, HttpServletResponse *response) {
        std::cout << "get username..." << std::endl;
    }

    RequestMapping(pp2, "/user/index")
    void pp2(HttpServletRequest *request, HttpServletResponse *response) {
        std::cout << "go to index page..." << std::endl;
    }
};

int main() {

    for (auto &c : funcPathMap) {
        std::cout << c.first << std::endl;
        c.second(NULL, NULL);
        std::cout << std::endl;
    }

    return 0;
}
