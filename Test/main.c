//
// Created by Salvatore Rivieccio on 01/10/16.
//
#include "../LowFreqSpinlock/low_freq_spinlock.h"

int main()
{
    low_freq_spinlock_t try = LOWFREQ_UNLOCKED;
    low_freq_op_lock(&try);
    //sezione critica
    low_freq_op_unlock(&try);
    return 0;
}