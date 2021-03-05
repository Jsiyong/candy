//
// Created by Jsiyong on 2021-03-06.
//

#ifndef CANDY_RUNNABLE_H
#define CANDY_RUNNABLE_H

/**
 * 接口
 */
struct Runnable {
    virtual ~Runnable() = default;

    virtual void run() = 0;
};

#endif //CANDY_RUNNABLE_H
