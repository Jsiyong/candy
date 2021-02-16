//
// Created by Jsiyong on 2021-02-17.
//
#include <iostream>
#include "../src/json/JsonParser.h"

int main() {

    std::string str = "{ \"k0\":{ \"INT\":122,\"NULL\":null , \"FALSE\": false, \"TRUE\": true},\"k1\":\"v1\",\"k2\" : \"v2\",\"k3\" : {\"j1\":\"j2\"}, \"k4\":[\"1\",{\"k2\":\"k1\"},\"3\",\"4\"] }";

    //json解析为JsonValue
    std::vector<char> json(str.begin(), str.end());
    JsonValue root;
    bool ret = JsonParser::parse(json, root);

    //JsonValue解析为json
    std::vector<char> jsonStr;
    JsonParser::parse(root, jsonStr);
    jsonStr.push_back('\0');
    std::cout << jsonStr.data() << std::endl;

    return 0;
}