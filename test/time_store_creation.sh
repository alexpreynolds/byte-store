#!/bin/bash -x

tests=($(seq 1 3))
sizes=(562 1000 1779 3162 5623 10000)
strategies=("full" "mid-quarter-zero" "custom")
cutoff_min=-0.5
cutoff_max=+0.5

for size in "${sizes[@]}"
do
    for test_idx in "${tests[@]}"
    do
        for strategy in "${strategies[@]}"
        do
            echo "create store ${size}-${test_idx}-${strategy} $1"
            bs_cmd="$3 -t $1 -c -l $2/sample.${size}.${test_idx}.bed -s $2/sample.${size}.${test_idx}.${strategy}.$1.bs -e ${strategy}"
            if [[ "${strategy}" == "custom" ]]
            then
                bs_cmd="$bs_cmd --encoding-cutoff-zero-min ${cutoff_min} --encoding-cutoff-zero-max ${cutoff_max}"
            fi
            { time -p $bs_cmd; } 2> $2/sample.${size}.${test_idx}.${strategy}.$1.bs.create_time
        done
    done
done
