# Linux / macOS C++ HTTP 服务器（epoll / kqueue）
项目后端使用C++开发语言，以 Linux 为原始开发环境，基于边缘非阻塞 I/O（Linux 上为 epoll，macOS 上为 kqueue）实现 HTTP 服务器。从 http 协议的解析，到表单 multipart/form-data 协议的解析，再到 JSON 协议的解析，再到 C++ 对象的序列化，一步一个脚印实现一个 http 服务器。实现的功能有文件的上传、下载、删除、改名等。

前端采用vue框架，为了简单，没有使用node.js，使用httpVueLoader来解析vue文件，因为前端不是重点。

* [效果展示](#效果展示)
* [开发环境](#开发环境)
* [线程模型](#线程模型)
* [后端功能点](#后端功能点)
* [前端技术栈](#前端技术栈)
* [项目启动](#项目启动)
* [添加业务](#添加业务)
* [注意](#注意)
* [联系方式](#联系方式)

## 效果展示
![项目图片显示](https://img-blog.csdnimg.cn/2021032223384335.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTE5MDE0,size_16,color_FFFFFF,t_70#pic_center)

## 开发环境
- Linux x86_64（原始环境：centos 7 / gcc 7.3.1）或 macOS（Apple Silicon / Intel）
- C++11 编译器：gcc 7+ 或 Apple clang
- cmake 3.10+
- pthread（随系统提供，CMake 通过 `Threads::Threads` 链接）

编译依赖：
- Linux：`build-essential`、`cmake`（Debian/Ubuntu：`sudo apt-get install -y build-essential cmake`）
- macOS：Xcode Command Line Tools，以及 cmake（`brew install cmake` 或从 [cmake.org](https://cmake.org/download/) 安装）

## 线程模型

为了方便线程的管理已经线程资源的释放，所有的线程[除了主线程]都从线程池中获取

**网络模块：**
-    acceptor 单独一条线程[本项目中是在主线程中]，负责监听客户端的连接
-    poller   单独一条线程，负责监控客户端的读写事件
-    executor 线程池，负责处理客户端的请求，每个请求一条线程，读取请求数据，解析请求数据，做业务，发生响应数据都在该条线程中

**日志模块：**
-    logger 开启异步日志后，日志模块单独一条线程
    

因网络包发生和接受的不确定性，本项目采用状态机去解析http协议
    

## 后端功能点
- 日志管理，模仿log4j的实现逻辑，支持异步日志
- Json解析/Json序列化，C++对象的Json序列化 支持递归结构
- Http协议解析；支持表单格式解析；支持状态保存
- 线程池，可拓展，模仿QT QThreadPool线程池的实现，同时拓展了核心线程
- 模仿springmvc风格的controller
- 一些其他的工具和封装

## 前端技术栈

- vue
- element-ui组件库
- httpVueLoader的使用，本项目前端只是辅助展示，所以并没有安装node.js，直接用httpVueLoader来解析vue文件

## 项目启动

在仓库根目录编译（Linux 与 macOS 相同）：

```bash
mkdir -p build
cd build
cmake ..
cmake --build . -j
cd ..
```

若 Linux 上默认的 `c++` 是 clang 且链接阶段找不到 `-lstdc++`，请显式使用 g++：

```bash
CC=gcc CXX=g++ cmake ..
cmake --build . -j
```

可执行文件会输出到仓库根目录的 `candy`（由 CMake `EXECUTABLE_OUTPUT_PATH` 指定）。

启动参数如下
```bash
./candy -h
-a --addr 监听地址
-d --daemon 后台运行
-f --file 配置文件
-h --help 帮助
-p --port 端口
-s --stop 停止服务器
```
启动

```bash
./candy -p8888
```

成功后日志中会看到 I/O 选择器创建成功，并打印：

```
... run at http://0.0.0.0:8888
```

然后浏览器访问 `http://127.0.0.1:8888/` 即可。

前台运行时用 Ctrl+C 结束；若以 `-d` 守护进程启动，可用 `./candy -s` 停止。

编译测试程序（同样在 `build/` 下生成到仓库根目录）：

```bash
./testjson
./testhttpmultipart
./testcontroller
./testlog
```

## 添加业务
> 写一个HomeController.cpp文件，添加相对应的路径和方法映射，便能生成接口。支持Json对象，网页，以及直接返回。具体写法参照路径/src/app/controller下的controller的写法
```cpp
#include "../../container/controller.h"
#include "../../log/logger.h"
#include "../../util/urlutil.h"

struct HelloWorld {
    int code = 10;
    std::string msg = "hello-world";
    std::vector<int> ids = {1, 9, 2, 3, 4, 5};
    std::vector<HelloWorld> hello3;
};
//序列化
Serialization(HelloWorld, code, msg, ids, hello3)

//添加一个控制器
struct HomeController : public Controller<HomeController> {

    //返回的是网页路径
    RequestMapping(home, "/")
    std::string home(HttpRequest *request, HttpResponse *response) {
        return "/index.html";
    }

    //返回的是Json对象
    RequestMapping(hello, "/hello")
    ResponseBody hello(HttpRequest *request, HttpResponse *response) {
        HelloWorld helloWorld;
        helloWorld.hello3.emplace_back();
        return helloWorld;
    }
	
    //直接写到responseBody中
    RequestMapping(hello2, "/hello2")
    void hello2(HttpRequest *request, HttpResponse *response) {
        //直接输出......
        response->getBody().append("123");
    }
};
```


## 注意
- 如果遇到服务器启动了，但是却无法访问的情况，请检查是不是所选的端口开放了访问

## 联系方式
897894556@qq.com
