// Written by Valentyn Faychuk at SoftServe on 31.01.2020
// email:
// 		faitchouk.valentyn@gmail.com

#include "threads-manager.h"

// Routine of a single thread
void * thread_routine(void * thread_context) {
     thread_context_t * ctx = ( thread_context_t *)thread_context;
    printf("Starting thread\n");
    // Wait for the first task to come in
    while (ctx -> state == THREAD_STATE_BEGIN);
    printf("Thread state is not begin now\n");
    if (ctx -> state != THREAD_STATE_FINISH) {
        printf("Thread state is not finish now\n");
        // Infinite loop for processing tasks
        for (;;) {
            while (ctx -> state != THREAD_STATE_PENDING &&
                   ctx -> state != THREAD_STATE_FINISH);
            if (ctx -> state == THREAD_STATE_FINISH) {
                void * retval;
                return retval;
            }
            printf("Routine has come to thread\n");
            // Execute given routine as soon as it is pending
            (*((void (*)(void *))ctx -> routine))(ctx -> args);
            // If threads manager forces quit, exit routine
            if (ctx -> state == THREAD_STATE_FINISH) {
                void * retval;
                return retval;
            } else {
                ctx -> state = THREAD_STATE_READY;
            }
        }
    }
    void * retval;
    return retval;
}

// Initialize queue
void thread_queue_init( threads_queue_t ** threads_queue, int threads_num) {
    if (threads_num < 0) {
        return;
    }
    printf("Queue initialization\n");
    *threads_queue = malloc(sizeof( threads_queue_t));
    (*threads_queue) -> threads_num = threads_num;
    // Allocate memory for threads and their contexts
    (*threads_queue) -> threads_ctx = malloc(sizeof(thread_context_t*) * threads_num);
    (*threads_queue) -> threads =     malloc(sizeof(pthread_t) * threads_num);
    for (int thread_id = 0; thread_id < threads_num; ++thread_id) {
        (*threads_queue) -> threads_ctx[thread_id] = malloc(sizeof( thread_context_t));
        (*threads_queue) -> threads_ctx[thread_id] -> state = THREAD_STATE_BEGIN;
        (*threads_queue) -> threads_ctx[thread_id] -> args    = NULL;
        (*threads_queue) -> threads_ctx[thread_id] -> routine = NULL;
        pthread_create(&(*threads_queue) -> threads[thread_id], NULL, thread_routine, (*threads_queue) -> threads_ctx[thread_id]);
    }
    // TODO checkout what else have to be done
}

// Add pending task
void thread_queue_put( threads_queue_t * threads_queue, void * routine, void * args) {
    printf("Routine has come to the queue\n");
    int threads_num = threads_queue -> threads_num;
    for (int thread_id = 0; thread_id < threads_num; ++thread_id) {
        if (threads_queue -> threads_ctx[thread_id] -> state == THREAD_STATE_READY ||
            threads_queue -> threads_ctx[thread_id] -> state == THREAD_STATE_BEGIN) {
            // If the specified thread has done previous task
            threads_queue -> threads_ctx[thread_id] -> routine = routine;
            threads_queue -> threads_ctx[thread_id] -> args = args;
            threads_queue -> threads_ctx[thread_id] -> state = THREAD_STATE_PENDING;
            return;
        }
    }
}

// Destroy queue
void thread_queue_destroy( threads_queue_t ** threads_queue) {
    printf("Queue destruction\n");
    int threads_num = (*threads_queue) -> threads_num;
    // Force threads to exit
    for (int thread_id = 0; thread_id < threads_num; ++thread_id) {
        (*threads_queue) -> threads_ctx[thread_id] -> state = THREAD_STATE_FINISH;
    }
    // Join threads and free their contexts
    void * retval;
    for (int thread_id = 0; thread_id < threads_num; ++thread_id) {
        pthread_join((*threads_queue) -> threads[thread_id], &retval);
        free((*threads_queue) -> threads_ctx[thread_id]);
    }
    free((*threads_queue) -> threads_ctx);
    free((*threads_queue) -> threads);
}
