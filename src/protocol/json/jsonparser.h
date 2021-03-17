#pragma once

#include <vector>
#include <list>
#include "jsonvalue.h"

enum class TokenType {
    ObjectBegin = 0,//对象开始
    ObjectEnd,//对象结束

    ArrayBegin,//数组开始
    ArrayEnd,//数组结束

    ObjectKeyValueSeparator,//对象键值对的分割符,也就是':'
    MemberSeparator,//成员分割符，也就是','

    String,//字符串
    Double,//实数
    LongLong,//整型
    True,
    False,
    Null//空值
};

struct JsonToken {
    TokenType type;

    std::string::const_iterator start;
    std::string::const_iterator end;

    JsonToken(TokenType type, const std::string::const_iterator &start) : type(type), start(start), end(start) {}
};

struct JsonParser {

    /**
     * 将json转为JsonValue
     * JsonValue: 返回的值
     */
    static JsonValue toJsonValue(const std::string &json);

    /**
     * 将jsonValue转为Json
     * json: 返回的值
     */
    static std::string toJson(const JsonValue &root);

private:

    //跳过空的
    static bool isSpace(char ch);

    /**
     * 判断是不是结束符
     * @param ch
     * @return
     */
    static bool isEndOfValue(char ch);

    /**
     * 是不是开始符
     * @param ch
     * @return
     */
    static bool isBeginOfValue(char ch);

    /**
     * 判断是不是分割符
     * @param ch
     * @return
     */
    static bool isSeparator(char ch);

    /**
     * 如果是token就加入token列表
     * @param ch
     */
    static void pushBackIfToken(std::list<JsonToken> &tokens, char ch, const std::string::const_iterator &pos);

    //通过token生成jsonValue
    static JsonValue genJsonValueViaTokens(std::list<JsonToken> &tokens);

    //通过token生成jsonArray
    static JsonValue genJsonArrayViaTokens(std::list<JsonToken> &tokens);

    //通过token生成jsonObject
    static JsonValue genJsonObjectViaTokens(std::list<JsonToken> &tokens);
};
