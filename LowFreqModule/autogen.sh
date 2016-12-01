#!/usr/bin/env bash

if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root" 1>&2
    exit 1
fi
    module="low_freq_module"
    device="low_freq_module"
    mode="666"
    dev_number="0"

    # make clean and make
    make clean
    make

    # clean old module and invoke insmod with all arguments we were passed
    # and use a pathname, as newer modutils don't look in . by default
    nr_threads=`cat /proc/sys/kernel/threads-max`
    /sbin/rmmod ${module} ; /sbin/insmod -f ./${module}.ko $* nr_of_threads=${nr_threads}|| exit 1
    address=`dmesg | awk -f address_script.awk | tail -n 1`

    echo "${address}">/sys/module/schedule/parameters/the_hook

    # remove stale nodes
    rm -f /dev/${device}


    major=`awk "\\$2==\"$module\" {print \\$1}" /proc/devices`

    # create device and give appropriate group/permissions, and change the group.
    # Not all distributions have staff; some have "wheel" instead.
    group="staff"
    grep '^staff:' /etc/group > /dev/null || group="wheel"

    mknod /dev/${device} c ${major} 0
    chgrp ${group} /dev/${device}
    chmod ${mode} /dev/${device}

    make clean