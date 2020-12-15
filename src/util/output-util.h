//
// Created by jiangsiyong on 2020/12/15.
//

#ifndef CANDY_OUTPUT_UTIL_H
#define CANDY_OUTPUT_UTIL_H

#include <stdio.h>
#include <stdlib.h>

#define err_exit_if(cond, ...)\
    if(cond) {\
        fprintf(stderr, __VA_ARGS__);\
        exit(1);\
    }

#endif //CANDY_OUTPUT_UTIL_H
