#!/bin/bash

tests=($(seq 1 10))
sizes=(562 1000 1779 3162 5623 10000);
compr=('bzip2' 'gzip')

for size in "${sizes[@]}"
do
    rm -rf $2/sample.${size}.*.$1.bs.compression_ratios
    for test_idx in "${tests[@]}"
    do
        for compr_type in "${compr[@]}"
        do
            echo "measure compression ratio ${size}-${test_idx} $1 $compr_type"
            let raw_fs=($(ls -l $2/sample.${size}.${test_idx}.$1.bs | awk '{print $5}'))
            let compr_fs=($(ls -l $2/sample.${size}.${test_idx}.$1.bs.$compr_type | awk '{print $5}'))
            let ratio=($compr_fs*1000000/$raw_fs);
            echo "$ratio/1000000" | bc -l | awk -vType=$1 -vSize=${size} -vTrial=${test_idx} -vSubtype=${compr_type} '{ print Type"\t"Subtype"\t"Size"\t"Trial"\t"$0}' >> $2/sample.${size}.${test_idx}.$1.bs.compression_ratios
        done
    done
done