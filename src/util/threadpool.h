//
// Created by Jsiyong on 2021-02-09.
//

#ifndef CANDY_THREADPOOL_H
#define CANDY_THREADPOOL_H

#include <list>
#include <set>
#include <pthread.h>

#if 0
/**
 * 接口
 */
struct Runnable {
    virtual ~Runnable() = default;

    virtual void run() = 0;
};

/**
 * 线程池
 */
struct ThreadPoolExecutor {


    ThreadPoolExecutor(int coreNum, int maxNum, int expiryTimeout);

    ~ThreadPoolExecutor();

    /**
     * 提交一个任务
     * @param task
     */
    void submit(Runnable *task);

private:

    /**
     * 线程处理函数
     * @param param
     * @return
     */
    static void *processTask(void *param);

    /**
     * 管理者线程
     * @param param
     * @return
     */
    static void *processManageWork(void *param);

    //创建一个新的工作线程
    void createNewWorkThread();

    /**
     * 判断是不是需要取消工作线程
     * @param taskNum
     * @param workThreadNum
     * @param idleThreadNum
     * @return
     */
    bool isNeeded2CancelWorkThread(int taskNum, int workThreadNum, int idleThreadNum);

private:

    bool _exit = false;//线程池是否退出

    int _maxNum = 0;//线程池最大数量
    int _coreNum = 0;//线程池最小数量[核心线程数]
    int _expiryTimeout = 0;//失效的时间

    //临界资源
    //////////////////////////////////////////////
    int _idleNum = 0;//空闲线程的数量[最多维持和_coreNum数量一样]
    int _workThreadCount = 0;//工作线程数量
    std::list<Runnable *> _tasks;//一个个的任务
    //////////////////////////////////////////////


    pthread_t _manageThread;//管理线程[周期性负责管理线程的数量]

    pthread_mutex_t _mutex;//_tasks的互斥锁
    pthread_cond_t _cond;//条件变量
};

#endif

/**
 * 接口
 */
struct Runnable {
    virtual ~Runnable() = default;

    virtual void run() = 0;
};

/**
 * 线程池
 */
struct ThreadPoolExecutor {


    ThreadPoolExecutor(int coreNum, int maxNum, int expiryTimeout);

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

    int _maxNum = 0;//线程池最大数量
    int _coreNum = 0;//线程池最小数量[核心线程数]
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
