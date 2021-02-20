//
// Created by jiangsiyong on 2020/12/16.
//

#ifndef CANDY_SERVER_H
#define CANDY_SERVER_H

#include "channel.h"
#include "poller.h"
#include "acceptor.h"

struct Server {

    Server();

    ~Server();

    void startAt(const char *host, unsigned short port);

private:

    Acceptor *_acceptor;
};

#endif //CANDY_SERVER_H
