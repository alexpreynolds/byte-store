#!/bin/bash -x

binwidth=0.25
tests=($(seq 1 3))
sizes=(562 1000 1779 3162 5623 10000);
strategies=("full" "mid-quarter-zero" "custom")

for size in "${sizes[@]}"
do
    for test_idx in "${tests[@]}"
    do
        for strategy in "${strategies[@]}"
        do
            echo "graph frequency histograms ${size}-${test_idx}-${strategy} $3"
            $1 -i $2/sample.${size}.${test_idx}.${strategy}.$3.freq.bs.txt -o $2/sample.${size}.${test_idx}.${strategy}.$3.freq.bs.txt -t "Relative score bin frequency" -x "Score bin" -y "Relative frequency" -w ${binwidth}
        done
    done
done
