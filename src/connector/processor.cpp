//
// Created by Jsiyong on 2021-02-07.
//

#include "processor.h"
#include "../log/logger.h"

void SocketProcessor::run() {

    switch (_status) {
        case ProcessorStatus::READ_REQUEST: {
            //如果事件的fd不是监听的fd，说明有读写事件发生，由于是边缘非阻塞，所以需要注意要一次性读完缓冲区的所有数据
            //客户端的读事件产生，读到recvBuffer中
            _channel->read(_recvBuffer);
            if (_channel->close()) {
                //若EPOLLRDHUP没有和EPOLLIN一起触发，可能会出现这种情况，但是在EPOLLRDHUP那里会处理的，所以直接break,退出函数就行
                break;
            }

            //协议解析
            _request->tryDecode(_recvBuffer);

            //解析完成后清空buf
            _recvBuffer.clear();

            //解析http完成之后，下一步就是回复客户端
            if (_request->completed()/* || !_request->valid()*/) {
                //在这里开始处理业务
                _status = ProcessorStatus::DO_SERVICE;
                if (_afterReadCompletedRequest) {
                    _afterReadCompletedRequest();
                }
            } else {
                //协议没解析完，下一次继续读取客户端请求
                if (_afterReadUnCompletedRequest) {
                    _afterReadUnCompletedRequest();
                }
            }
            break;
        }
        case ProcessorStatus::DO_SERVICE: {
            //开始做业务
            //做业务
            _pipeline->getServlet()->doService(_request, _response);
            //做完业务开始http回复报文的编码，将数据读取到发送缓冲区中
            _response->encode(_sendBuffer);
            _request->clear();//请求数据也没有用了
            _response->clear();//响应报文也没用了
//            //做完业务就开始写数据
            _status = ProcessorStatus::WRITE_RESPONSE;
            //注意，不要break，因为做完业务之后就开始写数据了，状态直接就是ProcessorStatus::WRITE_RESPONSE
        }
        case ProcessorStatus::WRITE_RESPONSE: {

            //如果是可写事件触发
//            trace("start write...");
            size_t writedSize = _channel->write(_sendBuffer);
//            trace("response data: %s", _sendBuffer);
            _sendBuffer.assign(_sendBuffer.begin() + writedSize, _sendBuffer.end());//写完之后就清空缓冲区

            if (_sendBuffer.empty()) {
                _status = ProcessorStatus::READ_REQUEST;
                if (_afterWriteCompletedResponse) {
                    _afterWriteCompletedResponse();
                }
            } else {
                //还有空余数据没发送
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

    _pipeline = new Pipeline();
}

SocketProcessor::~SocketProcessor() {
    delete _channel;
    delete _request;
    delete _response;
    delete _pipeline;
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
