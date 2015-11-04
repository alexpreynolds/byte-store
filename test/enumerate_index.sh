#!/bin/bash

# input:  BED3 or greater
# output: BED4, fourth column is zero-indexed row index ("row enumeration")

awk '{ print $1"\t"$2"\t"$3"\t"(NR-1); }' $1 > "${1%.*}.enum.bed"
