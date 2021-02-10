//
// Created by Jsiyong on 2021-02-07.
//

#include "processor.h"
#include "../log/logger.h"

void SocketProcessor::run() {
    pthread_mutex_lock(&_mutex);
    //如果事件的fd不是监听的fd，说明有读写事件发生，由于是边缘非阻塞，所以需要注意要一次性读完缓冲区的所有数据
    //客户端的读事件产生
    _channel->read();
    HttpRequest *pRequest = _channel->getHttpRequest();
    pRequest->tryDecode(_channel->getReadBuff());
#if 0
    info("[method]%s", pRequest->getMethod());
                info("[url]%s", pRequest->getUrl());
                info("[request params]");
                for (auto &p : pRequest->getRequestParams()) {
                    trace("++ key: %s", p.first);
                    trace("-- value: %s", p.second);
                }

                info("[protocol]%s", pRequest->getProtocol());
                info("[version]%s", pRequest->getVersion());
                info("[request headers]");
                for (auto &h : pRequest->getHeaders()) {
                    trace("++ key: %s", h.first);
                    trace("-- value: %s", h.second);
                }
                info("[body]%s", pRequest->getBody());
#endif
    warn("[body size]%lld", pRequest->getBody().size());

    //如果是可写事件触发
    if (pRequest->getDecodeState() == HttpRequestDecodeState::COMPLETE) {
        _channel->write();
    }
    pthread_mutex_unlock(&_mutex);
}

SocketProcessor::SocketProcessor(int fd) {
    _channel = new Channel(fd);
    pthread_mutex_init(&_mutex, NULL);
}

SocketProcessor::~SocketProcessor() {
    delete _channel;
}
