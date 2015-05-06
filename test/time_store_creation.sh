#!/bin/bash

tests=($(seq 1 10))
sizes=(562 1000 1779 3162 5623 10000);

for size in "${sizes[@]}"
do
    for test_idx in "${tests[@]}"
    do
        echo "create store ${size}-${test_idx} $1"
        bs_cmd="$3 -t $1 -c -l $2/sample.${size}.${test_idx}.bed -s $2/sample.${size}.${test_idx}.$1.bs"
        { time -p $bs_cmd; } 2> $2/sample.${size}.${test_idx}.$1.bs.create_time
    done
done