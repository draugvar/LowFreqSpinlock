//
// Created by Salvatore Rivieccio on 01/10/16.
//
#include "../LowFreqSpinlock/low_freq_spinlock.h"
#include <pthread.h>
#include "test.h"

low_freq_spinlock_t try;

void *threadFunc(void *arg)
{
    int sleep;
    sleep = (int) arg;

    while(sleep < 90000000)
    {
        low_freq_op_lock(&try);
        usleep((useconds_t) (int) arg);
        low_freq_op_unlock(&try);
        sleep += (int) arg;
    }
    return NULL;
}


int main(int argc, char *argv[])
{
    int sleep;

    try = LOW_FREQ_UNLOCKED;
    sleep = atoi(argv[1]);

    printf("main waiting for thread to terminate...\n");

    clock_t begin = clock();

    pthread_t pthread;	// this is our thread identifier
    pthread_create(&pthread, NULL, threadFunc, (void*) sleep);
    pthread_join(pthread, NULL);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("finished in %f sec\n", time_spent);

    return 0;
}