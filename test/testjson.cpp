//
// Created by Jsiyong on 2021-02-17.
//
#include <iostream>

#include "../src/protocol/json/serialization.h"

struct CFF {
    double d;

    CFF() {}

    CFF(double d) : d(d) {}
};

Serialization(CFF, d)

struct ABC {
    int i;
    std::string s;
    std::map<int, CFF> m;
    std::vector<int> j;
    long long f;
    CFF aaa;
};

Serialization(ABC, i, s, j, f, m, aaa)

int main() {
    std::string jsonStr;
    JsonObject jsonObj;

    std::string str = "{ \"你好\":{ \"I\\\"NT\":122,\"NULL\":null , \"FALSE\": false, \"TRUE\": true},\"k1\":\"v1\",\"k2\" : \"v2\",\"k3\" : {\"j1\":\"j2\"}, \"k4\":[\"1\",{\"k2\":\"k1\"},\"3\",\"4\", { } ,[],{\"1\":1},[1],[-200],[1.222222]] }";
    //std::string str = "[\"1\",{\"k2\":\"k1\"},\"3\",\"4\",[]]";
    //std::string str = "[1]";

    JsonValue root = JsonParser::toJsonValue(str);

    jsonStr = JsonParser::toJson(root);
    jsonStr.push_back('\0');
    std::cout << jsonStr.data() << std::endl;

    //字符串转义
    jsonObj.insert("1234\"", JsonValue());
    jsonStr = JsonParser::toJson(jsonObj);
    std::cout << jsonStr.c_str() << std::endl;


    ABC abc;
    abc.j = {1, 2, 3, 4};
    abc.i = 100;
    abc.m[1] = CFF(9.99);
    abc.m[10] = CFF(20);
    abc.aaa.d = 9.99;
    abc.f = 99991111;
    abc.s = "hello-world";


    jsonObj = Serializable<ABC>::serialize(abc);
    jsonStr = JsonParser::toJson(jsonObj);
    std::cout << jsonStr.c_str() << std::endl;

    ABC t = Serializable<ABC>::deserialize(jsonObj);
    jsonObj = Serializable<ABC>::serialize(t);
    jsonStr = JsonParser::toJson(jsonObj);
    std::cout << jsonStr.c_str() << std::endl;

    return 0;
}
