#!/bin/bash 
THREADS=2
CYCLES=1000

while [  $THREADS -lt 33 ];
do
    while [ $CYCLES -lt 1000001 ];
    do
        ./lowfreqp ${THREADS} ${CYCLES} 10 > result_p/p_${THREADS}_${CYCLES}.txt
        let CYCLES=CYCLES*10
    done
    let CYCLES=1000
    let THREADS=THREADS+2
done

THREADS=2
CYCLES=1000

while [  $THREADS -lt 33 ];
do
    while [ $CYCLES -lt 1000001 ];
    do
        ./lowfreqs ${THREADS} ${CYCLES} 10 > result_s/s_${THREADS}_${CYCLES}.txt
        sudo ./reset_freq.sh
        let CYCLES=CYCLES*10
    done
    let CYCLES=1000
    let THREADS=THREADS+2
done
