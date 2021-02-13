//
// Created by Jsiyong on 2021-02-07.
//

#include "processor.h"
#include "../log/logger.h"

void SocketProcessor::run() {

    switch (_status) {
        case ProcessorStatus::READ: {
            //如果事件的fd不是监听的fd，说明有读写事件发生，由于是边缘非阻塞，所以需要注意要一次性读完缓冲区的所有数据
            //客户端的读事件产生
            _channel->read();

            //协议解析
            _request->tryDecode(_channel->getReadBuff());
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
            warn("[body size]%lld", _request->getBody().size());

            //解析http完成之后，下一步就是回复客户端
            if (_request->getDecodeState() == HttpRequestDecodeState::COMPLETE) {
                _status = ProcessorStatus::WRITE;
                if (_afterReadCompletedRequest) {
                    _afterReadCompletedRequest();
                }
            } else {
                if (_afterReadUnCompletedRequest) {
                    _afterReadUnCompletedRequest();
                }
            }
            break;
        }
        case ProcessorStatus::WRITE: {
            //如果是可写事件触发
            trace("start write...");
            _channel->write();
            if (_channel->finishWrite()) {
                _status = ProcessorStatus::READ;
                if (_afterWriteCompletedResponse) {
                    _afterWriteCompletedResponse();
                }
            } else {
                if (_afterWriteUnCompletedResponse) {
                    _afterWriteUnCompletedResponse();
                }
            }
            break;
        }
    }
}

SocketProcessor::SocketProcessor(int fd) {
    _channel = new Channel(fd);
    _request = new HttpRequest();
    _response = new HttpResponse();
}

SocketProcessor::~SocketProcessor() {
    delete _channel;
    delete _request;
    delete _response;
}

Channel *SocketProcessor::getChannel() const {
    return _channel;
}

void SocketProcessor::setOnAfterReadCompletedRequest(const std::function<void(void)> &afterReadCompletedRequest) {
    _afterReadCompletedRequest = afterReadCompletedRequest;
}

void SocketProcessor::setOnAfterWriteCompletedResponse(const std::function<void(void)> &afterWriteCompletedResponse) {
    _afterWriteCompletedResponse = afterWriteCompletedResponse;
}

void SocketProcessor::setOnAfterReadUnCompletedRequest(const std::function<void(void)> &afterReadUnCompletedRequest) {
    _afterReadUnCompletedRequest = afterReadUnCompletedRequest;
}

void
SocketProcessor::setOnAfterWriteUnCompletedResponse(const std::function<void(void)> &afterWriteUnCompletedResponse) {
    _afterWriteUnCompletedResponse = afterWriteUnCompletedResponse;
}
