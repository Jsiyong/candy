#pragma once

#include <map>
#include <list>
#include <vector>
#include <string>

struct JsonObject;
struct JsonArray;
struct JsonValue;

enum class JsonValueType {
    Null = 0,
    Real,//实数一个类型就够了，其他类型都能转成实数类型
    String,
    Boolean,
    Array,
    Object
};

/**
 * json对象
 */
struct JsonObject {

    void insert(const std::string &key, const JsonValue &value);

    std::map<std::string, JsonValue>::const_iterator begin() const;

    std::map<std::string, JsonValue>::const_iterator end() const;

private:
    std::map<std::string, JsonValue> _object;//对象
};

/**
 * json数组
 */
struct JsonArray {

    void append(const JsonValue &value);

    std::list<JsonValue>::const_iterator begin() const;

    std::list<JsonValue>::const_iterator end() const;

private:
    std::list<JsonValue> _array;//数组
};

/**
 * Json的值对象
 */
struct JsonValue {

    JsonValue();

    JsonValue(const std::string &value);

    JsonValue(const std::vector<char>::const_iterator &begin, const std::vector<char>::const_iterator &end);

    JsonValue(int value);

    JsonValue(bool value);

    JsonValue(unsigned int value);

    JsonValue(uint64_t value);

    JsonValue(int64_t value);

    JsonValue(double value);

    JsonValue(const std::vector<char> &value);

    JsonValue(const JsonObject &value);

    JsonValue(const JsonArray &value);

    JsonValueType type() const;

    double toDouble() const;

    std::string toString() const;

    bool toBoolean() const;

    JsonArray toArray() const;

    JsonObject toObject() const;

#if 0
    //赋值
    JsonValue(const JsonValue& other);
    JsonValue(JsonValue&& other) noexcept;
    JsonValue& operator=(const JsonValue& other);
    JsonValue& operator=(JsonValue&& other) noexcept;
#endif

    ~JsonValue();

private:

    void setType(JsonValueType type);

    JsonValueType _type;

    //保存值
    union ValueHolder {
        double _real;
        bool _bool;
        std::string *_string;//字符串
        JsonObject *_object;//对象
        JsonArray *_array;//数组
    } _value;
};
