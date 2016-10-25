//
// Created by Salvatore Rivieccio on 04/10/16.
//
#include "low_freq_spinlock.h"
#include "util.h"

void set_low_freq();
void reset_low_freq();

char min_before_set[16];
char max_before_set[16];

int fd_scaling_min = 0;
int fd_scaling_max = 0;

void low_freq_op_lock(low_freq_spinlock_t *lock)
{
    while (!__sync_lock_test_and_set(&(lock)->exclusion, 1)) //invertito
    {
        set_low_freq();
        while ((lock)->exclusion)
        {
           break;//da togliere
        }
        reset_low_freq();
        break;//da togliere
    }
}

void low_freq_op_unlock(low_freq_spinlock_t *lock)
{
    __sync_synchronize(); // Memory barrier.
    (lock)->exclusion = 0;
}

void set_low_freq()
{
    int cpu_id = sched_getcpu();
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_id, &cpuset);
    sched_setaffinity(0, sizeof(cpuset), &cpuset); // 0 is the current thread
    char min_freq[64] = "";
    sprintf(min_freq, CPUINFO_MIN, cpu_id); //può essere ottimizzato

    int fd_min = open(min_freq, O_RDONLY);
    if (fd_min == -1)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    char min[16];
    read(fd_min, &min, sizeof(min));

    close(fd_min);

    char scaling_min[64] = "";
    sprintf(scaling_min, SCALING_MIN, cpu_id);
    fd_scaling_min = open(scaling_min, O_RDWR);

    if (fd_scaling_min == -1)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    read(fd_scaling_min, &min_before_set, sizeof(min_before_set));
    lseek(fd_scaling_min, 0, SEEK_SET);

    if (write(fd_scaling_min, &min, sizeof(min)) == -1 || write(fd_scaling_min, "\0", 1) == -1)
    {
        printf("Error writing file!\n");
        exit(1);
    }
    lseek(fd_scaling_min, 0, SEEK_SET);
    //close(fd_scaling_min);

    char scaling_max[64] = "";
    sprintf(scaling_max, SCALING_MAX, cpu_id);
    fd_scaling_max = open(scaling_max, O_RDWR);

    if (fd_scaling_max == -1)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    read(fd_scaling_max, &max_before_set, sizeof(max_before_set));
    lseek(fd_scaling_max, 0, SEEK_SET);

    if (write(fd_scaling_max, &min, sizeof(min)) == -1 || write(fd_scaling_max, "\0", 1) == -1)
    {
        printf("Error writing file!\n");
        exit(1);
    }
    lseek(fd_scaling_max, 0, SEEK_SET);

    //close(fd_scaling_max);

}

void reset_low_freq()
{
    if (write(fd_scaling_min, &min_before_set, sizeof(min_before_set)) == -1)
    {
        printf("Error writing file!\n");
        exit(1);
    }
    close(fd_scaling_min);

    if (write(fd_scaling_max, &max_before_set, sizeof(max_before_set)) == -1)
    {
        printf("Error writing file!\n");
        exit(1);
    }

    close(fd_scaling_max);
}