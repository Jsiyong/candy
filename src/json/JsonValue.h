#pragma once
#include <map>
#include <list>
#include <string>

enum class ValueType {
	Null = 0,
	Real,//实数一个类型就够了，其他类型都能转成实数类型
	String,
	Boolean,
	Array,
	Object
};

/**
 * Json的值对象
 */
struct JsonValue {
	friend struct JsonParser;

	JsonValue(ValueType type);
	JsonValue();

private:

	void setType(ValueType type);

	ValueType _type;

	//保存值
	union ValueHolder {
		double _real;
		bool _bool;
		std::string* _string;//字符串
		std::map<std::string, JsonValue>* _object;//对象
		std::list<JsonValue>* _array;//数组
	} _value;
};
