//
// Created by Salvatore Rivieccio on 18/12/16.
//

//
// Created by Salvatore Rivieccio on 04/10/16.
//
#include "normal_spinlock.h"

void normal_op_lock(normal_spinlock_t *lock)
{
    if(__sync_lock_test_and_set(&(lock)->exclusion, 1))
    {
        while(__sync_lock_test_and_set(&(lock)->exclusion, 1))
        {
            while ((lock)->exclusion)
            {
                ;
            }
        }
    }
}

void normal_op_unlock(normal_spinlock_t *lock)
{
    __sync_synchronize(); // Memory barrier.
    (lock)->exclusion = 0;
}
