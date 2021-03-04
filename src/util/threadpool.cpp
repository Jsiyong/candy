//
// Created by Jsiyong on 2021-02-09.
//
#include "threadpool.h"
#include "../log/logger.h"
#include "guard.h"
#include <unistd.h>
#include <sys/time.h>

#if 0
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
//        trace("idle num:%d, work threadNum:%d, taskNum:%d", idleThreadNum, workThreadNum, taskNum);

        //如果任务数量小于最大线程数且大于空闲线程数，那么就创建
        //最多还能创建的线程数
        int maxCanCreateNum = pool->_maxNum - workThreadNum;
        //想要创建的线程数
        int wantCreateNum = taskNum > idleThreadNum ? taskNum - idleThreadNum : 0;
        //计算之后最后能创建的线程数
        int newCreateThreadNum = maxCanCreateNum > wantCreateNum ? wantCreateNum : maxCanCreateNum;
//        trace("newCreateThreadNum:%d", newCreateThreadNum);
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

    trace("create a new thread. workThreadNum:%d", _workThreadCount);
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

#endif

ThreadPoolExecutor::ThreadPoolExecutor(int coreNum, int maxNum, int expiryTimeout) {
    _coreNum = coreNum;
    _maxNum = maxNum;
    _expiryTimeout = expiryTimeout;

    //变量初始化
    pthread_mutex_init(&_mutex, NULL);
}

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
    if (_allThreads.size() < _maxNum) {
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
        usleep(20);

        MutexLocker locker(&_mutex);
        threadNum = _allThreads.size();
        for (auto thread:_allThreads) {
            pthread_cond_signal(&thread->runnableReady);
        }
    } while (threadNum > 0);
    trace("threadpool deleted!!");
}

bool ThreadPoolExecutor::tooManyThreads() {
    //如果等待线程数大于任务数，并且线程数比核心线程数多，那么就是太多线程了
    return _waitingThreads.size() > _tasks.size() && _allThreads.size() > _coreNum;
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
            pthread_cond_wait(&_this->runnableReady, &_this->_manager->_mutex);
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
