//
// Created by Jsiyong on 2021-02-18.
//

#ifndef CANDY_SERIALIZATION_H
#define CANDY_SERIALIZATION_H

#include "jsonparser.h"
#include <sstream>
#include <string.h>

#define ARG_EXPAND(...)  __VA_ARGS__
#define ARG_1(arg, ...)  std::make_pair(std::string(#arg),&t.arg)
#define ARG_2(arg, ...)  std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_1(__VA_ARGS__))
#define ARG_3(arg, ...)  std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_2(__VA_ARGS__))
#define ARG_4(arg, ...)  std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_3(__VA_ARGS__))
#define ARG_5(arg, ...)  std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_4(__VA_ARGS__))
#define ARG_6(arg, ...)  std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_5(__VA_ARGS__))
#define ARG_7(arg, ...)  std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_6(__VA_ARGS__))
#define ARG_8(arg, ...)  std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_7(__VA_ARGS__))
#define ARG_9(arg, ...)  std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_8(__VA_ARGS__))
#define ARG_10(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_9(__VA_ARGS__))
#define ARG_11(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_10(__VA_ARGS__))
#define ARG_12(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_11(__VA_ARGS__))
#define ARG_13(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_12(__VA_ARGS__))
#define ARG_14(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_13(__VA_ARGS__))
#define ARG_15(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_14(__VA_ARGS__))
#define ARG_16(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_15(__VA_ARGS__))
#define ARG_17(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_16(__VA_ARGS__))
#define ARG_18(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_17(__VA_ARGS__))
#define ARG_19(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_18(__VA_ARGS__))
#define ARG_20(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_19(__VA_ARGS__))
#define ARG_21(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_20(__VA_ARGS__))
#define ARG_22(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_21(__VA_ARGS__))
#define ARG_23(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_22(__VA_ARGS__))
#define ARG_24(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_23(__VA_ARGS__))
#define ARG_25(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_24(__VA_ARGS__))
#define ARG_26(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_25(__VA_ARGS__))
#define ARG_27(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_26(__VA_ARGS__))
#define ARG_28(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_27(__VA_ARGS__))
#define ARG_29(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_28(__VA_ARGS__))
#define ARG_30(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_29(__VA_ARGS__))
#define ARG_31(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_30(__VA_ARGS__))
#define ARG_32(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_31(__VA_ARGS__))
#define ARG_33(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_32(__VA_ARGS__))
#define ARG_34(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_33(__VA_ARGS__))
#define ARG_35(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_34(__VA_ARGS__))
#define ARG_36(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_35(__VA_ARGS__))
#define ARG_37(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_36(__VA_ARGS__))
#define ARG_38(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_37(__VA_ARGS__))
#define ARG_39(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_38(__VA_ARGS__))
#define ARG_40(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_39(__VA_ARGS__))
#define ARG_41(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_40(__VA_ARGS__))
#define ARG_42(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_41(__VA_ARGS__))
#define ARG_43(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_42(__VA_ARGS__))
#define ARG_44(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_43(__VA_ARGS__))
#define ARG_45(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_44(__VA_ARGS__))
#define ARG_46(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_45(__VA_ARGS__))
#define ARG_47(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_46(__VA_ARGS__))
#define ARG_48(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_47(__VA_ARGS__))
#define ARG_49(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_48(__VA_ARGS__))
#define ARG_50(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_49(__VA_ARGS__))
#define ARG_51(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_50(__VA_ARGS__))
#define ARG_52(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_51(__VA_ARGS__))
#define ARG_53(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_52(__VA_ARGS__))
#define ARG_54(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_53(__VA_ARGS__))
#define ARG_55(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_54(__VA_ARGS__))
#define ARG_56(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_55(__VA_ARGS__))
#define ARG_57(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_56(__VA_ARGS__))
#define ARG_58(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_57(__VA_ARGS__))
#define ARG_59(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_58(__VA_ARGS__))
#define ARG_60(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_59(__VA_ARGS__))
#define ARG_61(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_60(__VA_ARGS__))
#define ARG_62(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_61(__VA_ARGS__))
#define ARG_63(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_62(__VA_ARGS__))
#define ARG_64(arg, ...) std::make_pair(std::string(#arg),&t.arg),ARG_EXPAND(ARG_63(__VA_ARGS__))

#define RSEQ_N() 64,63,62,61,60,                \
                 59,58,57,56,55,54,53,52,51,50, \
                 49,48,47,46,45,44,43,42,41,40, \
                 39,38,37,36,35,34,33,32,31,30, \
                 29,28,27,26,25,24,23,22,21,20, \
                 19,18,17,16,15,14,13,12,11,10, \
                 9,8,7,6,5,4,3,2,1,0

#define   ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
                _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
                _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
                _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
                _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
                _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
                _61, _62, _63, _64, N, ...) N


#define ARG_COUNT_INNER(...)    ARG_EXPAND(ARG_N(__VA_ARGS__))
#define GET_ARG_COUNT(...)      ARG_COUNT_INNER(__VA_ARGS__,RSEQ_N())
#define ARG_LIST(N, arg, ...)   ARG##_##N(arg,__VA_ARGS__)
#define GEN_FUNC(N, ...)        ARG_EXPAND(ARG_LIST(N, __VA_ARGS__))

#define SERIALIZATION_INNER(Class, Func)                                                                                                      \
template<>                                                                                                                                    \
struct Serializable<Class> {                                                                                                                  \
    static JsonObject serialize(const Class &t) { return JsonHelper::serialize(std::make_tuple(Func)); }                                      \
    static Class deserialize(const JsonObject &jsonObject) { Class t; JsonHelper::deserialize(std::make_tuple(Func), jsonObject); return t; } \
};

/**
 * Class 类名
 * ... 字段名
 */
#define Serialization(Class, ...) SERIALIZATION_INNER(Class, GEN_FUNC(GET_ARG_COUNT(__VA_ARGS__),__VA_ARGS__))

/**
 * 对象序列化
 */
template<typename T>
struct Serializable {
    /**
     * 序列化一个对象
     * @return
     */
    static JsonValue serialize(const T &t) { return JsonValue(); }

    /**
     * 反序列化为对象
     * @param jsonObj
     * @return
     */
    static T deserialize(const JsonObject &jsonObject) { return T(); }
};

class JsonHelper {
private:
    template<typename Tuple, size_t N>
    struct DataGetter {
        static void genJsonObject(const Tuple &fieldTuple, JsonObject &jsonObject) {
            DataGetter<Tuple, N - 1>::genJsonObject(fieldTuple, jsonObject);
            auto data = std::get<N - 1>(fieldTuple);
            std::string key = data.first;
            auto value = *data.second;
            jsonObject.insert(key, toJsonValue(value));
        }
    };

    template<typename Tuple>
    struct DataGetter<Tuple, 1> {
        static void genJsonObject(const Tuple &fieldTuple, JsonObject &jsonObject) {
            auto data = std::get<0>(fieldTuple);
            std::string key = data.first;
            auto value = *data.second;
            jsonObject.insert(key, toJsonValue(value));
        }
    };

    template<typename Tuple, size_t N>
    struct DataSetter {
        static void getValueFromJsonObject(const Tuple &fieldTuple, const JsonObject &jsonObject) {
            DataSetter<Tuple, N - 1>::getValueFromJsonObject(fieldTuple, jsonObject);
            auto data = std::get<N - 1>(fieldTuple);
            std::string key = data.first;
            if (jsonObject.contains(key)) {
                setValueByJson(data.second, jsonObject[key]);
            }
        }
    };

    template<typename Tuple>
    struct DataSetter<Tuple, 1> {
        static void getValueFromJsonObject(const Tuple &fieldTuple, const JsonObject &jsonObject) {
            auto data = std::get<0>(fieldTuple);
            std::string key = data.first;
            if (jsonObject.contains(key)) {
                setValueByJson(data.second, jsonObject[key]);
            }
        }
    };

public:
    static void setValue(int *field, const JsonValue &jsonValue) {
        *field = (int) jsonValue.toLongLong();
    }

    static void setValue(std::string *field, const JsonValue &jsonValue) {
        *field = jsonValue.toString();
    }

    static void setValue(bool *field, const JsonValue &jsonValue) {
        *field = jsonValue.toBoolean();
    }

    static void setValue(long long *field, const JsonValue &jsonValue) {
        *field = jsonValue.toLongLong();
    }

    static void setValue(double *field, const JsonValue &jsonValue) {
        *field = jsonValue.toDouble();
    }

    template<typename T>
    static void setValue(T *field, const JsonValue &jsonValue) {
        *field = Serializable<T>::deserialize(jsonValue.toObject());
    }

public:

    template<typename T>
    static void convertValueFromString(const std::string &str, T &v) {
        std::stringstream ss;
        ss << str;
        ss >> v;
    }

    static void convertValueFromString(const std::string &str, std::string &v) { v = str; }

    template<typename T>
    static void convertValueToString(const T &v, std::string &str) {
        std::stringstream ss;
        ss << v;
        ss >> str;
    }

    static void convertValueToString(const std::string &v, std::string &str) { str = v; }

    template<typename T>
    static void setValueByJson(T *field, const JsonValue &jsonValue) {
        setValue(field, jsonValue);
    }

    template<typename T>
    static void setValueByJson(std::vector<T> *field, const JsonValue &jsonValue) {
        for (const auto &v:jsonValue.toArray()) {
            T t;
            setValueByJson(&t, v);
            field->push_back(t);
        }
    }

    template<typename T>
    static void setValueByJson(std::list<T> *field, const JsonValue &jsonValue) {
        for (const auto &v:jsonValue.toArray()) {
            T t;
            setValueByJson(&t, v);
            field->push_back(t);
        }
    }

    template<typename K, typename V>
    static void setValueByJson(std::map<K, V> *field, const JsonValue &jsonValue) {
        for (const auto &kv : jsonValue.toObject()) {
            K k;
            convertValueFromString(kv.first, k);
            V v;
            setValueByJson(&v, kv.second);
            field->insert(std::make_pair(k, v));
        }
    }

public:

    static JsonValue genJsonValue(bool value) {
        return JsonValue(value);
    }

    static JsonValue genJsonValue(long long value) {
        return JsonValue(value);
    }

    static JsonValue genJsonValue(int value) {
        return JsonValue(value);
    }

    static JsonValue genJsonValue(const std::string &value) {
        return JsonValue(value);
    }

    static JsonValue genJsonValue(double value) {
        return JsonValue(value);
    }

    template<typename T>
    static JsonValue genJsonValue(const T &value) {
        return Serializable<T>::serialize(value);
    }

public:

    template<typename T>
    static JsonValue toJsonValue(const T &value) {
        return genJsonValue(value);
    }

    template<typename T>
    static JsonValue toJsonValue(const std::list<T> &values) {
        JsonArray jsonArray;
        for (const auto &v : values) {
            jsonArray.append(toJsonValue(v));
        }
        return jsonArray;
    }

    template<typename T>
    static JsonValue toJsonValue(const std::vector<T> &values) {
        JsonArray jsonArray;
        for (const auto &v : values) {
            jsonArray.append(toJsonValue(v));
        }
        return jsonArray;
    }

    template<typename K, typename V>
    static JsonValue toJsonValue(const std::map<K, V> &values) {
        JsonObject jsonObject;
        for (const auto &kv : values) {
            std::string key;
            convertValueToString(kv.first, key);//key需要转为字符串
            jsonObject.insert(key, toJsonValue(kv.second));
        }
        return jsonObject;
    }

public:
    template<typename... Args>
    static JsonObject serialize(const std::tuple<Args...> &fieldTuple) {
        JsonObject jsonObject;
        DataGetter<decltype(fieldTuple), sizeof...(Args)>::genJsonObject(fieldTuple, jsonObject);
        return jsonObject;
    }

    template<typename... Args>
    static void deserialize(const std::tuple<Args...> &fieldTuple, const JsonObject &jsonObject) {
        DataSetter<decltype(fieldTuple), sizeof...(Args)>::getValueFromJsonObject(fieldTuple, jsonObject);
    }
};

#endif //CANDY_SERIALIZATION_H
