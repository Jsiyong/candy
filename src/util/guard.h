//
// Created by Jsiyong on 2021-02-25.
//

#ifndef CANDY_GUARD_H
#define CANDY_GUARD_H

#include <functional>
#include <utility>

/**
 * 一些防护，比如说函数返回的时候需要主动释放文件
 */
struct ExitCaller {
    ~ExitCaller() { _functor(); }

    explicit ExitCaller(std::function<void()> functor) : _functor(std::move(functor)) {}

private:
    std::function<void()> _functor;
};

/**
 * 互斥锁的保护措施
 */
struct MutexLocker {

    explicit MutexLocker(pthread_mutex_t *mutex);

    ~MutexLocker();

    /**
     * 解锁
     */
    void unlock();

    /**
     * 重新锁上
     */
    void relock();

private:
    bool _lock;
    pthread_mutex_t *_mutex = NULL;
};

#endif //CANDY_GUARD_H
