#! /bin/bash

for nth in {1,2,3,4,6,8,12,16,24,32,48}
do
    cnt=2
    while [ $cnt -le 7 ]
    do
        echo ${nth}
        node=$((10**cnt))
        java SSSP -a 0 -n ${node} -t ${nth}
        ((cnt++))
    done
done
