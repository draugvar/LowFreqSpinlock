//
// Created by Salvatore Rivieccio on 25/10/16.
//

#include "low_freq_module.h"

unsigned inline int* create_id_table(int n) {
    return kmalloc((size_t) n * sizeof(int), GFP_KERNEL);
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

struct file* file_open(const char* path, int flags, int rights)
{
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

void file_close(struct file* file)
{
    filp_close(file, NULL);
}

int inline file_read(struct file* file, unsigned long long offset, unsigned char* data, unsigned int size)
{
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_read(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}

int inline file_write(struct file* file, unsigned long long offset, unsigned char* data, unsigned int size)
{
    int ret;
    mm_segment_t oldfs;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_write(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}

static long lfm_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int res, tid, cpu_id;

    switch (cmd)
    {
        case LFM_SET_TID:
            res = copy_from_user(&tid, (int *) arg, sizeof(int));
            id_table[tid] = 1;
            cpu_id = smp_processor_id();

            set_bit(cpu_id, &queue);
            //printk(KERN_INFO "Setting for %d on cpu %d\n", tid, cpu_id);

            //file_write(g_scaling_min_fd[cpu_id], 0, scaling_min, sizeof(scaling_min));
            file_write(g_scaling_max_fd[cpu_id], 0, cpuinfo_min, sizeof(cpuinfo_min));
            g_scaling_max_fd[cpu_id]->f_pos = 0;
            break;
        case LFM_UNSET_TID:
            res = copy_from_user(&tid, (int *) arg, sizeof(int));
            id_table[tid] = 0;
            cpu_id = smp_processor_id();

            clear_bit(cpu_id, &queue);
            //printk(KERN_INFO "Unsetting for %d on cpu %d\n", tid, cpu_id);

            //file_write(g_scaling_min_fd[cpu_id], 0, scaling_min, sizeof(scaling_min));
            file_write(g_scaling_max_fd[cpu_id], 0, cpuinfo_max, sizeof(cpuinfo_max));
            g_scaling_max_fd[cpu_id]->f_pos = 0;
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
    int cpu_id = smp_processor_id();
    if(id_table[pid] == 1 && !test_bit(cpu_id, &queue))
    {
        //file_write(g_scaling_min_fd[cpu_id], 0, scaling_min, sizeof(scaling_min));
        //file_write(g_scaling_max_fd[cpu_id], 0, cpuinfo_min, sizeof(cpuinfo_min));

        //vfs_write(g_scaling_max_fd[cpu_id], cpuinfo_min, sizeof(cpuinfo_min), 0);
        file_write(g_scaling_max_fd[cpu_id], 0, cpuinfo_min, sizeof(cpuinfo_min));
        g_scaling_max_fd[cpu_id]->f_pos = 0;
        set_bit(cpu_id, &queue);
    }
    else if(id_table[pid] == 0 && test_bit(cpu_id, &queue))
    {
        //file_write(g_scaling_min_fd[cpu_id], 0, scaling_min, sizeof(scaling_min));
        //file_write(g_scaling_max_fd[cpu_id], 0, cpuinfo_max, sizeof(cpuinfo_max));

        //vfs_write(g_scaling_max_fd[cpu_id], cpuinfo_max, sizeof(cpuinfo_max), 0);
        file_write(g_scaling_max_fd[cpu_id], 0, cpuinfo_max, sizeof(cpuinfo_max));
        g_scaling_max_fd[cpu_id]->f_pos = 0;
        clear_bit(cpu_id, &queue);
    }
    return 0;
}

int init_module(void)
{
    int i, j;

    struct file *cpuinfo_min_fd[n_proc];
    struct file *cpuinfo_max_fd[n_proc];

    char cpuinfo_min_path[n_proc][64];
    char cpuinfo_max_path[n_proc][64];
    char scaling_min_path[n_proc][64];
    char scaling_max_path[n_proc][64];

    g_scaling_min_fd = kmalloc((size_t) n_proc * sizeof(void *), GFP_KERNEL);
    g_scaling_max_fd = kmalloc((size_t) n_proc * sizeof(void *), GFP_KERNEL);

    queue = 0;

    printk(KERN_INFO "Maximum number of threads for this machine: %d\n", nr_of_threads);
    id_table = create_id_table(nr_of_threads);
    memset(id_table, 0, nr_of_threads * sizeof(int));

    for(i = 0; i < n_proc; i++)
    {
        sprintf(cpuinfo_min_path[i], CPUINFO_MIN, i);
        sprintf(cpuinfo_max_path[i], CPUINFO_MAX, i);
        sprintf(scaling_min_path[i], SCALING_MIN, i);
        sprintf(scaling_max_path[i], SCALING_MAX, i);

        cpuinfo_min_fd[i] = file_open(cpuinfo_min_path[i], O_RDONLY, 0644);
        cpuinfo_max_fd[i] = file_open(cpuinfo_max_path[i], O_RDONLY, 0644);
        g_scaling_min_fd[i] = file_open(scaling_min_path[i], O_RDWR, 0644);
        g_scaling_max_fd[i] = file_open(scaling_max_path[i], O_RDWR, 0644);

        if(cpuinfo_min_fd[i] ==  NULL || cpuinfo_max_fd[i] == NULL || g_scaling_min_fd[i] == NULL || g_scaling_max_fd[i] == NULL)
        {
            printk(KERN_INFO "Registering device failed\n");
            for(j = 0; j < i; j++)
            {
                file_close(cpuinfo_min_fd[j]);
                file_close(cpuinfo_max_fd[j]);
                file_close(g_scaling_min_fd[j]);
                file_close(g_scaling_max_fd[j]);
            }
            kfree(id_table);
            kfree(g_scaling_min_fd);
            kfree(g_scaling_min_fd);
            return -1;
        }

        file_read(cpuinfo_min_fd[i], 0, cpuinfo_min, sizeof(cpuinfo_min));
        file_read(cpuinfo_max_fd[i], 0, cpuinfo_max, sizeof(cpuinfo_max));
        file_read(g_scaling_min_fd[i], 0, scaling_min, sizeof(scaling_min));
        file_read(g_scaling_max_fd[i], 0, scaling_max, sizeof(scaling_max));

        cpuinfo_min_fd[i]->f_pos = 0;
        cpuinfo_max_fd[i]->f_pos = 0;
        g_scaling_min_fd[i]->f_pos = 0;
        g_scaling_max_fd[i]->f_pos = 0;

        file_close(cpuinfo_min_fd[i]);
        file_close(cpuinfo_max_fd[i]);
    }

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
    int i;
    for(i = 0; i < n_proc; i++)
    {
        file_close(g_scaling_min_fd[i]);
        file_close(g_scaling_max_fd[i]);
    }
    kfree(id_table);
    kfree(g_scaling_min_fd);
    kfree(g_scaling_min_fd);
    unregister_chrdev(Major, DEVICE_NAME);
    printk(KERN_INFO "low freq module device unregistered, it was assigned major number %d\n", Major);
}