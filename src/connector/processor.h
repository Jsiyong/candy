//
// Created by Jsiyong on 2021-02-07.
//

#ifndef CANDY_PROCESSOR_H
#define CANDY_PROCESSOR_H

#include "channel.h"
#include "../util/threadpool.h"
#include "../protocol/http/httprequest.h"
#include "../container/servlet.h"

/**
 * socket处理器的状态机
 */
enum class ProcessorStatus {
    READ_REQUEST,//读请求状态
    DO_SERVICE,//处理服务的状态
    WRITE_RESPONSE//写回复数据状态
};

/**
 * socket的处理元素，相当于一个一个的处理任务
 * 这个结合线程池之后，可能这个对象的数据会在不同的线程中使用和修改
 * 所以这个对象对外的处理方法需要加锁
 */
struct SocketProcessor : Runnable {

    explicit SocketProcessor(int fd);

    ~SocketProcessor() override;

    /**
     * 需要加锁，因为有可能解析协议等会有多个线程同时进行
     */
    void run() override;

    SocketChannel *getChannel() const;

    void setOnAfterReadCompletedRequest(const std::function<void(void)> &afterReadCompletedRequest);

    void setOnAfterWriteCompletedResponse(const std::function<void(void)> &afterWriteCompletedResponse);

    void setOnAfterReadUnCompletedRequest(const std::function<void(void)> &afterReadUnCompletedRequest);

    void setOnAfterWriteUnCompletedResponse(const std::function<void(void)> &afterWriteUnCompletedResponse);

private:

    ProcessorStatus _status = ProcessorStatus::READ_REQUEST;//当前处理的状态
    SocketChannel *_channel = NULL;
    HttpRequest *_request = NULL;//http请求对象
    HttpResponse *_response = NULL;//响应对象

    Servlet *_servlet = NULL;//用户应用

    //回调函数
    std::function<void(void)> _afterReadCompletedRequest;//在读到完整请求之后
    std::function<void(void)> _afterReadUnCompletedRequest;//在读到不完整请求之后
    std::function<void(void)> _afterWriteCompletedResponse;//在写完完整的回复之后
    std::function<void(void)> _afterWriteUnCompletedResponse;//在写完完整的回复之后
};

#endif //CANDY_PROCESSOR_H
