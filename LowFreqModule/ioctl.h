//
// Created by Salvatore Rivieccio on 02/11/16.
//

#ifndef CLIONPROJECTS_IOCTL_H
#define CLIONPROJECTS_IOCTL_H

#endif //CLIONPROJECTS_IOCTL_H

#include <linux/ioctl.h>
/* IOCTL */
#define LFM_IS_PRESENT _IOR(250, 0, int)
#define LFM_SET_TID _IO(250, 1)
#define LFM_UNSET_TID _IO(250, 2)

int ioctl_call(int , char *argv[]);