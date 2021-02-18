#include "JsonValue.h"
#include <algorithm>

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
    if (JsonValueType::Double == _type) {
        return _value._real;
    }
    return 0.0;
}

long long JsonValue::toLongLong() const {
    if (JsonValueType::LongLong == _type) {
        return _value._number;
    }
    return 0;
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
    switch (_type) {
        case JsonValueType::String:
            delete _value._string;
            break;
        case JsonValueType::Array:
            delete _value._array;
            break;
        case JsonValueType::Object:
            delete _value._object;
            break;
        default:
            break;
    }
    _value._string = NULL;
}

void JsonValue::setType(JsonValueType type) {
    //初始化
    _type = type;
    _value._string = NULL;

    static std::string emptyString = "";
    switch (_type) {
        case JsonValueType::Null:
            break;
        case JsonValueType::Double:
            _value._real = 0.0;
            break;
        case JsonValueType::LongLong:
            _value._real = 0;
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

bool JsonObject::contains(const std::string &key) const {
    return _object.find(key) != _object.end();
}

const JsonValue &JsonObject::operator[](const std::string &key) const {
    auto it = _object.find(key);
    return (*it).second;
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

JsonValue::JsonValue(long long value) {
    setType(JsonValueType::LongLong);
    _value._number = value;
}

JsonValue::JsonValue(double value) {
    setType(JsonValueType::Double);
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

JsonValue::JsonValue(const JsonValue &other) {
    //负责other的内容到this中
    _type = other.type();
    switch (_type) {
        case JsonValueType::Null:
        case JsonValueType::Double:
        case JsonValueType::LongLong:
        case JsonValueType::Boolean:
            _value = other._value;
            break;
        case JsonValueType::String:
            _value._string = new std::string(*other._value._string);
            break;
        case JsonValueType::Array:
            _value._array = new JsonArray(*other._value._array);
            break;
        case JsonValueType::Object:
            _value._object = new JsonObject(*other._value._object);
            break;
        default:
            break;
    }
}

JsonValue::JsonValue(JsonValue &&other) noexcept {
    swap(other);
}

void JsonValue::swap(JsonValue &other) {
    //交换类型和值
    std::swap(_value, other._value);
    std::swap(_type, other._type);
}

JsonValue &JsonValue::operator=(const JsonValue &other) {
    JsonValue(other).swap(*this);
    return *this;
}

JsonValue &JsonValue::operator=(JsonValue &&other) noexcept {
    other.swap(*this);
    return *this;
}
