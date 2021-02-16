#include "JsonParser.h"

enum class ParserState {
	START = 0,//开始解析

	STRING_BEGIN,//字符串开始
	STRING,//解析字符串过程中

	NUMBER_BEGIN,//数字开始
	ZERO_NUMBER_BEGIN,//数字以0开局
	NUMBER_BEFORE_POINT,//解析小数点前的数字
	NUMBER_AFTER_POINT,//小数点后的数字

	VALUE_END,//值结束的状态

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

	ARRAY,//解析数组

	ESCAPE,//转义/

	ERROR//错误的状态
};

bool JsonParser::parse(const std::vector<char>& json, JsonValue& root) {

	std::vector<char>::const_iterator p = json.begin();//当前的指针

	ParserState state = ParserState::START;
	ParserState preState = state;//前一个状态

	std::list<JsonToken> tokens;//存放有效的token
	//状态机自我运行中
	while (state != ParserState::ERROR && p != json.end()) {
		char ch = *p;
		auto cur = p++;

		switch (state) {
		case ParserState::START: {
			if (isSpace(ch)) {
				//do nothing
			}
			else if (ch == '{') {
				tokens.push_back(JsonToken(TokenType::ObjectBegin, cur));
				//下一步解析对象
			}
			else if (ch == '[') {
				tokens.push_back(JsonToken(TokenType::ArrayBegin, cur));
				//解析数组
			}
			else if (ch == '"') {
				//字符串
				state = ParserState::STRING_BEGIN;
			}
			else if (ch == '-') {
				//解析数字的状态
				state = ParserState::NUMBER_BEGIN;
				tokens.push_back(JsonToken(TokenType::Number, cur));
			}
			else if (ch == '0') {
				//小数开始
				tokens.push_back(JsonToken(TokenType::Number, cur));
				state = ParserState::ZERO_NUMBER_BEGIN;
			}
			else if (ch >= '1' && ch <= '9') {
				tokens.push_back(JsonToken(TokenType::Number, cur));
				state = ParserState::NUMBER_BEFORE_POINT;
			}
			else if (ch == 'f') {
				//解析false
				tokens.push_back(JsonToken(TokenType::False, cur));
				state = ParserState::F_IN_FALSE;
			}
			else if (ch == 't') {
				tokens.push_back(JsonToken(TokenType::True, cur));
				state = ParserState::T_IN_TRUE;
			}
			else if (ch == 'n') {
				tokens.push_back(JsonToken(TokenType::Null, cur));
				state = ParserState::N_IN_NULL;
			}
			else {
				state = ParserState::ERROR;
			}
			break;
		}
		case ParserState::N_IN_NULL: {
			if (ch == 'u') {
				state = ParserState::NU_IN_NULL;
			}
			else {
				state = ParserState::ERROR;
			}
			break;
		}
		case ParserState::NU_IN_NULL: {
			if (ch == 'l') {
				state = ParserState::NUL_IN_NULL;
			}
			else {
				state = ParserState::ERROR;
			}
			break;
		}
		case ParserState::NUL_IN_NULL: {
			if (ch == 'l') {
				tokens.back().end = cur;
				state = ParserState::VALUE_END;
			}
			else {
				state = ParserState::ERROR;
			}
			break;
		}

		case ParserState::T_IN_TRUE: {
			if (ch == 'r') {
				state = ParserState::TR_IN_TRUE;
			}
			else {
				state = ParserState::ERROR;
			}
			break;
		}
		case ParserState::TR_IN_TRUE: {
			if (ch == 'u') {
				state = ParserState::TRU_IN_TRUE;
			}
			else {
				state = ParserState::ERROR;
			}
			break;
		}
		case ParserState::TRU_IN_TRUE: {
			if (ch == 'e') {
				tokens.back().end = cur;
				state = ParserState::VALUE_END;
			}
			else {
				state = ParserState::ERROR;
			}
			break;
		}
		case ParserState::F_IN_FALSE: {
			if (ch == 'a') {
				state = ParserState::FA_IN_FALSE;
			}
			else {
				state = ParserState::ERROR;
			}
			break;
		}
		case ParserState::FA_IN_FALSE: {
			if (ch == 'l') {
				state = ParserState::FAL_IN_FALSE;
			}
			else {
				state = ParserState::ERROR;
			}
			break;
		}
		case ParserState::FAL_IN_FALSE: {
			if (ch == 's') {
				state = ParserState::FALS_IN_FALSE;
			}
			else {
				state = ParserState::ERROR;
			}
			break;
		}
		case ParserState::FALS_IN_FALSE: {
			if (ch == 'e') {
				tokens.back().end = cur;
				state = ParserState::VALUE_END;
			}
			else {
				state = ParserState::ERROR;
			}
			break;
		}
		case ParserState::ESCAPE: {
			//遇到转义的，回到前一个状态
			state = preState;
			break;
		}
		case ParserState::STRING_BEGIN: {
			tokens.push_back(JsonToken(TokenType::String, cur));//栈顶元素为字符串类型
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
				state = ParserState::VALUE_END;
			}
			else if (ch == '\\') {
				//转义的话
				state = ParserState::ESCAPE;
			}
			else {
				//do nothing
			}
			break;
		}
		case ParserState::VALUE_END: {
			if (isSpace(ch)) {
				//do nothing
			}
			else if (ch == ',') {
				//如果是',',表示是分割符，则重头开始解析
				tokens.push_back(JsonToken(TokenType::MemberSeparator, cur));
				state = ParserState::START;
			}
			else if (ch == ':') {
				//如果是':',表示是对象键值对的分割符，则重头开始解析
				tokens.push_back(JsonToken(TokenType::ObjectKeyValueSeparator, cur));
				state = ParserState::START;
			}
			else if (ch == '}') {
				//遇到的是对象的结束标志
				tokens.push_back(JsonToken(TokenType::ObjectEnd, cur));
			}
			else if (ch == ']') {
				//遇到的是数组的结束标志
				tokens.push_back(JsonToken(TokenType::ArrayEnd, cur));
			}
			else {
				state = ParserState::ERROR;
			}
			break;
		}
		case ParserState::NUMBER_BEGIN: {
			//解析数字
			if (ch == '0') {
				state = ParserState::ZERO_NUMBER_BEGIN;
			}
			else if (ch >= '1' && ch <= '9') {
				state = ParserState::NUMBER_BEFORE_POINT;//小数点之前的数字
			}
			else {
				//其他情况都是错误的
				state = ParserState::ERROR;
			}
			break;
		}
		case ParserState::ZERO_NUMBER_BEGIN: {
			//遇到0开头的数字
			if (ch == '.') {
				//解析小数
				state = ParserState::NUMBER_AFTER_POINT;//小数点之后的数字
			}
			else if (ch == ',') {
				tokens.back().end = cur;
				tokens.push_back(JsonToken(TokenType::MemberSeparator, cur));
				state = ParserState::START;
			}
			else if (isSpace(ch)) {
				tokens.back().end = cur;
				//如果是空格，说明是数字0
				state = ParserState::VALUE_END;
			}
			else {
				//其他情况都是不允许的
				state = ParserState::ERROR;
			}
			break;
		}
		case ParserState::NUMBER_BEFORE_POINT: {
			//小数点前
			if (ch >= '0' && ch <= '9') {
				//do nothing
			}
			else if (ch == '.') {
				//遇到.转到小数点后的状态
				state = ParserState::NUMBER_AFTER_POINT;
			}
			else if (ch == ',') {
				tokens.back().end = cur;
				tokens.push_back(JsonToken(TokenType::MemberSeparator, cur));
				state = ParserState::START;
			}
			else if (isSpace(ch)) {
				tokens.back().end = cur;
				state = ParserState::VALUE_END;//结束
			}
			else {
				//其他情况错误
				state = ParserState::ERROR;
			}
			break;
		}
		case ParserState::NUMBER_AFTER_POINT: {
			//小数点后的状态就随意了，只要是数字就行
			if (ch >= '0' && ch <= '9') {
				//do nothing
			}
			else if (ch == ',') {
				tokens.back().end = cur;
				tokens.push_back(JsonToken(TokenType::MemberSeparator, cur));
				state = ParserState::START;
			}
			else if (isSpace(ch)) {
				tokens.back().end = cur;
				state = ParserState::VALUE_END;
			}
			else {
				state = ParserState::ERROR;
			}
			break;
		}

		}
		//记录前一个状态，方便回去
		preState = state;
	}
	//若解析是错误的，直接返回
	if (state == ParserState::ERROR) {
		return false;
	}
	//生成jsonValue
	return genJsonValueViaTokens(root, tokens);
}

bool JsonParser::isSpace(char ch) {
	return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}

bool JsonParser::genJsonValueViaTokens(JsonValue& root, std::list<JsonToken>& tokens) {
	//先出队一个元素
	JsonToken token = tokens.front();
	//当前处理的是值
	tokens.pop_front();

	if (TokenType::ObjectBegin == token.type) {
		root.setType(ValueType::Object);
		//当前处理的是对象
		return genJsonObjectViaTokens(root, tokens);
	}
	else if (TokenType::ArrayBegin == token.type) {
		root.setType(ValueType::Array);
		//当前处理的是数组
		return genJsonArrayViaTokens(root, tokens);
	}
	else {
		if (TokenType::String == token.type) {
			root.setType(ValueType::String);//当前是字符串
			root._value._string->assign(token.start, token.end);
		}
		else if (TokenType::True == token.type) {
			root.setType(ValueType::Boolean);//当前是布尔型
			root._value._bool = true;
		}
		else if (TokenType::False == token.type) {
			root.setType(ValueType::Boolean);//当前是布尔型
			root._value._bool = false;
		}
		else if (TokenType::Number == token.type) {
			root.setType(ValueType::Real);
			root._value._real = std::atof(std::string(token.start, token.end).c_str());
		}
		else if (TokenType::Null == token.type) {
			root.setType(ValueType::Null);
		}
		else {
			//do nothing
		}
	}
	return true;
}

//因为栈的关系，导致列表的顺序是反的，所以需要倒过来
bool JsonParser::genJsonArrayViaTokens(JsonValue& root, std::list<JsonToken>& tokens) {
	//找到第一个']'然后不停出队
	while (TokenType::ArrayEnd != tokens.front().type) {
		JsonValue valueTmp;
		genJsonValueViaTokens(valueTmp, tokens);
		root._value._array->push_back(valueTmp);
		//然后去掉数组间的分割符
		if (TokenType::MemberSeparator == tokens.front().type) {
			tokens.pop_front();//','
		}
		else if (TokenType::ArrayEnd == tokens.front().type) {
			//do nothing
		}
		else {
			return false;
		}
	}
	//循环结束后，类型为ArrayBegin，需要pop掉
	tokens.pop_front();
	return true;
}

bool JsonParser::genJsonObjectViaTokens(JsonValue& root, std::list<JsonToken>& tokens) {
	//找到第一个'}'然后不停出队
	while (TokenType::ObjectEnd != tokens.front().type) {
		//再出队key
		JsonToken key = tokens.front();
		tokens.pop_front();

		//然后去掉对象间的分割符':'
		if (tokens.front().type != TokenType::ObjectKeyValueSeparator) {
			return false;
		}
		tokens.pop_front();//':'

		//再出栈value
		JsonValue valueTmp;
		genJsonValueViaTokens(valueTmp, tokens);

		root._value._object->insert(std::make_pair(std::string(key.start, key.end), valueTmp));

		//最后再去掉对象值之间的分割符
		if (TokenType::MemberSeparator == tokens.front().type) {
			tokens.pop_front();//','
		}
		else if (TokenType::ObjectEnd == tokens.front().type) {
			//do nothing
		}
		else {
			return false;
		}
	}
	//循环结束后，类型为TokenType::ObjectBegin，需要pop掉
	tokens.pop_front();
	return true;
}


void JsonParser::parse(const JsonValue& root, std::vector<char>& json) {

	switch (root._type)
	{
	case ValueType::Null: {
		json.insert(json.end(), { 'n','u','l','l' });
		break;
	}
	case ValueType::Real: {
		std::string str = std::to_string(root._value._real);
		json.insert(json.end(), str.begin(), str.end());
		break;
	}
	case ValueType::String: {
		json.push_back('"');
		json.insert(json.end(), root._value._string->begin(), root._value._string->end());
		json.push_back('"');
		break;
	}
	case ValueType::Boolean: {
		if (root._value._bool) {
			json.insert(json.end(), { 't','r','u','e' });
		}
		else {
			json.insert(json.end(), { 'f','a','l','s','e' });
		}
		break;
	}
	case ValueType::Array: {
		json.push_back('[');
		bool isFirst = true;
		for (const auto& v : *root._value._array) {
			if (isFirst) {
				isFirst = false;
			}
			else {
				json.push_back(',');//不是第一次，就加上','分割符
			}
			std::vector<char> jsonTmp;
			parse(v, jsonTmp);
			json.insert(json.end(), jsonTmp.begin(), jsonTmp.end());
		}
		json.push_back(']');
		break;
	}
	case ValueType::Object: {
		json.push_back('{');
		bool isFirst = true;
		for (const auto& kv : *root._value._object) {
			if (isFirst) {
				isFirst = false;
			}
			else {
				json.push_back(',');//不是第一次，就加上','分割符
			}
			//找出key
			json.push_back('"');
			json.insert(json.end(), kv.first.begin(), kv.first.end());
			json.push_back('"');
			//插入分割符
			json.push_back(':');
			//找出value
			std::vector<char> jsonTmp;
			parse(kv.second, jsonTmp);
			json.insert(json.end(), jsonTmp.begin(), jsonTmp.end());
		}
		json.push_back('}');
		break;
	}
	default:
		break;
	}
}
