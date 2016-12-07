//
// Created by Salvatore Rivieccio on 25/10/16.
//

#ifndef CLIONPROJECTS_LOW_FREQ_MODULE_H
#define CLIONPROJECTS_LOW_FREQ_MODULE_H

#endif //CLIONPROJECTS_LOW_FREQ_MODULE_H

#define LINUX
#define DEVICE_NAME "low_freq_module"  /* Device file name in /dev/ - not mandatory  */
#define EXPORT_SYMTAB

#define VIRTUAL

#ifdef VIRTUAL
    #define CPUINFO_MIN "/home/draugvar/cpu/cpu%i/cpufreq/cpuinfo_min_freq"
    #define CPUINFO_MAX "/home/draugvar/cpu/cpu%i/cpufreq/cpuinfo_max_freq"
    #define SCALING_MIN "/home/draugvar/cpu/cpu%i/cpufreq/scaling_min_freq"
    #define SCALING_MAX "/home/draugvar/cpu/cpu%i/cpufreq/scaling_max_freq"
#endif

#ifndef VIRTUAL
    #define CPUINFO_MIN "/sys/devices/system/cpu/cpu%i/cpufreq/cpuinfo_min_freq"
    #define CPUINFO_MAX "/sys/devices/system/cpu/cpu%i/cpufreq/cpuinfo_max_freq"
    #define SCALING_MIN "/sys/devices/system/cpu/cpu%i/cpufreq/scaling_min_freq"
    #define SCALING_MAX "/sys/devices/system/cpu/cpu%i/cpufreq/scaling_max_freq"
#endif


#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <linux/types.h> /* size_t */
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/buffer_head.h>
#include <asm/uaccess.h> /* copy_from/to_user */
#include <asm/segment.h>
#include <asm/smp.h>

#include "ioctl.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Salvatore Rivieccio");

static int lfm_open(struct inode *, struct file *);
static int lfm_release(struct inode *, struct file *);
static long lfm_ioctl(struct file *f, unsigned int cmd, unsigned long arg);
//int on_schedule();

/* Major number assigned to broadcast device driver */
static int Major;
static int nr_of_threads = 10000;

module_param(nr_of_threads, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

unsigned int *id_table;

struct file *scaling_min_fd;
struct file *scaling_max_fd;

unsigned char cpuinfo_min[16];
unsigned char cpuinfo_max[16];
unsigned char scaling_min[16];
unsigned char scaling_max[16];

static struct file_operations fops = {
        .open = lfm_open,
        .release = lfm_release,
        .unlocked_ioctl = lfm_ioctl
};
