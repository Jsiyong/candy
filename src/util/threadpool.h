//
// Created by Jsiyong on 2021-02-09.
//

#ifndef CANDY_THREADPOOL_H
#define CANDY_THREADPOOL_H

#include <list>
#include <set>
#include <pthread.h>
#include "singleton.h"
#include "runnable.h"

/**
 * 线程池: 采用单例模式去做
 */
struct ThreadPoolExecutor : public Singleton<ThreadPoolExecutor> {

    void startWith(int coreThreadCount, int maxThreadCount, int expiryTimeout);

    ~ThreadPoolExecutor();

    /**
     * 提交一个任务
     * @param task
     */
    void submit(Runnable *task);

private:
    /**
     * 执行的线程
     */
    struct ExecutorThread {
        explicit ExecutorThread(ThreadPoolExecutor *manager);

        ~ExecutorThread();

        static void *run(void *param);

        pthread_cond_t runnableReady;//条件变量

        pthread_t tid = 0;//当前的线程的id

        Runnable *runnable = NULL;//创建这个线程时初始化的任务
    private:
        //管理者对象
        ThreadPoolExecutor *_manager = NULL;
    };

    //创建一个新的工作线程
    void startThread(Runnable *task);

    /**
     * 是不是线程太多了，需要清理了
     * @return
     */
    bool tooManyThreads();

private:

    bool _exit = false;//线程池是否退出

    int _maxThreadCount = 0;//线程池最大数量
    int _coreThreadCount = 0;//线程池最小数量[核心线程数]
    int _expiryTimeout = 0;//失效的时间

    ///////////
    pthread_mutex_t _mutex;//临界资源的互斥锁
    //临界资源
    //////////////////////////////////////////////
    std::set<ExecutorThread *> _allThreads;//存放所有的线程
    std::list<ExecutorThread *> _waitingThreads;//所有等待工作的线程
    std::set<ExecutorThread *> _coreWaitingThread;//核心等待线程，空闲时数量应该小于等于核心线程数
    std::list<Runnable *> _tasks;//一个个的任务
    //////////////////////////////////////////////
    ///////////
};

#endif //CANDY_THREADPOOL_H
