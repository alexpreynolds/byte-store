#!/bin/bash -x

tests=($(seq 1 3))
sizes=(562 1000 1779 3162 5623 10000);
strategies=("full" "mid-quarter-zero" "custom")

echo "-> Time store query compressed... ($1)"

for size in "${sizes[@]}"
do
    let ub=($size - 1)
    for test_idx in "${tests[@]}"
    do
        for strategy in "${strategies[@]}"
        do
            echo "query all store ${size}-${test_idx}-${strategy} $1"
            bs_cmd="$3 -t $1 -q -l $2/sample.${size}.${test_idx}.bed -s $2/sample.${size}.${test_idx}.${strategy}.$1.bs -i 0-$ub"
            { time -p $bs_cmd > /dev/null; } 2> $2/sample.${size}.${test_idx}.${strategy}.$1.bs.query_all_time
        done
    done
done
