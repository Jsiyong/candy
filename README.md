# 基于Epoll从零开始实现HTTP服务器
项目后端使用C++开发语言，以Linux为开发环境，基于epoll边缘非阻塞模型，从http协议的解析，到表单multipart/form-data协议的解析，再到JSON协议的解析，再到C++对象的序列化，一步一个脚印实现一个http服务器。实现的功能有文件的上传、下载、删除、改名等。

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
- centos 7
- gcc version 7.3.1
- cmake 3.17

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
启动参数如下
```bash
[root@localhost tmp.mkWXUL9TBY]# ./candy -h
-a --addr 监听地址
-d --daemon 后台运行
-f --file 配置文件
-h --help 帮助
-p --port 端口
-s --stop 停止服务器
```
启动

```bash
[root@localhost tmp.mkWXUL9TBY]# ./candy -p8888
2021-03-22 23:47:36.950 INFO  candy[139832041047872] poller.cpp:25 Poller >> epoll create success!!
2021-03-22 23:47:36.951 TRACE candy[139832041047872] acceptor.cpp:56 acceptAt >> 

████████      ████      ████       ██ ███████    ██          ██ 
██           ██  ██     ██ ██      ██ ██      ██   ██      ██   
██          ████████    ██   ██    ██ ██       ██    ██████     
██         ██      ██   ██     ██  ██ ██       ██      ██       
██        ██        ██  ██      ██ ██ ██      ██       ██       
████████ ██          ██ ██       ████ ████████         ██       run at http://0.0.0.0:8888

```
然后浏览器便可以访问了

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
