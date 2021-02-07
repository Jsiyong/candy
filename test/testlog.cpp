//
// Created by Jsiyong on 2021-02-02.
//
#include "../src/log/logger.h"

static void *test_pid(void *) {
    info("hello new thread");
}

int main() {
    Logger *log = new AsyncLogger();
    LogAppender *logAppender = new ConsoleLogAppender();
    LogManager::getInstance()->setLogger(log);
    log->addAppender(logAppender);

    trace("world is %s", "good");
    info("log info, server config [port:%d],[addr:%s],[mode:%d]", 10, "1.2.3", 1);

    pthread_t t;
    pthread_create(&t, NULL, test_pid, NULL);

    pthread_join(t, NULL);

    return 0;
}