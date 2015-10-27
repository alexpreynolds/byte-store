#!/bin/bash -x

# input 1:  reference file (e.g., genes of interest)
# input 2:  row-enumerated index file (see "enumerate_index.sh")
#
# output:   ordered comma-delimited query string containing zero-indexed row
#           indices of index elements that overlap reference elements, which
#           can be passed to byte-store --multiple-index-query=str option

bedmap --echo-map-id-uniq $1 $2 | tr '\n' ',' | awk '{ print substr($0, 0, length($0)-1); }'
