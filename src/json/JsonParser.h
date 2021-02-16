#pragma once
#include <vector>
#include <list>
#include "JsonValue.h"

enum class TokenType {
	ObjectBegin = 0,//对象开始
	ObjectEnd,//对象结束

	ArrayBegin,//数组开始
	ArrayEnd,//数组结束

	ObjectKeyValueSeparator,//对象键值对的分割符,也就是':'
	MemberSeparator,//成员分割符，也就是','

	String,//字符串
	Number,//数字
	True,
	False,
	Null//空值
};

struct JsonToken {
	TokenType type;

	std::vector<char>::const_iterator start;
	std::vector<char>::const_iterator end;

	JsonToken(TokenType type, std::vector<char>::const_iterator start) :type(type), start(start), end(start) {}
};

struct JsonParser {

	/**
	 * 将json转为JsonValue
	 * JsonValue: 返回的值
	 */
	static bool parse(const std::vector<char>& json, JsonValue& root);

	/**
	 * 将jsonValue转为Json
	 * json: 返回的值
	 */
	static void parse(const JsonValue& root, std::vector<char>& json);

private:

	//跳过空的
	static bool isSpace(char ch);

	//通过token生成jsonValue
	static bool genJsonValueViaTokens(JsonValue& root, std::list<JsonToken>& tokens);

	//通过token生成jsonArray
	static bool genJsonArrayViaTokens(JsonValue& root, std::list<JsonToken>& tokens);

	//通过token生成jsonObject
	static bool genJsonObjectViaTokens(JsonValue& root, std::list<JsonToken>& tokens);
};
