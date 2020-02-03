// Written by Valentyn Faychuk at SoftServe on 04.12.2019
// email:
// 		faitchouk.valentyn@gmail.com

#include "threads-manager.h"

void * do_some_job(void * i) {
    printf("do some job over i which is: %d\n", *((int *)i));
}

int main(int argc, char ** argv) {
    threads_queue_t *threads_queue;
    thread_queue_init(&threads_queue, 10);
    
    // Paste your code here
    int * i = malloc(sizeof(int));
    *i = 10;
    thread_queue_put(threads_queue, &do_some_job, i);
    for (long j = 0; j < 500000000; ++j) {
        //do something
    }
    // End pasting code
   
    thread_queue_destroy(&threads_queue);
    return 0;
}
