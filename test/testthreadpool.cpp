//
// Created by Jsiyong on 2021-02-10.
//

#include "../src/util/threadpool.h"
#include "../src/log/logger.h"
#include <unistd.h>

struct Task : public Runnable {
    Task(int i) : i(i) {}

    void run() {
        sleep(5);
        warn("task[%d] ok!!", i);
    }

private:
    int i;
};

int main() {

    Logger *pLogger = new AsyncLogger();
    LogAppender *pLogAppender = new ConsoleLogAppender();
    pLogger->addAppender(pLogAppender);
    LogManager::getInstance()->setLogger(pLogger);

    ThreadPoolExecutor *executor = ThreadPoolExecutor::getInstance();
    executor->startWith(1, 2, 10);

    for (int i = 0; i < 2; ++i) {
        Task *t = new Task(i);
        executor->submit(t);
    }

    sleep(100);
    for (int i = 20; i < 30; ++i) {
        Task *t = new Task(i);
        executor->submit(t);
        sleep(1);
    }

    delete executor;
    delete pLogger;
    sleep(10);
    return 0;
}