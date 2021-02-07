//
// Created by jiangsiyong on 2020/12/16.
//

#ifndef CANDY_SERVBASE_H
#define CANDY_SERVBASE_H

#include "channel.h"
#include "poller.h"
#include "acceptor.h"

struct ServBase {

    ServBase();

    ~ServBase();

    void startAt(const char *host, unsigned short port);

private:

    Acceptor *_acceptor;
};

#endif //CANDY_SERVBASE_H
