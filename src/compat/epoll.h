//
// Portable epoll API: native on Linux, kqueue-backed on macOS.
//

#ifndef CANDY_COMPAT_EPOLL_H
#define CANDY_COMPAT_EPOLL_H

#if defined(__APPLE__)

#include <stdint.h>
#include <sys/types.h>

enum {
    EPOLL_CTL_ADD = 1,
    EPOLL_CTL_DEL = 2,
    EPOLL_CTL_MOD = 3
};

enum {
    EPOLLIN = 0x001,
    EPOLLOUT = 0x004,
    EPOLLERR = 0x008,
    EPOLLHUP = 0x010,
    EPOLLRDHUP = 0x2000,
    EPOLLONESHOT = 1u << 30,
    EPOLLET = 1u << 31
};

typedef union epoll_data {
    void *ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;

struct epoll_event {
    uint32_t events;
    epoll_data_t data;
};

int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

#else

#include <sys/epoll.h>

#endif

#endif // CANDY_COMPAT_EPOLL_H
