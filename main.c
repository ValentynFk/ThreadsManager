#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#define THREAD_CTX_STATE_BEGIN   1
#define THREAD_CTX_STATE_PENDING 2
#define THREAD_CTX_STATE_READY   3
#define THREAD_CTX_STATE_FINISH  4

// Context of a single thread
struct thread_context_t {
    void * args;
    void * routine;
    int state;
};
// Routine of a single thread
void * thread_routine(void * thread_context) {
    struct thread_context_t * ctx = (struct thread_context_t *)thread_context;
    printf("Starting thread\n");
    // Wait for the first task to come in
    while (ctx -> state == THREAD_CTX_STATE_BEGIN);
    printf("Thread state is not begin now\n");
    if (ctx -> state != THREAD_CTX_STATE_FINISH) {
        printf("Thread state is not finish now\n");
        // Infinite loop for processing tasks
        for (;;) {
            while (ctx -> state != THREAD_CTX_STATE_PENDING &&
                   ctx -> state != THREAD_CTX_STATE_FINISH);
            if (ctx -> state == THREAD_CTX_STATE_FINISH) {
                void * retval;
                return retval;
            }
            printf("Routine has come to thread\n");
            // Execute given routine as soon as it is pending
            (*((void (*)(void *))ctx -> routine))(ctx -> args);
            // If threads manager forces quit, exit routine
            if (ctx -> state == THREAD_CTX_STATE_FINISH) {
                void * retval;
                return retval;
            } else {
                ctx -> state = THREAD_CTX_STATE_READY;
            }
        }
    }
    void * retval;
    return retval;
}

// Queue of threads
struct threads_queue_t {
    int threads_num;
    int empty_threads_num;
    pthread_t * threads;
    struct thread_context_t ** threads_ctx;
};
// Initialize queue
void init_thread_queue(struct threads_queue_t ** threads_queue, int threads_num) {
    if (threads_num < 0) {
        return;
    }
    printf("Queue initialization\n");
    *threads_queue = malloc(sizeof(struct threads_queue_t));
    (*threads_queue) -> threads_num = threads_num;
    // Allocate memory for threads and their contexts
    (*threads_queue) -> threads_ctx = malloc(sizeof(struct thread_context_t*) * threads_num);
    (*threads_queue) -> threads =     malloc(sizeof(pthread_t) * threads_num);
    for (int thread_id = 0; thread_id < threads_num; ++thread_id) {
        (*threads_queue) -> threads_ctx[thread_id] = malloc(sizeof(struct thread_context_t));
        (*threads_queue) -> threads_ctx[thread_id] -> state = THREAD_CTX_STATE_BEGIN;
        (*threads_queue) -> threads_ctx[thread_id] -> args    = NULL;
        (*threads_queue) -> threads_ctx[thread_id] -> routine = NULL;
        pthread_create(&(*threads_queue) -> threads[thread_id], NULL, thread_routine, (*threads_queue) -> threads_ctx[thread_id]);
    }
    // TODO checkout what else have to be done
}
// Add pending task
void put_to_thread_queue(struct threads_queue_t * threads_queue, void * routine, void * args) {
    printf("Routine has come to the queue\n");
    int threads_num = threads_queue -> threads_num;
    for (int thread_id = 0; thread_id < threads_num; ++thread_id) {
        if (threads_queue -> threads_ctx[thread_id] -> state == THREAD_CTX_STATE_READY ||
            threads_queue -> threads_ctx[thread_id] -> state == THREAD_CTX_STATE_BEGIN) {
            // If the specified thread has done previous task
            threads_queue -> threads_ctx[thread_id] -> routine = routine;
            threads_queue -> threads_ctx[thread_id] -> args = args;
            threads_queue -> threads_ctx[thread_id] -> state = THREAD_CTX_STATE_PENDING;
            return;
        }
    }
}
// Destroy queue
void delete_thread_queue(struct threads_queue_t ** threads_queue) {
    printf("Queue destruction\n");
    int threads_num = (*threads_queue) -> threads_num;
    // Force threads to exit
    for (int thread_id = 0; thread_id < threads_num; ++thread_id) {
        (*threads_queue) -> threads_ctx[thread_id] -> state = THREAD_CTX_STATE_FINISH;
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
void * do_some_job(void * i) {
    printf("do some job over i which is: %d\n", *((int *)i));
}

int main(int argc, char ** argv) {
    struct threads_queue_t *threads_queue;
    init_thread_queue(&threads_queue, 10);
    
    // Paste your code here
    int * i = malloc(sizeof(int));
    *i = 10;
    put_to_thread_queue(threads_queue, &do_some_job, i);
    for (long j = 0; j < 500000000; ++j) {
        //do something
    }
    // End pasting code
    
    delete_thread_queue(&threads_queue);
    return 0;
}
