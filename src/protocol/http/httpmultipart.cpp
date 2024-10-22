//
// Created by Jsiyong on 2021-03-13.
//

#include "httpmultipart.h"
#include "string.h"

enum DecodeState {
    START,
    START_BOUNDARY,

    HEADER_FIELD_START_BEFORE,//头开始前
    HEADER_FIELD_START,
    HEADER_FIELD,
    HEADER_VALUE_START,
    HEADER_VALUE,
    HEADER_VALUE_ALMOST_DONE,
    HEADERS_ALMOST_DONE,
    PART_DATA_START_BEFORE,
    PART_DATA,
    PART_END,
    END,//结束的状态
    ERROR//错误的状态
};

void HttpMultiPart::tryDecode(const std::string &src) {
    //解析完成或者buf为空都直接返回
    if (src.empty() || completed()) {
        return;
    }

    size_t i = 0;
    for (; i < src.size() && _decodeState != DecodeState::ERROR && _decodeState != DecodeState::END; ++i) {
        char ch = src.at(i);//当前的字符
        switch (_decodeState) {
            case DecodeState::START: {
                if (matchBoundaryChar(ch)) {
                    _matchedBoundaryBuffer.push_back(ch);//分割符开始
                    _decodeState = DecodeState::START_BOUNDARY;//下一个状态就开始分割线
                } else {
                    _decodeState = DecodeState::ERROR;//否则就是错误
                }
                break;
            }
            case DecodeState::START_BOUNDARY: {
                if (ch == '\r') {
                    _matchedBoundaryBuffer.clear();
                    _decodeState = DecodeState::HEADER_FIELD_START_BEFORE;
                } else if (matchBoundaryChar(ch)) {
                    _matchedBoundaryBuffer.push_back(ch);
                } else {
                    _decodeState = DecodeState::ERROR;
                }
                break;
            }
            case DecodeState::HEADER_FIELD_START_BEFORE: {
                //遇到\r..表示可能boundary结束了，比较前面的boundary是否与目标的相等，是的话就是了
                if (ch == '\n') {
                    _decodeState = DecodeState::HEADER_FIELD_START;//开始解析字段
                    //创建一个表单项
                    _formData.emplace_back();
                } else {
                    _decodeState = DecodeState::ERROR;
                }
                break;
            }
            case DecodeState::HEADER_FIELD_START: {
                if (isspace(ch)) {
                    //do bothing
                } else {
                    _decodeState = DecodeState::HEADER_FIELD;
                    _headerKey.assign(1, ch);
                }
                break;
            }
            case DecodeState::HEADER_FIELD: {
                if (isspace(ch) || ch == ':') {
                    _decodeState = DecodeState::HEADER_VALUE_START;
                } else if (ch == '\r' || ch == '\n') {
                    _decodeState = DecodeState::ERROR;
                } else {
                    _headerKey.push_back(ch);
                }
                break;
            }
            case DecodeState::HEADER_VALUE_START: {
                if (isspace(ch)) {
                    //do nothing
                } else if (ch == '\r' || ch == '\n') {
                    _decodeState = DecodeState::ERROR;
                } else {
                    _decodeState = DecodeState::HEADER_VALUE;
                    _headerValue.assign(1, ch);
                }
                break;
            }
            case DecodeState::HEADER_VALUE: {
                if (ch == '\r') {
                    _formData.back().header.emplace(_headerKey, _headerValue);//加入formData
                    _decodeState = DecodeState::HEADER_VALUE_ALMOST_DONE;
                } else {
                    _headerValue.push_back(ch);
                }
                break;
            }
            case DecodeState::HEADER_VALUE_ALMOST_DONE: {
                if (ch == '\n') {
                    _decodeState = DecodeState::HEADERS_ALMOST_DONE;
                } else {
                    _decodeState = DecodeState::ERROR;
                }
                break;
            }
            case DecodeState::HEADERS_ALMOST_DONE: {
                if (ch == '\r') {
                    _decodeState = DecodeState::PART_DATA_START_BEFORE;
                } else {
                    //如果不是遇到\r，那么就是下一个键值对开始，开始解析下一个键值对
                    _decodeState = DecodeState::HEADER_FIELD;
                    _headerKey.assign(1, ch);
                }
                break;
            }
            case DecodeState::PART_DATA_START_BEFORE: {
                if (ch == '\n') {
                    _decodeState = DecodeState::PART_DATA;
                } else {
                    _decodeState = DecodeState::ERROR;
                }
                break;
            }
            case DecodeState::PART_DATA: {
                //如果遇到分割符，就一直一直加进去
                if (matchBoundaryChar(ch)) {
                    _matchedBoundaryBuffer.push_back(ch);
                } else if (isFullBoundary() && ch == '\r') {
                    _matchedBoundaryBuffer.clear();
                    //遇到下一个分割符了，那就开始解析报文头了
                    _decodeState = DecodeState::HEADER_FIELD_START_BEFORE;
                } else if (isBoundaryFinallyEnd() && ch == '\r') {
                    //遇到结尾了，解析即将结束
                    _decodeState = DecodeState::PART_END;
                } else {
                    //若不正常，则继续当成body处理
                    if (!_matchedBoundaryBuffer.empty()) {
                        _formData.back().body.append(_matchedBoundaryBuffer);
                        _matchedBoundaryBuffer.clear();
                    }
                    //需要再次判断当前的字符是不是符合boundary再决定要加入哪一个_matchedBoundaryBuffer/body
                    if (matchBoundaryChar(ch)) {
                        _matchedBoundaryBuffer.push_back(ch);
                    } else {
                        _formData.back().body.push_back(ch);
                    }
                }
                break;
            }
            case DecodeState::PART_END: {
                if (ch == '\n') {
                    _decodeState = DecodeState::END;
                } else {
                    _decodeState = DecodeState::ERROR;
                }
            }
            default:
                break;
        }
    }
}

bool HttpMultiPart::matchBoundaryChar(char ch) {
    if (_matchedBoundaryBuffer.size() == _boundary.size() ||
        _matchedBoundaryBuffer.size() == _boundary.size() + 1) {
        return ch == '-';
    }
    if (_matchedBoundaryBuffer.size() >= _boundary.size() + 2) {
        return false;//其他情况是错误的
    }
    //如果当前加入的字符也是_boundary对应位置的字符，那么就是成功的
    return _boundary.at(_matchedBoundaryBuffer.size()) == ch;
}

void HttpMultiPart::setBoundary(const std::string &boundary) {
    _boundary = "\r\n--" + boundary;//加上前面的\r\n--，方便状态机处理
    _matchedBoundaryBuffer.append("\r\n");//初始化_matchedBoundaryBuffer
}

bool HttpMultiPart::isBoundaryFinallyEnd() {
    return _boundary.size() + 2 == _matchedBoundaryBuffer.size();//直接比较大小就够了
}

bool HttpMultiPart::isFullBoundary() {
    return _boundary.size() == _matchedBoundaryBuffer.size();//直接比较大小就够了，内容已经在上面比较过了
}

bool HttpMultiPart::completed() {
    return _decodeState == DecodeState::END;
}

void HttpMultiPart::fromHttpRequest(HttpRequest *request) {
    if (request && request->getBody().empty()) {
        return;
    }

    //Content-Type: multipart/form-data; boundary=----WebKitFormBoundarySAJsB3ZsTmgE6job
    std::string contentType = request->getHeader("Content-Type");
    if (contentType.empty()) {
        return;
    }

    int colonIndex = contentType.find_first_of(';');
    if (contentType.substr(0, colonIndex) != "multipart/form-data") {
        return;
    }
    //找出boundary
    int equalIndex = contentType.find_last_of('=');
    std::string boundary = contentType.substr(equalIndex + 1);
    //去掉空格
    boundary.erase(0, boundary.find_first_not_of(' '));
    boundary.erase(boundary.find_last_not_of(' ') + 1);

    this->setBoundary(boundary);
    this->tryDecode(request->getBody());//解析
}

std::map<std::string, std::string> HttpMultiPart::parseContentDisposition(const std::string &contentDisposition) {
    //Content-Disposition: form-data; name=\"cs1\"; filename=\"tp2.png\"
    //form-data; name=\"cs1\"; filename=\"tp2.png\"
    std::string keyValueString = contentDisposition.substr(contentDisposition.find_first_of(';') + 1);
    if (keyValueString.empty()) {
        return {};
    }

    int state = 0;
    std::map<std::string, std::string> result;
    std::string key;
    std::string value;

    for (int i = 0; i < keyValueString.size(); ++i) {
        char ch = keyValueString.at(i);
        switch (state) {
            case 0: {
                //开始
                if (!isspace(ch) && ch != ';') {
                    key.assign(1, ch);
                    state = 1;
                }
                break;
            }
            case 1: {//遇到等于号
                if (ch == '=') {
                    state = 2;
                } else {
                    key.push_back(ch);
                }
                break;
            }
            case 2: {
                if (ch == '"') {
                    value.clear();
                    state = 3;
                }
                break;
            }
            case 3: {
                if (ch == '"') {
                    //字符串结束了
                    state = 0;
                    result.emplace(key, value);
                } else {
                    value.push_back(ch);
                }
                break;
            }
        }
    }
    return result;
}

const std::vector<FormDataItem> &HttpMultiPart::getFormData() const {
    return _formData;
}
