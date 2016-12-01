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

int on_schedule(void)
{
    struct task_struct *task = current;
    int pid = task->pid;
    printk(KERN_INFO "stampa %d\n", pid);
    if(get_bitmap(pid))
    {
        // questo processo vuole viaggiare a velocità minima, check and set
    } else
    {
        // questo processo non c'è, vedere se bisogna rialzare la velocità della cpu
    }
    return 0;
}

int init_module(void)
{
    printk(KERN_INFO "Maximum number of threads for this machine: %d\n", nr_of_threads);

    tid_table = create_bitmap(nr_of_threads);

    Major = register_chrdev(0, DEVICE_NAME, &fops);
    if (Major < 0)
    {
        printk(KERN_INFO "Registering device failed\n");
        return Major;
    }
    printk(KERN_INFO "Low_freq device registered, it is assigned major number %d\n", Major);
    printk(KERN_INFO "Address of on_schedule function is 0x%p\n",on_schedule);
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