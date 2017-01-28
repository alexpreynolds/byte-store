#!/bin/bash +x

bs_binary=$1
input_bed=$2
prefix_path=$3
row_step=$4

${bs_binary} -t pearson-r-sqr-split-single-chunk-metadata -c -l ${input_bed} -s ${prefix_path} -r ${row_step}
