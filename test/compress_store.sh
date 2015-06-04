#!/bin/bash -x

tests=($(seq 1 3))
sizes=(562 1000 1779 3162 5623 10000);
compr=("bzip2" "gzip")
strategies=("full" "mid-quarter-zero" "custom")

echo "-> Compressing store... ($1)"

for size in "${sizes[@]}"
do
    for test_idx in "${tests[@]}"
    do
        for compr_type in "${compr[@]}"
        do
            for strategy in "${strategies[@]}"
            do
                echo "compress store ${size}-${test_idx}-${strategy} $1 $compr_type"
                $compr_type --stdout $2/sample.${size}.${test_idx}.${strategy}.$1.bs > $2/sample.${size}.${test_idx}.${strategy}.$1.bs.$compr_type
            done
        done
    done
done
