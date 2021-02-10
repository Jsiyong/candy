//
// Created by Jsiyong on 2021-02-09.
//
#include "threadpool.h"
#include "../log/logger.h"
#include <unistd.h>

ThreadPoolExecutor::ThreadPoolExecutor(int coreNum, int maxNum, int expiryTimeout) {
    _coreNum = coreNum;
    _maxNum = maxNum;
    _expiryTimeout = expiryTimeout;

    //变量初始化
    pthread_mutex_init(&_mutex, NULL);
    pthread_cond_init(&_cond, NULL);

    //创建工作线程
    for (int i = 0; i < _coreNum; ++i) {
        createNewWorkThread();
    }
    //创建管理者线程
    pthread_create(&_manageThread, NULL, &ThreadPoolExecutor::processManageWork, this);
}

void *ThreadPoolExecutor::processTask(void *param) {
    ThreadPoolExecutor *pool = (ThreadPoolExecutor *) param;
    //线程池还没有退出的时候执行这个循环
    while (!pool->_exit) {
        pthread_mutex_lock(&pool->_mutex);
        pool->_idleNum++;//处理任务前，空闲线程加一

        while (pool->_tasks.empty()) {
            //只要任务为空，就阻塞等待任务不为空
            pthread_cond_wait(&pool->_cond, &pool->_mutex);
            //被唤醒后，判断是不是需要退出线程，是的话，就退出线程
            if (pool->isNeeded2CancelWorkThread(pool->_tasks.size(), pool->_workThreadCount, pool->_idleNum)) {
                //重新赋值现在的工作线程信息
                pool->_idleNum--;//空闲线程减一
                pool->_workThreadCount--;//工作线程减一
                pthread_mutex_unlock(&(pool->_mutex));
                info("thread[%lld] is exiting. idleNum:%d, workNum:%d", pthread_self(), pool->_idleNum,
                     pool->_workThreadCount);
                pthread_exit(NULL);
            }
        }
        //取出队首
        Runnable *task = pool->_tasks.front();
        pool->_tasks.pop_front();
        //准备处理任务
        pool->_idleNum--;//空闲线程减一
        pthread_mutex_unlock(&pool->_mutex);

        //开始执行任务
        task->run();
    }
    pthread_mutex_lock(&pool->_mutex);
    pool->_workThreadCount--;
    pool->_idleNum--;
    pthread_mutex_unlock(&pool->_mutex);
    pthread_exit(NULL);
}

void *ThreadPoolExecutor::processManageWork(void *param) {
    ThreadPoolExecutor *pool = (ThreadPoolExecutor *) param;

    while (!pool->_exit) {
        sleep(pool->_expiryTimeout);
        pthread_mutex_lock(&(pool->_mutex));
        int taskNum = pool->_tasks.size();//任务数量
        int workThreadNum = pool->_workThreadCount;
        int idleThreadNum = pool->_idleNum;
        pthread_mutex_unlock(&(pool->_mutex));
        trace("idle num:%d, work threadNum:%d, taskNum:%d", idleThreadNum, workThreadNum, taskNum);

        //如果任务数量小于最大线程数且大于空闲线程数，那么就创建
        //最多还能创建的线程数
        int maxCanCreateNum = pool->_maxNum - workThreadNum;
        //想要创建的线程数
        int wantCreateNum = taskNum > idleThreadNum ? taskNum - idleThreadNum : 0;
        //计算之后最后能创建的线程数
        int newCreateThreadNum = maxCanCreateNum > wantCreateNum ? wantCreateNum : maxCanCreateNum;
        trace("newCreateThreadNum:%d", newCreateThreadNum);
        //创建新线程
        for (int i = 0; i < newCreateThreadNum; ++i) {
            pool->createNewWorkThread();
        }

        //如果存在空闲的线程并且工作线程数比核心线程数多，那么就准备释放掉多余的空闲线程
        if (pool->isNeeded2CancelWorkThread(taskNum, workThreadNum, idleThreadNum)) {
            pthread_cond_broadcast(&pool->_cond);
        }
    }
    pthread_exit(NULL);
}

void ThreadPoolExecutor::submit(Runnable *task) {
    pthread_mutex_lock(&_mutex);
    _tasks.push_back(task);
    pthread_mutex_unlock(&_mutex);
    //若没有线程处于等待状态，则不受影响
    pthread_cond_broadcast(&_cond);//发出广播，让所有线程都去抢这个任务
}

void ThreadPoolExecutor::createNewWorkThread() {

    pthread_mutex_lock(&_mutex);
    _workThreadCount++;//工作线程数++
    pthread_mutex_unlock(&_mutex);
    pthread_t pid;
    pthread_attr_t attr;
    //设置线程分离属性
    int ret = pthread_attr_init(&attr);
    exit_if(ret != 0, "pthread_init error:%s", strerror(ret));
    ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    exit_if(ret != 0, "pthread_attr_setdetachstate error:%s", strerror(ret));
    ret = pthread_create(&pid, &attr, &ThreadPoolExecutor::processTask, this);
    exit_if(ret != 0, "pthread_create error:%s", strerror(ret));

    warn("create a new thread. workThreadNum:%d, taskNum:%d", _workThreadCount, _tasks.size());
}

bool ThreadPoolExecutor::isNeeded2CancelWorkThread(int taskNum, int workThreadNum, int idleThreadNum) {
    return _exit || (taskNum == 0 && idleThreadNum > 0 && workThreadNum > _coreNum);
}

ThreadPoolExecutor::~ThreadPoolExecutor() {
    _exit = true;
    //广播，告诉所有工作线程要退出线程了
    pthread_cond_broadcast(&_cond);
    //让管理线程正常退出
    pthread_join(_manageThread, NULL);
    //确保所有的线程都已经释放完毕
    int threadNum = 0;
    do {
        usleep(20);
        pthread_mutex_lock(&_mutex);
        threadNum = _workThreadCount;
        pthread_mutex_unlock(&_mutex);
    } while (threadNum > 0);

    pthread_mutex_destroy(&_mutex);
    pthread_cond_destroy(&_cond);
    trace("pthread release ok!!");
}

Runnable::~Runnable() {

}
