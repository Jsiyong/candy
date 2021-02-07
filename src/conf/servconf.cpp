//
// Created by Jsiyong on 2021-02-03.
//
#include "servconf.h"

//服务器默认配置
struct ServConf servConf = {
        .port = DEFAULT_SERV_PORT,
        .host = (char *) DEFAULT_SERV_ADDR,
        .mode = ServMode::FOREGROUND
};