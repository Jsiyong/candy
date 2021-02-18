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
    LongLong,//整型[64位的]
    Double,//实数
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

    bool contains(const std::string &key) const;

    const JsonValue &operator[](const std::string &key) const;

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

    JsonValue(long long value);

    JsonValue(bool value);

    JsonValue(double value);

    JsonValue(const std::vector<char> &value);

    JsonValue(const JsonObject &value);

    JsonValue(const JsonArray &value);

    JsonValueType type() const;

    double toDouble() const;

    long long toLongLong() const;

    std::string toString() const;

    bool toBoolean() const;

    JsonArray toArray() const;

    JsonObject toObject() const;

    //赋值
    JsonValue(const JsonValue &other);

    JsonValue(JsonValue &&other) noexcept;

    JsonValue &operator=(const JsonValue &other);

    JsonValue &operator=(JsonValue &&other) noexcept;

    ~JsonValue();

private:

    /**
     * 交换指针内容
     * @param other
     */
    void swap(JsonValue &other);

    void setType(JsonValueType type);

    JsonValueType _type;

    //保存值
    union ValueHolder {
        double _real;
        long long _number;
        bool _bool;
        std::string *_string;//字符串
        JsonObject *_object;//对象
        JsonArray *_array;//数组
    } _value;
};
