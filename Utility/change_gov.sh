#!/bin/bash

max_cpu_id=1
cpu=0
while [ $cpu -le $max_cpu_id ]
         do
         cpufreq-set -c $cpu -g userspace
         cpu=$[cpu+1]
done
