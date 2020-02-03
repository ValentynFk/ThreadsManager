// Written by Valentyn Faychuk at SoftServe on 31.01.2020
// email:
// 		faitchouk.valentyn@gmail.com

#ifndef THREADS_MANAGER_H
#define THREADS_MANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
// Threads manager definitions

#define THREAD_STATE_BEGIN   1
#define THREAD_STATE_PENDING 2
#define THREAD_STATE_READY   3
#define THREAD_STATE_FINISH  4

// Context of a single thread
typedef struct thread_context_t {
    void * args;
    void * routine;
    int state;
} thread_context_t;

// Queue of threads
typedef struct threads_queue_t {
    int threads_num;
    int empty_threads_num;
    pthread_t * threads;
    struct thread_context_t ** threads_ctx;
} threads_queue_t;

// Destroy queue
void thread_queue_destroy(struct threads_queue_t ** threads_queue);
// Add pending task
void thread_queue_put(struct threads_queue_t * threads_queue, void * routine, void * args);
// Initialize queue
void thread_queue_init(struct threads_queue_t ** threads_queue, int threads_num);
// Routine of a single thread
void * thread_routine(void * thread_context);

#endif // THREADS_MANAGER_H
