#include "JsonValue.h"

JsonValue::JsonValue() {
    setType(JsonValueType::Null);
}

JsonValue::JsonValue(const std::string &str) {
    setType(JsonValueType::String);
    _value._string->assign(str);
}


JsonValue::JsonValue(const JsonArray &value) {
    setType(JsonValueType::Array);
    *_value._array = value;
}

JsonValueType JsonValue::type() const {
    return _type;
}

double JsonValue::toDouble() const {
    if (JsonValueType::Real == _type) {
        return _value._real;
    }
    return 0.0;
}


std::string JsonValue::toString() const {
    if (JsonValueType::String == _type) {
        return std::string(_value._string->begin(), _value._string->end());
    }
    return std::string();
}

bool JsonValue::toBoolean() const {
    if (JsonValueType::Boolean == _type) {
        return _value._bool;
    }
    return false;
}

JsonArray JsonValue::toArray() const {
    if (JsonValueType::Array == _type) {
        return *_value._array;
    }
    return JsonArray();
}

JsonObject JsonValue::toObject() const {
    if (JsonValueType::Object == _type) {
        return *_value._object;
    }
    return JsonObject();
}

JsonValue::JsonValue(const std::vector<char>::const_iterator &begin, const std::vector<char>::const_iterator &end) {
    setType(JsonValueType::String);
    _value._string->assign(begin, end);
}

JsonValue::~JsonValue() {
    //switch (_type) {
    //case JsonValueType::String:
    //	delete _value._string;
    //	break;
    //case JsonValueType::Array:
    //	delete _value._array;
    //	break;
    //case JsonValueType::Object:
    //	delete _value._object;
    //	break;
    //default:
    //	break;
    //}
    //_value._string = NULL;
}


JsonValue::JsonValue(int value) {
    setType(JsonValueType::Real);
    _value._real = value;
}

void JsonValue::setType(JsonValueType type) {
    //初始化
    _type = type;
    _value._string = NULL;

    static std::string emptyString = "";
    switch (_type) {
        case JsonValueType::Null:
            break;
        case JsonValueType::Real:
            _value._real = 0.0;
            break;
        case JsonValueType::String:
            _value._string = new std::string(emptyString);
            break;
        case JsonValueType::Array:
            _value._array = new JsonArray();
            break;
        case JsonValueType::Object:
            _value._object = new JsonObject();
            break;
        case JsonValueType::Boolean:
            _value._bool = false;
            break;
        default:
            break;
    }
}

void JsonObject::insert(const std::string &key, const JsonValue &value) {
    _object.insert(std::make_pair(key, value));
}


std::map<std::string, JsonValue>::const_iterator JsonObject::begin() const {
    return _object.begin();
}


std::map<std::string, JsonValue>::const_iterator JsonObject::end() const {
    return _object.end();
}

void JsonArray::append(const JsonValue &value) {
    _array.push_back(value);
}


std::list<JsonValue>::const_iterator JsonArray::begin() const {
    return _array.begin();
}

std::list<JsonValue>::const_iterator JsonArray::end() const {
    return _array.end();
}

JsonValue::JsonValue(bool value) {
    setType(JsonValueType::Boolean);
    _value._bool = value;
}

JsonValue::JsonValue(unsigned int value) {
    setType(JsonValueType::Real);
    _value._real = value;
}

JsonValue::JsonValue(uint64_t value) {
    setType(JsonValueType::Real);
    _value._real = static_cast<uint64_t>(value);
}

JsonValue::JsonValue(int64_t value) {
    setType(JsonValueType::Real);
    _value._real = static_cast<int64_t>(value);
}

JsonValue::JsonValue(double value) {
    setType(JsonValueType::Real);
    _value._real = value;
}

JsonValue::JsonValue(const std::vector<char> &value) {
    setType(JsonValueType::String);
    _value._string->assign(value.begin(), value.end());
}

JsonValue::JsonValue(const JsonObject &value) {
    setType(JsonValueType::Object);
    *_value._object = value;
}
