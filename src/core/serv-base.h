//
// Created by jiangsiyong on 2020/12/16.
//

#ifndef CANDY_SERV_BASE_H
#define CANDY_SERV_BASE_H

#include "channel.h"
#include "selector.h"

struct ServBase {

    ServBase();

    ~ServBase();

    void start(const char *host, unsigned short port);

    void run();


private:
    int _servfd;
    Channel *_listenChannel;
    Selector *_selector;
};

#endif //CANDY_SERV_BASE_H
