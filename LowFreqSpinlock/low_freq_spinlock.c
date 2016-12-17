//
// Created by Salvatore Rivieccio on 04/10/16.
//
#include "low_freq_spinlock.h"

void low_freq_op_lock(low_freq_spinlock_t *lock)
{
    /*int cpu_id = sched_getcpu();

    char min_freq_path[64];
    char max_freq_path[64];

    sprintf(min_freq_path, CPUINFO_MIN, cpu_id);
    sprintf(max_freq_path, CPUINFO_MAX, cpu_id);

    int min_freq_fd = open(min_freq_path, O_RDONLY);
    int max_freq_fd = open(max_freq_path, O_RDONLY);

    if (min_freq_fd == -1 || max_freq_fd == -1)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    read(min_freq_fd, &min_freq, sizeof(min_freq));
    read(max_freq_fd, &max_freq, sizeof(max_freq));*/

    if(!__sync_lock_test_and_set(&(lock)->exclusion, 1)) //invertire
    {
        set_low_freq();
        while(!__sync_lock_test_and_set(&(lock)->exclusion, 1)) //invertire
        {
            while ((lock)->exclusion)
            {
                break;//da togliere
            }
            break; //da togliere
        }
        reset_low_freq();
    }

    //close(min_freq_fd);
    //close(max_freq_fd);
}

void low_freq_op_unlock(low_freq_spinlock_t *lock)
{
    __sync_synchronize(); // Memory barrier.
    (lock)->exclusion = 0;
}

void inline set_low_freq() //da vedere inline
{
    int tid = syscall(SYS_gettid);
    sprintf(s_tid, "%d", tid);
    char *argv[3] = {"ioctl", s_tid, "-s"}; //param: name, tid, mode
    ioctl_call(3, argv);

    /*int cpu_id = sched_getcpu();

    char scaling_min[64];
    sprintf(scaling_min, SCALING_MIN, cpu_id);
    fd_scaling_min = open(scaling_min, O_RDWR);

    if (fd_scaling_min == -1)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    if (write(fd_scaling_min, &min_freq, sizeof(min_freq)) == -1 || write(fd_scaling_min, "\0", 1) == -1)
    {
        printf("Error writing file!\n");
        exit(1);
    }
    lseek(fd_scaling_min, 0, SEEK_SET);

    char scaling_max[64];
    sprintf(scaling_max, SCALING_MAX, cpu_id);
    fd_scaling_max = open(scaling_max, O_RDWR);

    if (fd_scaling_max == -1)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    if (write(fd_scaling_max, &min_freq, sizeof(min_freq)) == -1 || write(fd_scaling_max, "\0", 1) == -1)
    {
        printf("Error writing file!\n");
        exit(1);
    }
    lseek(fd_scaling_max, 0, SEEK_SET);*/
}

void reset_low_freq()
{
    /*if (write(fd_scaling_min, &min_freq, sizeof(min_freq)) == -1)
    {
        printf("Error writing file!\n");
        exit(1);
    }

    if (write(fd_scaling_max, &max_freq, sizeof(max_freq)) == -1)
    {
        printf("Error writing file!\n");
        exit(1);
    }*/
    char *argv[3] = {"ioctl", s_tid, "-u"}; //param: name, tid, mode
    ioctl_call(3, argv);

    //close(fd_scaling_min);
    //close(fd_scaling_max);
}