//
// Created by Salvatore Rivieccio on 04/10/16.
//
#include "low_freq_spinlock.h"

void low_freq_op_lock(low_freq_spinlock_t *lock)
{
    if(__sync_lock_test_and_set(&(lock)->exclusion, 1))
    {
        set_low_freq();
        while(__sync_lock_test_and_set(&(lock)->exclusion, 1))
        {
            while ((lock)->exclusion)
            {
                ;
            }
        }
        reset_low_freq();
    }
}

void low_freq_op_unlock(low_freq_spinlock_t *lock)
{
    __sync_synchronize(); // Memory barrier.
    (lock)->exclusion = 0;
}

void inline set_low_freq()
{
    int tid = syscall(__NR_gettid);
    char s_tid[32];
    sprintf(s_tid, "%d", tid);
    char *argv[3] = {"ioctl", s_tid, "-s"}; //param: name, tid, mode
    ioctl_call(3, argv);
}

void inline reset_low_freq()
{
    int tid = syscall(__NR_gettid);
    char s_tid[32];
    sprintf(s_tid, "%d", tid);
    char *argv[3] = {"ioctl", s_tid, "-u"}; //param: name, tid, mode
    ioctl_call(3, argv);
}