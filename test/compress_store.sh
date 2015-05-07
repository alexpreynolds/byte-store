#!/bin/bash

tests=($(seq 1 3))
sizes=(562 1000 1779 3162 5623 10000);
compr=('bzip2' 'gzip')

for size in "${sizes[@]}"
do
    for test_idx in "${tests[@]}"
    do
        for compr_type in "${compr[@]}"
        do
            echo "compress store ${size}-${test_idx} $1 $compr_type"
            $compr_type --stdout $2/sample.${size}.${test_idx}.$1.bs > $2/sample.${size}.${test_idx}.$1.bs.$compr_type            
        done
    done
done