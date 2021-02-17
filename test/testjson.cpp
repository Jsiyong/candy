//
// Created by Jsiyong on 2021-02-17.
//
#include <iostream>
#include "../src/json/JsonParser.h"


int main() {

    std::string str = "{ \"你好\":{ \"I\\\"NT\":122,\"NULL\":null , \"FALSE\": false, \"TRUE\": true},\"k1\":\"v1\",\"k2\" : \"v2\",\"k3\" : {\"j1\":\"j2\"}, \"k4\":[\"1\",{\"k2\":\"k1\"},\"3\",\"4\",[],[],{\"1\":1},[1]] }";
    //std::string str = "[\"1\",{\"k2\":\"k1\"},\"3\",\"4\",[]]";
    //std::string str = "[1]";
    std::vector<char> json(str.begin(), str.end());

    JsonValue root = JsonParser::parse(json);

    std::vector<char> jsonStr = JsonParser::parse(root);
    jsonStr.push_back('\0');
    std::cout << jsonStr.data() << std::endl;

    //字符串转义
    JsonObject jsonObj;
    jsonObj.insert("1234\"", JsonValue());
    jsonStr = JsonParser::parse(jsonObj);
    jsonStr.push_back('\0');
    std::cout << jsonStr.data() << std::endl;

    return 0;
}
