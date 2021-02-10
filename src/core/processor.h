//
// Created by Jsiyong on 2021-02-07.
//

#ifndef CANDY_PROCESSOR_H
#define CANDY_PROCESSOR_H

#include "channel.h"

/**
 * socket的处理元素，相当于一个一个的处理任务
 * 这个结合线程池之后，可能这个对象的数据会在不同的线程中使用和修改
 * 所以这个对象对外的处理方法需要加锁
 */
struct SocketProcessor {


private:
    Channel *_channel = NULL;
    pthread_mutex_t _mutex;//互斥锁
    HttpRequest *_request = NULL;//http请求对象
    HttpResponse *_response = NULL;//响应对象
};

#endif //CANDY_PROCESSOR_H
