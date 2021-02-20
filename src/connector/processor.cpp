//
// Created by Jsiyong on 2021-02-07.
//

#include "processor.h"
#include "../log/logger.h"

void SocketProcessor::run() {

    switch (_status) {
        case ProcessorStatus::READ_REQUEST: {
            //如果事件的fd不是监听的fd，说明有读写事件发生，由于是边缘非阻塞，所以需要注意要一次性读完缓冲区的所有数据
            //客户端的读事件产生
            _channel->read();

            //协议解析
            _request->tryDecode(_channel->getReadBuff());

            //解析http完成之后，下一步就是回复客户端
            if (_request->getDecodeState() == HttpRequestDecodeState::COMPLETE) {
                //在这里开始处理业务
                _status = ProcessorStatus::DO_SERVICE;
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
        case ProcessorStatus::DO_SERVICE: {
            //开始做业务
            //做业务
            _servlet->service(_request, _response);

            //做完业务就开始写数据
            _status = ProcessorStatus::WRITE_RESPONSE;
            //注意，不要break，因为做完业务之后就开始写数据了，状态直接就是ProcessorStatus::WRITE_RESPONSE
        }
        case ProcessorStatus::WRITE_RESPONSE: {
            //如果是可写事件触发
            trace("start write...");
            _channel->write();
            if (_channel->finishWrite()) {
                _status = ProcessorStatus::READ_REQUEST;
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
    _channel = new SocketChannel(fd);
    _request = new HttpRequest();
    _response = new HttpResponse();

    _servlet = new DispatcherServlet();
}

SocketProcessor::~SocketProcessor() {
    delete _channel;
    delete _request;
    delete _response;
}

SocketChannel *SocketProcessor::getChannel() const {
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
