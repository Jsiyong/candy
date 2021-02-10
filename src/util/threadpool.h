//
// Created by Jsiyong on 2021-02-09.
//

#ifndef CANDY_THREADPOOL_H
#define CANDY_THREADPOOL_H

#include <list>
#include <set>
#include <pthread.h>

/**
 * 接口
 */
struct Runnable {
    virtual ~Runnable();

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

#endif //CANDY_THREADPOOL_H
