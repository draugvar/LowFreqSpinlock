#!/usr/bin/env bash
max_cpu_id=1
cpu=0
while [ ${cpu} -le ${max_cpu_id} ]
         do
         cd ~/cpu/cpu${cpu}/cpufreq;
         echo "800000"  > scaling_min_freq;
         echo "1500000" > scaling_max_freq;
         cpu=$[cpu+1]
done