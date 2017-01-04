//
// Created by Salvatore Rivieccio on 18/12/16.
//

#include "../Utility/normal_spinlock.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "test.h"

normal_spinlock_t try;
int c = 0;

void *threadFunc(void *arg)
{
    normal_op_lock(&try);

    printf("thread %d running in critical section\n", c);
    c++;
    usleep((useconds_t) (int) arg);

    normal_op_unlock(&try);

    return NULL;
}


int main(int argc, char *argv[])
{
    int i, sleep;

    sleep = atoi(argv[1]);
    try = NORMAL_UNLOCKED;

    printf("main waiting for thread to terminate...\n");

    clock_t begin = clock();

    pthread_t pthread[LOOP];	// this is our thread identifier
    pthread_create(&pthread[0], NULL, threadFunc, (void*) sleep);

    for(i = 1; i < LOOP; i++)
    {
        pthread_create(&pthread[i], NULL, threadFunc, (void*) sleep);

        pthread_join(pthread[i-1], NULL);
    }

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("finished in %f sec\n", time_spent);

    return 0;
}