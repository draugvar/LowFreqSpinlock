#!/bin/bash
gcc low_test_pierangelo.c ../LowFreqSpinlock/low_freq_spinlock.c ../LowFreqModule/ioctl.c -l pthread -o lowfreqp -L../rapl-power -lrapl -lm
gcc low_test_salvatore.c ../LowFreqSpinlock/low_freq_spinlock.c ../LowFreqModule/ioctl.c -l pthread -o lowfreqs -L../rapl-power -lrapl -lm

