#include "JsonValue.h"

JsonValue::JsonValue(ValueType type) {
	setType(type);
}

//全都初始化为空
JsonValue::JsonValue() {
	_type = ValueType::Null;
	_value._string = NULL;
}

void JsonValue::setType(ValueType type) {
	//初始化
	_type = type;
	_value._string = NULL;

	static std::string emptyString = "";
	switch (_type) {
	case ValueType::Null:
		break;
	case ValueType::Real:
		_value._real = 0.0;
		break;
	case ValueType::String:
		_value._string = new std::string(emptyString);
		break;
	case ValueType::Array:
		_value._array = new std::list<JsonValue>();
		break;
	case ValueType::Object:
		_value._object = new std::map<std::string, JsonValue>();
		break;
	case ValueType::Boolean:
		_value._bool = false;
		break;
	default:
		break;
	}
}
