//
// Created by Salvatore Rivieccio on 04/10/16.
//
#define _GNU_SOURCE

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "../LowFreqModule/ioctl.h"


#ifndef CLIONPROJECTS_ADAPTIVE_SPINLOCK_H
#define CLIONPROJECTS_ADAPTIVE_SPINLOCK_H

#endif //CLIONPROJECTS_ADAPTIVE_SPINLOCK_H

#ifdef __NR_gettid
#else
    #error "__NR_gettid unavailable on this system"
#endif

typedef struct __low_freq_spinlock_t
{
    volatile int exclusion;
}low_freq_spinlock_t ;

#define LOW_FREQ_UNLOCKED (low_freq_spinlock_t) { 0 }

#define low_freq_init(x)	do { (x)->lock = 0; } while (0)

void low_freq_op_lock(low_freq_spinlock_t *);

void low_freq_op_unlock(low_freq_spinlock_t *);

void set_low_freq();

void reset_low_freq();