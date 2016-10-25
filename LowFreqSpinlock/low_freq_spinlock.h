//
// Created by Salvatore Rivieccio on 04/10/16.
//
#define _GNU_SOURCE

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef CLIONPROJECTS_ADAPTIVE_SPINLOCK_H
#define CLIONPROJECTS_ADAPTIVE_SPINLOCK_H

#endif //CLIONPROJECTS_ADAPTIVE_SPINLOCK_H

#define VIRTUAL

#ifdef  VIRTUAL
    #define CPUINFO_MIN "/home/draugvar/cpu/cpu%i/cpufreq/cpuinfo_min_freq"
    #define SCALING_SET "/home/draugvar/cpu/cpu%i/cpufreq/scaling_setspeed"
    #define SCALING_MAX "/home/draugvar/cpu/cpu%i/cpufreq/scaling_max_freq"
    #define SCALING_MIN "/home/draugvar/cpu/cpu%i/cpufreq/scaling_min_freq"
#endif

#ifndef  VIRTUAL
    #define CPUINFO_MIN "/sys/devices/system/cpu/cpu%i/cpufreq/cpuinfo_min_freq"
    #define SCALING_SET "/sys/devices/system/cpu/cpu%i/cpufreq/scaling_setspeed"
    #define SCALING_MAX "/sys/devices/system/cpu/cpu%i/cpufreq/scaling_max_freq"
    #define SCALING_MIN "/sys/devices/system/cpu/cpu%i/cpufreq/scaling_min_freq"
#endif


typedef struct __low_freq_spinlock_t
{
    volatile int exclusion;
}low_freq_spinlock_t ;

#define LOWFREQ_UNLOCKED (low_freq_spinlock_t) { 0 }

#define adaptive_init(x)	do { (x)->lock = 0; } while (0)

void low_freq_op_lock(low_freq_spinlock_t *);

void low_freq_op_unlock(low_freq_spinlock_t *);