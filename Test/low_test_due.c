//
// Created by Salvatore Rivieccio on 09/01/17.
//
//
// Created by Salvatore Rivieccio on 01/10/16.
//
#include "../LowFreqSpinlock/low_freq_spinlock.h"
#include <pthread.h>
#include "test.h"

low_freq_spinlock_t try;
int c = 0;

void *threadFunc(void *arg)
{
    low_freq_op_lock(&try);

    printf("thread %d running in critical section\n", c);
    c++;
    usleep((useconds_t) (int) arg);

    low_freq_op_unlock(&try);

    return NULL;
}


int main(int argc, char *argv[])
{
    int i, cycles, loop, c, n_threads, duration;


    try = LOW_FREQ_UNLOCKED;
    cycles = atoi(argv[1]);
    n_threads = atoi(argv[2]);
    duration = atoi(argv[3]);

    printf("Benchmark started, creating threads...\n");

    clock_t begin = clock();

    pthread_t pthread[128];	// this is our thread identifier

    for(i = 0; i < n_threads; i++)
    {
        pthread_create(&pthread[i], NULL, threadFunc, (void*) sleep);
    }
    sleep(duration);
    pthread_join(pthread[c-1], NULL);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("finished in %f sec\n", time_spent);

    return 0;
}
