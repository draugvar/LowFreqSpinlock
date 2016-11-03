//
// Created by Salvatore Rivieccio on 25/10/16.
//

#include "low_freq_module.h"

void set_bitmap(int i) {
    tid_table[i / 8] |= 1 << (i & 7);
}

void unset_bitmap(int i) {
    tid_table[i / 8] &= ~(1 << (i & 7));
}

int get_bitmap(int i) {
    return tid_table[i / 8] & (1 << (i & 7)) ? 1 : 0;
}

bitmap_t create_bitmap(int n) {
    return kmalloc((size_t) ((n + 7) / 8), GFP_KERNEL);
}

static int lfm_open(struct inode *inode, struct file *filp)
{
    try_module_get(THIS_MODULE);
    return 0; /* success */
}

static int lfm_release(struct inode *inode, struct file *filp)
{
    module_put(THIS_MODULE);
    return 0; /* success */
}

static long lfm_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int res, tid;

    switch (cmd)
    {
        case LFM_SET_TID:
            res = copy_from_user(&tid, (int *) arg, sizeof(int));
            set_bitmap(tid);
            printk(KERN_INFO "Setting for: %d", tid);
            break;
        case LFM_UNSET_TID:
            res = copy_from_user(&tid, (int *) arg, sizeof(int));
            unset_bitmap(tid);
            printk(KERN_INFO "Unsetting for: %d", tid);
            break;
        case LFM_IS_PRESENT:
            res = copy_from_user(&tid, (int *) arg, sizeof(int));
            get_bitmap(tid);
            printk(KERN_INFO "Bit value for: %d", tid);
            break;
        default:
            return -EINVAL;
    }
    return 0;
}

int init_module(void)
{
    printk(KERN_INFO "Inserting low_freq_module.\n");
    // scan of /proc/sys/kernel/threads-max in order to get max threads number
    /*int fd_threads_max;
    int number;
    fd_threads_max = open("/proc/sys/kernel/threads-max", O_RDONLY);
    if (fd_threads_max == -1)
    {
        printk(KERN_INFO "Error opening file!\n");
        return -1;
    }

    char min[2];
    read(fd_threads_max, &min, sizeof(min));

    close(fd_threads_max);*/
    tid_table = create_bitmap(160000); // da cambiare

    Major = register_chrdev(0, DEVICE_NAME, &fops);
    if (Major < 0)
    {
        printk(KERN_INFO "Registering device failed\n");
        return Major;
    }
    printk(KERN_INFO "Low_freq device registered, it is assigned major number %d\n", Major);

    /*
     * A non 0 return means init_module failed; module can't be loaded.
     */
    return 0;
}

void cleanup_module(void)
{
    unregister_chrdev(Major, DEVICE_NAME);
    printk(KERN_INFO "low freq module device unregistered, it was assigned major number %d\n", Major);
    kfree(tid_table);
}