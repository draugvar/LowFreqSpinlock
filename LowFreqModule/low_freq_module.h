//
// Created by Salvatore Rivieccio on 25/10/16.
//

#ifndef CLIONPROJECTS_LOW_FREQ_MODULE_H
#define CLIONPROJECTS_LOW_FREQ_MODULE_H

#endif //CLIONPROJECTS_LOW_FREQ_MODULE_H

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
#include <asm/uaccess.h> /* copy_from/to_user */

#include "ioctl.h"

static int lfm_open(struct inode *, struct file *);
static int lfm_release(struct inode *, struct file *);
static long lfm_ioctl(struct file *f, unsigned int cmd, unsigned long arg);

#define DEVICE_NAME "low_freq_module"  /* Device file name in /dev/ - not mandatory  */
#define DEVICE_PATH "/dev/"DEVICE_NAME
#define DEVICE_MAX_NUMBER 1
#define EXPORT_SYMTAB

/* Major number assigned to broadcast device driver */
static int Major;


typedef unsigned char* bitmap_t;

bitmap_t tid_table;

static struct file_operations fops = {
        .open = lfm_open,
        .release = lfm_release,
        .unlocked_ioctl = lfm_ioctl
};
