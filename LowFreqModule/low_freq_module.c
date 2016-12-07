//
// Created by Salvatore Rivieccio on 25/10/16.
//

#include "low_freq_module.h"

unsigned inline int* create_id_table(int n) {
    return kmalloc((size_t) n, GFP_KERNEL);
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
            id_table[tid] = 1;
            printk(KERN_INFO "Setting for: %d", tid);
            break;
        case LFM_UNSET_TID:
            res = copy_from_user(&tid, (int *) arg, sizeof(int));
            id_table[tid] = 0;
            printk(KERN_INFO "Unsetting for: %d", tid);
            break;
        case LFM_IS_PRESENT:
            res = copy_from_user(&tid, (int *) arg, sizeof(int));
            printk(KERN_INFO "Value for %d: %d", tid, id_table[tid]);
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
    if(id_table[pid])
    {
        // questo processo vuole viaggiare a velocità minima, check and set
    } else
    {
        // questo processo non c'è, vedere se bisogna rialzare la velocità della cpu
    }
    return 0;
}

struct file* file_open(const char* path, int flags, int rights) {
    struct file* filp;
    mm_segment_t oldfs;
    int err = 0;

    oldfs = get_fs();
    set_fs(get_ds());
    filp = filp_open(path, flags, rights);
    set_fs(oldfs);
    if(IS_ERR(filp)) {
        err = PTR_ERR(filp);
        return NULL;
    }
    return filp;
}

void file_close(struct file* file) {
    filp_close(file, NULL);
}

int inline file_read(struct file* file, unsigned long long offset, unsigned char* data, unsigned int size) {
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_read(file, data, size, &offset);
    data[ret] = '\0';

    set_fs(oldfs);
    return ret;
}

int inline file_write(struct file* file, unsigned long long offset, unsigned char* data, unsigned int size) {
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_write(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}


int init_module(void)
{
    int cpu_id;
    struct file *cpuinfo_min_fd;
    struct file *cpuinfo_max_fd;
    char cpuinfo_min_path[64];
    char cpuinfo_max_path[64];
    char scaling_min_path[64];
    char scaling_max_path[64];

    printk(KERN_INFO "Maximum number of threads for this machine: %d\n", nr_of_threads);

    id_table = create_id_table(nr_of_threads);
    memset(id_table, 0, nr_of_threads);

    Major = register_chrdev(0, DEVICE_NAME, &fops);
    if (Major < 0)
    {
        printk(KERN_INFO "Registering device failed\n");
        return Major;
    }
    printk(KERN_INFO "Low_freq device registered, it is assigned major number %d\n", Major);
    printk(KERN_INFO "Address of on_schedule function is 0x%p\n",on_schedule);

    cpu_id = smp_processor_id();

    sprintf(cpuinfo_min_path, CPUINFO_MIN, cpu_id);
    sprintf(cpuinfo_max_path, CPUINFO_MAX, cpu_id);
    sprintf(scaling_min_path, SCALING_MIN, cpu_id);
    sprintf(scaling_max_path, SCALING_MAX, cpu_id);

    cpuinfo_min_fd = file_open(cpuinfo_min_path, O_RDONLY, 0644);
    cpuinfo_max_fd = file_open(cpuinfo_max_path, O_RDONLY, 0644);
    scaling_min_fd = file_open(scaling_min_path, O_RDWR, 0644);
    scaling_max_fd = file_open(scaling_max_path, O_RDWR, 0644);

    if(cpuinfo_min_fd ==  NULL || cpuinfo_max_fd == NULL || scaling_min_fd == NULL || scaling_max_fd == NULL){
        return -1;
    }

    file_read(cpuinfo_min_fd, 0, cpuinfo_min, sizeof(cpuinfo_min));
    file_read(cpuinfo_max_fd, 0, cpuinfo_max, sizeof(cpuinfo_max));
    file_read(scaling_min_fd, 0, scaling_min, sizeof(scaling_min));
    file_read(scaling_max_fd, 0, scaling_max, sizeof(scaling_max));
    file_close(cpuinfo_min_fd);
    file_close(cpuinfo_max_fd);

    printk(KERN_INFO "CPU min freq: %s", cpuinfo_min);
    printk(KERN_INFO "CPU max freq: %s", cpuinfo_max);
    printk(KERN_INFO "CPU scaling min freq: %s", scaling_min);
    printk(KERN_INFO "CPU scaling max freq: %s", scaling_max);
    /*
     * A non 0 return means init_module failed; module can't be loaded.
     */
    return 0;
}

void cleanup_module(void)
{
    unregister_chrdev(Major, DEVICE_NAME);
    printk(KERN_INFO "low freq module device unregistered, it was assigned major number %d\n", Major);
    kfree(id_table);
}