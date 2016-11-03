//
// Created by Salvatore Rivieccio on 02/11/16.
//

#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <fcntl.h>
#include <memory.h>
#include <unistd.h>
#include "ioctl.h"

void tid_is_present(int fd, char *argv)
{
    int res = atoi(argv);

    if (ioctl(fd, LFM_IS_PRESENT, &res) == -1)
    {
        perror("lfm ioctl is present query\n");
    }
    else
    {
        printf("%d\n", res);
    }
}

void set_tid(int fd , char *argv)
{
    int res = atoi(argv);

    if (ioctl(fd, LFM_SET_TID, &res) == -1)
    {
        perror("lfm ioctl set tid\n");
    }
    else
    {
        printf("Process with tid %d set\n", res);
    }
}

void unset_tid(int fd , char *argv)
{
    int res = atoi(argv);

    if (ioctl(fd, LFM_UNSET_TID, &res) == -1)
    {
        perror("lfm ioctl unset tid\n");
    }
    else
    {
        printf("Process with tid %d unset\n", res);
    }
}

int ioctl_call(int argc, char *argv[])
{
    char *file_name = "/dev/low_freq_module";
    int fd;

    enum {
        is_present,
        set,
        unset
    } option;

    if(argc == 3)
    {
        if (strcmp(argv[2], "-s") == 0)
        {
            option = set;
        }
        else if(strcmp(argv[2], "-u") == 0)
        {
            option = unset;
        }
        else if(strcmp(argv[2], "-is") == 0)
        {
            option = is_present;
        }
        else
        {
            fprintf(stderr, "Usage: %s tid [-s | -u | -is ]\n", argv[0]);
            return -1;
        }
    } else
    {
        fprintf(stderr, "Usage: %s tid [-s | -u | -is ]\n", argv[0]);
        return -1;
    }

    fd = open(file_name, O_RDWR);

    if (fd == -1)
    {
        perror("ioctl open\n");
        return -1;
    }

    switch (option)
    {
        case set:
            set_tid(fd, argv[1]);
            break;
        case unset:
            unset_tid(fd, argv[1]);
            break;
        case is_present:
            tid_is_present(fd, argv[1]);
            break;
        default:
            break;
    }

    close(fd);

    return 0;
}