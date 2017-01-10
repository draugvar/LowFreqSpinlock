//
// Created by Salvatore Rivieccio on 18/12/16.
//

#ifndef CLIONPROJECTS_NORMAL_SPINLOCK_H
#define CLIONPROJECTS_NORMAL_SPINLOCK_H

#endif //CLIONPROJECTS_NORMAL_SPINLOCK_H

typedef struct __normal_spinlock_t
{
    volatile int exclusion;
}normal_spinlock_t ;

#define NORMAL_UNLOCKED (normal_spinlock_t) { 0 }

#define normal_init(x)	do { (x)->lock = 0; } while (0)

void normal_lock(normal_spinlock_t *);

void normal_unlock(normal_spinlock_t *);
