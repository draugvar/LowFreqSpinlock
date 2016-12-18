//
// Created by Salvatore Rivieccio on 18/12/16.
//

#include "../Utility/normal_spinlock.h"
#include <pthread.h>
#include "test.h"

normal_spinlock_t try;

void *threadFunc(void *arg)
{
    char *str;
    volatile int i = 0;
    volatile int j = 0;
    volatile int k = 0;

    str=(char*)arg;

    normal_op_lock(&try);
    printf("%s running in critical section\n",str);
    while(i < TIME)
    {
        while(j < TIME)
        {
            while(k < TIME)
            {
                k++;
            }
            j++;
        }
        ++i;
    }
    normal_op_unlock(&try);

    return NULL;
}


int main()
{
    try = NORMAL_UNLOCKED;

    pthread_t first;	// this is our thread identifier
    pthread_t second;	// this is our thread identifier
    pthread_t third;	// this is our thread identifier

    printf("main waiting for thread to terminate...\n");
    pthread_create(&first,NULL,threadFunc,"first");
    pthread_create(&second,NULL,threadFunc,"second");
    pthread_create(&third,NULL,threadFunc,"third");

    pthread_join(first,NULL);
    pthread_join(second,NULL);
    pthread_join(third,NULL);
    printf("finished.\n");

    return 0;
}