#!/bin/bash

tests=($(seq 1 3))
sizes=(562 1000 1779 3162 5623 10000)
strategies=("full" "mid-quarter-zero")

for size in "${sizes[@]}"
do
    for test_idx in "${tests[@]}"
    do
        for strategy in "${strategies[@]}"
        do
            echo "measure frequency store ${size}-${test_idx}-${strategy} $1"
            $3 -t $1 -f -l $2/sample.${size}.${test_idx}.bed -s $2/sample.${size}.${test_idx}.${strategy}.$1.bs -e ${strategy} > $2/sample.${size}.${test_idx}.${strategy}.$1.freq.bs.txt
        done
    done
done