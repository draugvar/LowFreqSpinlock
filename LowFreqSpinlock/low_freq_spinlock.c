//
// Created by Salvatore Rivieccio on 04/10/16.
//
#include "low_freq_spinlock.h"

void low_freq_lock(low_freq_spinlock_t *lock)
{
    if(__sync_lock_test_and_set(&(lock)->exclusion, 1))
    {
        int tid = syscall(__NR_gettid);
        char s_tid[32];
        sprintf(s_tid, "%d", tid);
        set_low_freq(s_tid);
        while(__sync_lock_test_and_set(&(lock)->exclusion, 1))
        {
            while ((lock)->exclusion)
            {
                ;
            }
        }
        reset_low_freq(s_tid);
    }
}

void low_freq_unlock(low_freq_spinlock_t *lock)
{
    __sync_synchronize(); // Memory barrier.
    (lock)->exclusion = 0;
}

void inline set_low_freq(char *s_tid)
{
    char *argv[3] = {"ioctl", s_tid, "-s"}; //param: name, tid, mode
    ioctl_call(3, argv);
}

void inline reset_low_freq(char *s_tid)
{
    char *argv[3] = {"ioctl", s_tid, "-u"}; //param: name, tid, mode
    ioctl_call(3, argv);
}