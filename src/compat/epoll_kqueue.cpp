//
// Created for macOS: map the project's epoll usage onto kqueue.
// Linux builds do not compile this translation unit's bodies.
//

#if defined(__APPLE__)

#include "epoll.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>

static uint16_t kqueueFlagsFromEpoll(uint32_t events) {
    uint16_t flags = EV_ADD;
    if (events & EPOLLET) {
        flags |= EV_CLEAR;
    }
    if (events & EPOLLONESHOT) {
        flags |= EV_ONESHOT;
    }
    return flags;
}

static int kqueueDeleteFilters(int kq, int fd) {
    struct kevent ch[2];
    EV_SET(&ch[0], fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    EV_SET(&ch[1], fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    // A oneshot event is already gone after delivery; ENOENT is not fatal.
    (void) kevent(kq, ch, 2, NULL, 0, NULL);
    errno = 0;
    return 0;
}

static int kqueueAddFilters(int kq, int fd, struct epoll_event *event) {
    if (event == NULL) {
        errno = EINVAL;
        return -1;
    }

    uint16_t flags = kqueueFlagsFromEpoll(event->events);
    void *udata = (void *) (intptr_t) event->data.fd;

    struct kevent ch[2];
    int n = 0;
    // Hangup/error is reported on the read filter (EV_EOF / EV_ERROR).
    if (event->events & (EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
        EV_SET(&ch[n++], fd, EVFILT_READ, flags, 0, 0, udata);
    }
    if (event->events & EPOLLOUT) {
        EV_SET(&ch[n++], fd, EVFILT_WRITE, flags, 0, 0, udata);
    }
    if (n == 0) {
        EV_SET(&ch[n++], fd, EVFILT_READ, flags, 0, 0, udata);
    }

    if (kevent(kq, ch, n, NULL, 0, NULL) < 0) {
        return -1;
    }
    return 0;
}

int epoll_create(int size) {
    (void) size;
    int kq = kqueue();
    if (kq < 0) {
        return -1;
    }
    int flags = fcntl(kq, F_GETFD, 0);
    if (flags >= 0) {
        (void) fcntl(kq, F_SETFD, flags | FD_CLOEXEC);
    }
    return kq;
}

int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event) {
    switch (op) {
        case EPOLL_CTL_ADD:
            return kqueueAddFilters(epfd, fd, event);
        case EPOLL_CTL_MOD:
            // Replacement semantics: drop previous filters, then add the new set.
            kqueueDeleteFilters(epfd, fd);
            return kqueueAddFilters(epfd, fd, event);
        case EPOLL_CTL_DEL:
            return kqueueDeleteFilters(epfd, fd);
        default:
            errno = EINVAL;
            return -1;
    }
}

int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout) {
    if (events == NULL || maxevents <= 0) {
        errno = EINVAL;
        return -1;
    }

    const int cap = maxevents > 1024 ? 1024 : maxevents;
    struct kevent kevs[1024];
    struct timespec ts;
    struct timespec *pts = NULL;
    if (timeout >= 0) {
        ts.tv_sec = timeout / 1000;
        ts.tv_nsec = (timeout % 1000) * 1000000L;
        pts = &ts;
    }

    int n = kevent(epfd, NULL, 0, kevs, cap, pts);
    if (n < 0) {
        return -1;
    }

    for (int i = 0; i < n; ++i) {
        memset(&events[i], 0, sizeof(events[i]));
        events[i].data.fd = (int) kevs[i].ident;

        if (kevs[i].flags & EV_ERROR) {
            events[i].events |= EPOLLERR;
        }
        if (kevs[i].flags & EV_EOF) {
            events[i].events |= EPOLLRDHUP | EPOLLHUP;
            if (kevs[i].fflags != 0) {
                events[i].events |= EPOLLERR;
            }
        }

        if (kevs[i].filter == EVFILT_READ) {
            // If the peer closed but bytes remain, let the reader drain first.
            if (kevs[i].data > 0 || !(kevs[i].flags & EV_EOF)) {
                events[i].events |= EPOLLIN;
                if (kevs[i].data > 0 && (kevs[i].flags & EV_EOF)) {
                    events[i].events &= ~(EPOLLRDHUP | EPOLLHUP);
                }
            }
        } else if (kevs[i].filter == EVFILT_WRITE) {
            if (!(kevs[i].flags & EV_EOF) && !(kevs[i].flags & EV_ERROR)) {
                events[i].events |= EPOLLOUT;
            } else if (!(kevs[i].flags & EV_EOF)) {
                events[i].events |= EPOLLOUT;
            }
        }

        if (events[i].events == 0) {
            events[i].events = EPOLLERR;
        }
    }
    return n;
}

#endif // __APPLE__
