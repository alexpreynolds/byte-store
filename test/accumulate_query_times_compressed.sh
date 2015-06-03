#!/bin/bash -x

tests=($(seq 1 3))
sizes=(562 1000 1779 3162 5623 10000);
strategies=("full" "mid-quarter-zero" "custom")

for size in "${sizes[@]}"
do
    rm -f $2/sample.${size}.$1.query_all_times
    for test_idx in "${tests[@]}"
    do
        for strategy in "${strategies[@]}"
        do
            echo "accumulate query times ${size}-${test_idx}-${strategy} $1"
            FILESIZE=$(du -sk "$2/sample.${size}.${test_idx}.${strategy}.$1.bs.blocks" | cut -f1 | awk '{printf "%d", $0*1024;}')
            awk -vType=$1 -vSize=${size} -vTrial=${test_idx} -vStrategy=${strategy} -vFilesize=${FILESIZE} '{split($0, kv, " ");} END { print Type"\t"Size"\t"Trial"\t"Strategy"\t"Filesize"\t"kv[2]; }' $2/sample.${size}.${test_idx}.${strategy}.$1.bs.query_all_time >> $2/sample.${size}.$1.query_all_times
        done
    done
done
