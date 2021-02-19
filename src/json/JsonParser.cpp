#include "jsonparser.h"

enum class ParserState {
    BEGIN = 0,//开始解析

    OBJECT_OR_ARRAY_BEGIN,//如果是数组或者字符串

    STRING_BEGIN,//字符串开始
    STRING,//解析字符串过程中

    NUMBER_BEGIN,//数字开始
    NUMBER_POINT,//小数点
    NUMBER_BEFORE_POINT,//解析小数点前的数字
    NUMBER_AFTER_POINT,//解析小数点后的数字

    END,//结束的状态

    N_IN_NULL,
    NU_IN_NULL,
    NUL_IN_NULL,

    T_IN_TRUE,
    TR_IN_TRUE,
    TRU_IN_TRUE,

    F_IN_FALSE,
    FA_IN_FALSE,
    FAL_IN_FALSE,
    FALS_IN_FALSE,

    ESCAPE,//转义/

    ERROR//错误的状态
};

JsonValue JsonParser::parse(const std::vector<char> &json) {

    std::vector<char>::const_iterator p = json.begin();//当前的指针

    ParserState state = ParserState::BEGIN;

    std::list<JsonToken> tokens;//存放有效的token
    //状态机自我运行中
    while (state != ParserState::ERROR && p != json.end()) {
        char ch = *p;
        auto cur = p++;

        switch (state) {
            case ParserState::BEGIN: {
                if (isSpace(ch)) {
                    //do nothing
                } else if (isBeginOfValue(ch)) {
                    //判断是不是开始符号或者结束符号，那么就添加上去，同时状态还是这个开始状态，因为等着解析其他的字符
                    pushBackIfToken(tokens, ch, cur);
                    state = ParserState::OBJECT_OR_ARRAY_BEGIN;
                } else if (ch == '"') {
                    //字符串
                    state = ParserState::STRING_BEGIN;
                } else if (ch >= '0' && ch <= '9') {
                    tokens.emplace_back(TokenType::Double, cur);
                    p--;//记住，指针要向前移动
                    state = ParserState::NUMBER_BEGIN;
                } else if (ch == '-') {
                    //解析负数指针不用向前移动
                    tokens.emplace_back(TokenType::Double, cur);
                    state = ParserState::NUMBER_BEGIN;
                } else if (ch == 'f') {
                    //解析false
                    tokens.emplace_back(TokenType::False, cur);
                    state = ParserState::F_IN_FALSE;
                } else if (ch == 't') {
                    tokens.emplace_back(TokenType::True, cur);
                    state = ParserState::T_IN_TRUE;
                } else if (ch == 'n') {
                    tokens.emplace_back(TokenType::Null, cur);
                    state = ParserState::N_IN_NULL;
                } else {
                    state = ParserState::ERROR;
                }
                break;
            }
            case ParserState::OBJECT_OR_ARRAY_BEGIN: {
                if (isSpace(ch)) {
                    //do nothing
                } else if (isEndOfValue(ch)) {
                    //如果是结束符，那么下一个状态为值结束状态
                    pushBackIfToken(tokens, ch, cur);
                    state = ParserState::END;
                } else {
                    p--;//注意要退一格
                    state = ParserState::BEGIN;
                }
                break;
            }
            case ParserState::N_IN_NULL: {
                state = ch == 'u' ? ParserState::NU_IN_NULL : ParserState::ERROR;
                break;
            }
            case ParserState::NU_IN_NULL: {
                state = ch == 'l' ? ParserState::NUL_IN_NULL : ParserState::ERROR;
                break;
            }
            case ParserState::NUL_IN_NULL: {
                if (ch == 'l') {
                    tokens.back().end = cur;
                    state = ParserState::END;
                } else {
                    state = ParserState::ERROR;
                }
                break;
            }

            case ParserState::T_IN_TRUE: {
                state = ch == 'r' ? ParserState::TR_IN_TRUE : ParserState::ERROR;
                break;
            }
            case ParserState::TR_IN_TRUE: {
                state = ch == 'u' ? ParserState::TRU_IN_TRUE : ParserState::ERROR;
                break;
            }
            case ParserState::TRU_IN_TRUE: {
                if (ch == 'e') {
                    tokens.back().end = cur;
                    state = ParserState::END;
                } else {
                    state = ParserState::ERROR;
                }
                break;
            }
            case ParserState::F_IN_FALSE: {
                state = ch == 'a' ? ParserState::FA_IN_FALSE : ParserState::ERROR;
                break;
            }
            case ParserState::FA_IN_FALSE: {
                state = ch == 'l' ? ParserState::FAL_IN_FALSE : ParserState::ERROR;
                break;
            }
            case ParserState::FAL_IN_FALSE: {
                state = ch == 's' ? ParserState::FALS_IN_FALSE : ParserState::ERROR;
                break;
            }
            case ParserState::FALS_IN_FALSE: {
                if (ch == 'e') {
                    tokens.back().end = cur;
                    state = ParserState::END;
                } else {
                    state = ParserState::ERROR;
                }
                break;
            }
            case ParserState::ESCAPE: {
                //遇到转义的，回到前一个状态
                state = ParserState::STRING;//转义是在字符串才有的，所以回到字符串状态
                break;
            }
            case ParserState::STRING_BEGIN: {
                tokens.emplace_back(TokenType::String, cur);//栈顶元素为字符串类型
                //解析字符串
                state = ParserState::STRING;
                break;
            }
            case ParserState::STRING: {
                //字符串
                if (ch == '"') {
                    //字符串结束
                    //取出栈顶的元素,记录位置
                    tokens.back().end = cur;
                    state = ParserState::END;
                } else if (ch == '\\') {
                    //转义的话
                    state = ParserState::ESCAPE;
                }
                break;
            }
            case ParserState::END: {
                //如果是结尾的符，就一直解析，解析到不是结尾符为止
                if (isSpace(ch) || isSeparator(ch) || isEndOfValue(ch)) {
                    pushBackIfToken(tokens, ch, cur);
                } else {
                    //不是结尾符，那么转到开始状态,注意需要改变当前指针
                    p--;
                    state = ParserState::BEGIN;
                }
                break;
            }
            case ParserState::NUMBER_BEGIN: {
                //解析数字
                if (ch == '0') {
                    //解析0或者小数
                    state = ParserState::NUMBER_POINT;
                } else if (ch >= '1' && ch <= '9') {
                    state = ParserState::NUMBER_BEFORE_POINT;//小数点之前的数字
                } else {
                    //其他情况都是错误的
                    state = ParserState::ERROR;
                }
                break;
            }
            case ParserState::NUMBER_POINT: {
                //遇到0开头的数字
                if (ch == '.') {
                    //解析小数
                    state = ParserState::NUMBER_AFTER_POINT;//小数点之后的数字
                } else if (ch == ',' || isSpace(ch) || isEndOfValue(ch)) {
                    tokens.back().type = TokenType::LongLong;
                    tokens.back().end = cur;
                    p--;//退一格
                    state = ParserState::END;
                } else {
                    //其他情况都是不允许的
                    state = ParserState::ERROR;
                }
                break;
            }
            case ParserState::NUMBER_BEFORE_POINT: {
                //遇到0开头的数字
                if (ch >= '0' && ch <= '9') {
                    //do nothing
                } else {
                    //其他情况根ZERO_NUMBER_POINT一样，不过指针需要向前移动
                    p--;
                    state = ParserState::NUMBER_POINT;//小数点之后的数字
                }
                break;
            }
            case ParserState::NUMBER_AFTER_POINT: {
                //小数点前后，可以是0-9任何数字
                if (ch >= '0' && ch <= '9') {
                    //do nothing
                } else if (ch == ',' || isSpace(ch) || isEndOfValue(ch)) {
                    tokens.back().end = cur;
                    p--;//退一格
                    state = ParserState::END;
                } else {
                    //其他情况都是不允许的
                    state = ParserState::ERROR;
                }
                break;
            }
            default:
                break;
        }
    }
    //若解析是错误的，直接返回
    if (state == ParserState::ERROR) {
        return JsonValue();
    }
    //生成jsonValue
    return genJsonValueViaTokens(tokens);
}

bool JsonParser::isSpace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}

JsonValue JsonParser::genJsonValueViaTokens(std::list<JsonToken> &tokens) {
    //先出队一个元素
    JsonToken token = tokens.front();
    //当前处理的是值
    tokens.pop_front();

    if (TokenType::ObjectBegin == token.type) {
        //当前处理的是对象
        return genJsonObjectViaTokens(tokens);
    } else if (TokenType::ArrayBegin == token.type) {
        //当前处理的是数组
        return genJsonArrayViaTokens(tokens);
    }

    //下面的情况的值是普通类型的
    if (TokenType::String == token.type) {
        return JsonValue(token.start, token.end);//当前是字符串
    } else if (TokenType::True == token.type) {
        return JsonValue(true);//当前是布尔型
    } else if (TokenType::False == token.type) {
        return JsonValue(false);//当前是布尔型
    } else if (TokenType::Double == token.type) {
        return JsonValue(std::atof(std::string(token.start, token.end).c_str()));//当前是实数
    } else if (TokenType::LongLong == token.type) {
        return JsonValue(std::atoll(std::string(token.start, token.end).c_str()));//当前是实数
    } else if (TokenType::Null == token.type) {
        return JsonValue();
    }
    return JsonValue();
}

//因为栈的关系，导致列表的顺序是反的，所以需要倒过来
JsonValue JsonParser::genJsonArrayViaTokens(std::list<JsonToken> &tokens) {
    JsonArray jsonArray;
    //找到第一个']'然后不停出队
    while (TokenType::ArrayEnd != tokens.front().type) {
        jsonArray.append(genJsonValueViaTokens(tokens));

        //如果还没有遇到]结束符队列就空了，说明格式错误
        if (tokens.empty()) {
            return JsonValue();
        }

        //然后去掉数组间的分割符
        if (TokenType::MemberSeparator == tokens.front().type) {
            tokens.pop_front();//','
        } else if (TokenType::ArrayEnd == tokens.front().type) {
            //do nothing
        } else {
            return JsonValue();
        }
    }
    //循环结束后，类型为ArrayBegin，需要pop掉
    tokens.pop_front();
    return jsonArray;
}

JsonValue JsonParser::genJsonObjectViaTokens(std::list<JsonToken> &tokens) {
    JsonObject jsonObject;
    //找到第一个'}'然后不停出队
    while (TokenType::ObjectEnd != tokens.front().type) {
        //再出队key
        JsonToken key = tokens.front();
        tokens.pop_front();

        //然后去掉对象间的分割符':'
        if (tokens.front().type != TokenType::ObjectKeyValueSeparator) {
            return false;
        }
        //如果还没有遇到:队列就空了，说明格式错误
        if (tokens.empty()) {
            return JsonValue();
        }
        tokens.pop_front();//':'

        //再出队value
        jsonObject.insert(std::string(key.start, key.end), genJsonValueViaTokens(tokens));

        //如果还没有遇到}队列就空了，说明格式错误
        if (tokens.empty()) {
            return JsonValue();
        }

        //最后再去掉对象值之间的分割符
        if (TokenType::MemberSeparator == tokens.front().type) {
            tokens.pop_front();//','
        } else if (TokenType::ObjectEnd == tokens.front().type) {
            //do nothing
        } else {
            return JsonValue();
        }
    }
    //循环结束后，类型为TokenType::ObjectBegin，需要pop掉
    tokens.pop_front();
    return jsonObject;
}

std::vector<char> JsonParser::parse(const JsonValue &root) {
    std::vector<char> json;

    switch (root.type()) {
        case JsonValueType::Null: {
            json.insert(json.end(), {'n', 'u', 'l', 'l'});
            break;
        }
        case JsonValueType::Double: {
            std::string str = std::to_string(root.toDouble());
            json.insert(json.end(), str.begin(), str.end());
            break;
        }
        case JsonValueType::LongLong: {
            std::string str = std::to_string(root.toLongLong());
            json.insert(json.end(), str.begin(), str.end());
            break;
        }
        case JsonValueType::String: {
            json.push_back('"');
            //处理转义
            for (char ch : root.toString()) {
                switch (ch) {
                    case '\"':
                        json.insert(json.end(), {'\\', '\"'});
                        break;
                    case '\\':
                        json.insert(json.end(), {'\\', '\\'});
                        break;
                    case '\b':
                        json.insert(json.end(), {'\\', 'b'});
                        break;
                    case '\f':
                        json.insert(json.end(), {'\\', 'f'});
                        break;
                    case '\n':
                        json.insert(json.end(), {'\\', 'n'});
                        break;
                    case '\r':
                        json.insert(json.end(), {'\\', 'r'});
                        break;
                    case '\t':
                        json.insert(json.end(), {'\\', 't'});
                        break;
                    default:
                        json.push_back(ch);//其他情况不变
                }
            }
            //结尾加上'"'
            json.push_back('"');
            break;
        }
        case JsonValueType::Boolean: {
            if (root.toBoolean()) {
                json.insert(json.end(), {'t', 'r', 'u', 'e'});
            } else {
                json.insert(json.end(), {'f', 'a', 'l', 's', 'e'});
            }
            break;
        }
        case JsonValueType::Array: {
            json.push_back('[');
            bool isFirst = true;
            for (const auto &v : root.toArray()) {
                if (isFirst) {
                    isFirst = false;
                } else {
                    json.push_back(',');//不是第一次，就加上','分割符
                }
                std::vector<char> jsonTmp = parse(v);
                json.insert(json.end(), jsonTmp.begin(), jsonTmp.end());
            }
            json.push_back(']');
            break;
        }
        case JsonValueType::Object: {
            json.push_back('{');
            bool isFirst = true;
            for (const auto &kv : root.toObject()) {
                if (isFirst) {
                    isFirst = false;
                } else {
                    json.push_back(',');//不是第一次，就加上','分割符
                }
                //找出key
                std::vector<char> jsonTmp = parse(kv.first);
                json.insert(json.end(), jsonTmp.begin(), jsonTmp.end());

                //插入分割符
                json.push_back(':');

                //找出value
                jsonTmp = parse(kv.second);
                json.insert(json.end(), jsonTmp.begin(), jsonTmp.end());
            }
            json.push_back('}');
            break;
        }
        default:
            break;
    }
    return json;
}

bool JsonParser::isEndOfValue(char ch) {
    return ch == '}' || ch == ']';
}

void JsonParser::pushBackIfToken(std::list<JsonToken> &tokens, char ch, const std::vector<char>::const_iterator &pos) {
    switch (ch) {
        case '{':
            tokens.emplace_back(TokenType::ObjectBegin, pos);
            break;
        case '}':
            tokens.emplace_back(TokenType::ObjectEnd, pos);
            break;
        case '[':
            tokens.emplace_back(TokenType::ArrayBegin, pos);
            break;
        case ']':
            tokens.emplace_back(TokenType::ArrayEnd, pos);
            break;
        case ':':
            tokens.emplace_back(TokenType::ObjectKeyValueSeparator, pos);
            break;
        case ',':
            tokens.emplace_back(TokenType::MemberSeparator, pos);
            break;
        default:
            break;
    }
}

bool JsonParser::isSeparator(char ch) {
    return ch == ':' || ch == ',';
}

bool JsonParser::isBeginOfValue(char ch) {
    return ch == '{' || ch == '[';
}
