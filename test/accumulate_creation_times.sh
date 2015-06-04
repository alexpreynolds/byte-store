#!/bin/bash -x

tests=($(seq 1 3))
sizes=(562 1000 1779 3162 5623 10000);
strategies=("full" "mid-quarter-zero" "custom")

echo "-> Accumulating creation times... ($1)"

for size in "${sizes[@]}"
do
    rm -f $2/sample.${size}.$1.create_times
    for test_idx in "${tests[@]}"
    do
        for strategy in "${strategies[@]}"
        do
            echo "accumulate creation times ${size}-${test_idx}-${strategy} $1"
            FILESIZE=$(wc -c "$2/sample.${size}.${test_idx}.${strategy}.$1.bs" | awk '{print $1}')
            awk -vType=$1 -vSize=${size} -vTrial=${test_idx} -vStrategy=${strategy} -vFilesize=${FILESIZE} '{split($0, kv, " ");} END { print Type"\t"Size"\t"Trial"\t"Strategy"\t"Filesize"\t"kv[2]; }' $2/sample.${size}.${test_idx}.${strategy}.$1.bs.create_time >> $2/sample.${size}.$1.create_times
        done
    done
done
