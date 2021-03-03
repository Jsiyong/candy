//
// Created by Jsiyong on 2021-03-04.
//

#include "guard.h"

MutexLocker::MutexLocker(pthread_mutex_t *mutex) : _mutex(mutex) {
    relock();
}

MutexLocker::~MutexLocker() {
    if (_lock) {
        pthread_mutex_unlock(_mutex);
    }
}

void MutexLocker::unlock() {
    pthread_mutex_unlock(_mutex);
    _lock = false;
}

void MutexLocker::relock() {
    pthread_mutex_lock(_mutex);
    _lock = true;
}
