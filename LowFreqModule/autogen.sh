#!/usr/bin/env bash

module="low_freq_module"
device="low_freq_module"
mode="666"
dev_number="0"

# make clean and make
make clean
make

# clean old module and invoke insmod with all arguments we were passed
# and use a pathname, as newer modutils don't look in . by default
sudo /sbin/rmmod $module ; sudo /sbin/insmod -f ./$module.ko $* || exit 1

# remove stale nodes
sudo rm -f /dev/${device}


major=`awk "\\$2==\"$module\" {print \\$1}" /proc/devices`

# create device and give appropriate group/permissions, and change the group.
# Not all distributions have staff; some have "wheel" instead.
group="staff"
grep '^staff:' /etc/group > /dev/null || group="wheel"

sudo mknod /dev/${device} c $major 0
sudo chgrp $group /dev/${device}
sudo chmod $mode /dev/${device}

make clean