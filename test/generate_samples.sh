#!/bin/bash -x

tests=($(seq 1 3))
sizes=(562 1000 1779 3162 5623 10000);

for size in "${sizes[@]}"
do
    for test_idx in "${tests[@]}"
    do
        echo "${size}-${test_idx}"
        sample -k ${size} --preserve-order $1 > $2/sample.${size}.${test_idx}.bed
    done
done
