//
// Created by Jsiyong on 2021-02-02.
//

#ifndef CANDY_SINGLETON_H
#define CANDY_SINGLETON_H

/*
 * 单例模式的基类
 */
template<typename T>
struct Singleton {
    /**
     * 静态局部变量法
     * @return
     */
    static T *getInstance() {
        static T t;
        return &t;
    }

    //禁止拷贝
    Singleton(const Singleton &) = delete;

    Singleton(Singleton &&) = delete;

    Singleton &operator=(const Singleton &) = delete;

    Singleton &operator=(Singleton &&) = delete;

protected:
    Singleton() = default;

    ~Singleton() = default;
};

#endif //CANDY_SINGLETON_H
