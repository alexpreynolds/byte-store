# byte-store

[![Build Status](https://travis-ci.org/alexpreynolds/byte-store.svg)](https://travis-ci.org/alexpreynolds/byte-store)

Generation and querying of upper triangular and square matrices storing 1-byte encoded correlation score values between -1.0 and +1.0, in 0.01 increments (so-called "byte-store" files).

Score values are either simulated with the [MT19937 RNG](http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html) or sourced from a BED4 file ([example](https://raw.githubusercontent.com/alexpreynolds/byte-store/master/test/vec_test1000.bed)), which contains a comma-delimited string of number values from which [Pearson's r](http://en.wikipedia.org/wiki/Pearson_product-moment_correlation_coefficient) correlation scores are calculated and encoded.

## Installation

```
$ git clone https://github.com/alexpreynolds/byte-store.git
$ make 
```

## Testing

Some basic tests were performed to evaluate "byte-store" creation and query timing, as well as store compression efficiency, using E. Rynes' master DHS signal dataset to sample and encode inputs at various levels, between ~500 and 10k elements, which generates up to 6 GB of intermediate data. Refer to the ``test-sample-performance`` target in the project ``makefile`` for more details.

### Inputs

The reservoir sampling [`sample` utility](https://github.com/alexpreynolds/sample) is used to generate a `sort-bed`-sorted BED4 file, generating uniformly random sampled subsets of the ~680 MB master DHS signal dataset at the following levels: 562, 1000, 1779, 3162, 5623, and 10000 elements. Because of disparity in signal in subsets, we sample 10 trials at each level.

### Creation timing

GNU `time` is used to measure the wallclock time required to create SUT (strictly upper triangular) and square matrix "byte-store" files, which store encodings of Pearson's r correlation scores calculated from the score vectors in the input sample. For visualization, we measure the ratio of elapsed time to number of processed elements, which gives a seconds per element rate for byte-store creation.

### Query timing

Queries of byte-store files are performed at each level, for each trial, to measure the retrieval time for all encoded pairwise correlation scores, writing the two regions and their correlation score to BED7 (BED3 + BED3 + encoded score). For visualization, we measure the ratio of elapsed time to number of processed elements, which gives a seconds per element rate for querying a byte-store file.

### Compression efficiency

To evaluate long-term storage of a SUT or square-matrix byte-store file, we compress it with bzip2 and gzip methods and measure the efficiency ratio: compressed file size divided by raw file size.

## Results

In preliminary tests on a byte store written to `/tmp`:

* Creating a SUT byte store is faster than making a square-matrix store: <a href="url"><img src="https://dl.dropboxusercontent.com/u/31495717/byte-store-test.store_creation_rate.png" align="left" width="640" ></a>
* Querying a SUT byte store gets faster than lookups of a square-matrix store: <a href="url"><img src="https://dl.dropboxusercontent.com/u/31495717/byte-store-test.store_query_rate.png" align="left" width="640" ></a>
* Compressing a SUT or square-matrix byte-store with gzip gives better results than bzip2: <a href="url"><img src="https://dl.dropboxusercontent.com/u/31495717/byte-store-test.compression_efficiency.png" align="left" width="640" ></a>
