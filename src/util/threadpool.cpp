//
// Created by Jsiyong on 2021-02-09.
//
#include "threadpool.h"
#include "../log/logger.h"
#include "guard.h"
#include <unistd.h>

void ThreadPoolExecutor::submit(Runnable *task) {

    MutexLocker locker(&_mutex);
//    trace("current thread_num[%d], waiting_num[%d], task_num[%d]", _allThreads.size(), _waitingThreads.size(),
//          _tasks.size());
    //判断是不是有线程，没有的话就创一个
    if (_allThreads.empty()) {
        startThread(task);
        return;
    }
    //判断是不是有空闲线程，有的话就直接传给这个线程，唤醒它工作
    if (!_waitingThreads.empty()) {
        ExecutorThread *waitingThread = _waitingThreads.front();
        waitingThread->runnable = task;//这个任务就给它做了
        _waitingThreads.pop_front();//移除出等待队列
        //唤醒这个线程做任务了
        pthread_cond_signal(&waitingThread->runnableReady);
        return;
    }

    //如果没有线程可用了，而且线程数小于配置的最大线程数，那么就创建一个线程来用
    if (_allThreads.size() < _maxThreadCount) {
        startThread(task);
        return;
    }

    //下面的情况是不能创建新的线程的，就将任务加入等待队列中，让所有的线程去抢这个任务
    _tasks.push_back(task);
}

void ThreadPoolExecutor::startThread(Runnable *task) {

    pthread_attr_t attr;
    auto pThread = new ExecutorThread(this);
    pThread->runnable = task;//先加入任务，避免创建线程的时候就开始操作任务了，但是任务却没有加上去

    //设置线程分离属性
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&pThread->tid, &attr, &ExecutorThread::run, pThread);

    //加入一个线程
    _allThreads.emplace(pThread);
    trace("start a new thread, thread_num[%d], waiting_num[%d], task_num[%d]", _allThreads.size(),
          _waitingThreads.size(), _tasks.size());
}

ThreadPoolExecutor::~ThreadPoolExecutor() {
    _exit = true;
    //确保所有的线程都已经释放完毕
    int threadNum = 0;
    do {
        usleep(100);

        MutexLocker locker(&_mutex);
        threadNum = _allThreads.size();
        for (auto thread:_allThreads) {
            pthread_cond_signal(&thread->runnableReady);
        }
    } while (threadNum > 0);

    pthread_mutex_destroy(&_mutex);

    trace("threadpool deleted!!");
}

bool ThreadPoolExecutor::tooManyThreads() {
    //还有线程需要分出来做任务，所以不算太多线程
    if (_waitingThreads.size() <= _tasks.size()) {
        return false;
    }
    //核心线程，一直保持coreNum条线程在等待
    return _coreWaitingThread.size() > _coreThreadCount;
    //其他情况就是太多了
}

void ThreadPoolExecutor::startWith(int maxThreadCount, int coreThreadCount, int expiryTimeout) {
    _coreThreadCount = coreThreadCount;//最大线程数不能小于核心线程数
    _maxThreadCount = std::max(maxThreadCount, coreThreadCount);//最大线程数不能小于核心线程数
    _expiryTimeout = expiryTimeout;

    //变量初始化
    pthread_mutex_init(&_mutex, NULL);
}

void *ThreadPoolExecutor::ExecutorThread::run(void *param) {
    auto _this = (ExecutorThread *) param;

    MutexLocker locker(&_this->_manager->_mutex);
    //当线程没有退出，就继续运行
    while (!_this->_manager->_exit) {
        Runnable *task = _this->runnable;//这个不用加锁，因为前面已经保证会加入这个任务再创建这个线程
        _this->runnable = NULL;

        do {
            locker.unlock();

            task->run();//开始运行任务

            locker.relock();//重新锁上

            //判断线程数是不是太多了，是的话就准备失效这个线程
            if (_this->_manager->tooManyThreads()) {
                //当前线程数太多了，当前线程得准备跑路了
                break;
            }
            //没有任务做了，后面需要放到等待队列里面
            if (_this->_manager->_tasks.empty()) {
                break;
            }
            //有任务做，就开始取下一个任务做
            //取出队首任务，开始做下一个任务
            task = _this->_manager->_tasks.front();
            _this->_manager->_tasks.pop_front();//记得出队
        } while (true);
        //判断是不是线程太多了，太多了就放入等待队列里面，并且等待一定的时间直到过期
        //如果线程没有太多，那么就无限等待，直到有任务来
        //放入等待队列中
        bool tooManyThreads = _this->_manager->tooManyThreads();
        _this->_manager->_waitingThreads.push_back(_this);//记住要判断完之后再加入等待线程
        if (tooManyThreads) {
            struct timeval now{};//当前时间
            struct timespec timespec{};//超时时间
            gettimeofday(&now, NULL);
            timespec.tv_sec = now.tv_sec + _this->_manager->_expiryTimeout;
            timespec.tv_nsec = now.tv_usec * 1000;
            //等待超时
            if (0 != pthread_cond_timedwait(&_this->runnableReady, &_this->_manager->_mutex, &timespec)) {
                //若时间到了，超时了，就直接移除掉这个指针，结束当前线程
                break;
            }
        } else {
            //等到了，就继续跑，没等到，就一直阻塞
            _this->_manager->_coreWaitingThread.emplace(_this);
            pthread_cond_wait(&_this->runnableReady, &_this->_manager->_mutex);
            _this->_manager->_coreWaitingThread.erase(_this);
        }
    }
    //线程退出前，清理空间
    _this->_manager->_waitingThreads.remove(_this);
    _this->_manager->_allThreads.erase(_this);
    //删除这个对象
    delete _this;

    trace("release a thread, thread_num[%d], waiting_num[%d], task_num[%d]", _this->_manager->_allThreads.size(),
          _this->_manager->_waitingThreads.size(), _this->_manager->_tasks.size());

    pthread_exit(NULL);
}

ThreadPoolExecutor::ExecutorThread::ExecutorThread(ThreadPoolExecutor *manager) : _manager(manager) {
    //初始化条件变量
    pthread_cond_init(&runnableReady, NULL);
}

ThreadPoolExecutor::ExecutorThread::~ExecutorThread() {
    pthread_cond_destroy(&runnableReady);//移除条件变量
}
