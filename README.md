# byte-store

[![Build Status](https://travis-ci.org/alexpreynolds/byte-store.svg)](https://travis-ci.org/alexpreynolds/byte-store)

This utility generates and queries strictly upper triangular (SUT) and square matrices storing one-byte encoded correlation score values between -1.0 and +1.0. A so-called "byte-store" file is a string of bytes in SUT or square matrix form. Three encoding strategies are offered, a "full" strategy that encodes scores in 0.01 increments, a "mid-quarter-zero" strategy that encodes scores within the interval (-0.25, +0.25) as the +0.00 byte-equivalent, and a "custom" strategy, which allows specification of custom minimum and maximum bounds for encoding zero-score bytes.

Compressed square matrix byte-store files store a compressed form of encoded scores, along with metadata to perform indexed retrieval of rows. A compressed byte store can be a monolithic file or split into per-block files, each containing a specified number of rows.

## Filesizes

We initially compare SUT and square matrix byte storage methods because of the significant filesize differences at large scales. SUT byte-store files take up ``n(n-1)/2`` bytes for ``n`` elements, while a square matrix takes up ``n^2`` bytes. Compressed square matrix files generally take considerably fewer bytes, depending on the nature of the score data, chosen encoding strategy and efficiency of the selected compression algorithm.

## Score values

Score values are either simulated with the [MT19937 RNG](http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html) or sourced from a BED4 file ([example](https://raw.githubusercontent.com/alexpreynolds/byte-store/master/test/vec_test1000.bed)), which contains a comma-delimited string of number values from which pairwise [Pearson's r](http://en.wikipedia.org/wiki/Pearson_product-moment_correlation_coefficient) correlation scores are calculated and encoded:

```
chr1    1193700 1193850 20,8,10,31,...
chr1    1459000 1459150 17,10,9,42,...
chr1    2244600 2244750 149,72,71,87,...
chr1    3568000 3568150 70,34,17,70,...
```

Byte-store files are encoded with the "full", "mid-quarter-zero" or "custom" strategy. The default "full" strategy maps all scores from [-1.0, +1.0] within an ``unsigned char`` in 0.01 increments. The "mid-quarter-zero" strategy is similar, but any scores within (-0.25, +0.25) are directly mapped to the equivalent +0.00 ``unsigned char`` value. Tests of the "custom" strategy encode scores within (-0.50, +0.50) to the +0.00 byte-equivalent.

## Getting started

To build `byte-store`, download the source and compile:

```
$ git clone https://github.com/alexpreynolds/byte-store.git
$ make 
```

To date, compilation and score tests have passed under GNU `gcc` 4.6.3 and Clang 3.4 under [Ubuntu 12.04 LTS Server (64-bit)](http://docs.travis-ci.com/user/ci-environment/), GNU `gcc` 4.8.2 under RHEL6, and Clang 3.6 under Mac OS X Yosemite 10.10.4.

## Testing

Some basic tests are included with the ``makefile`` to evaluate byte-store creation and query timing, as well as compression efficiency. BED files are included for basic tests. Refer to the ``test-pearsonr-sut-4`` and subsequent targets for a detailed description of creation and querying of a variety of input sizes and encoding/compression parameters.

A more comprehensive test suite uses [E. Rynes' master DHS signal dataset (~192 MB)](https://dl.dropboxusercontent.com/u/31495717/byte-store-test.starch) to sample and encode inputs at levels between ~500 and 10k elements, which can generate 6 GB or more of intermediate data. Refer to the ``test-sample-performance`` target in the project ``makefile`` for more details. We discuss the results of these tests further below.

### Inputs

The reservoir sampling utility [`sample`](https://github.com/alexpreynolds/sample) is used to generate a `sort-bed`-sorted BED4 file containing some random number of elements from the source input. Here, each file is a uniformly-random sampled subset of the ~680 MB master DHS signal dataset at the following levels: 562, 1000, 1779, 3162, 5623, and 10000 elements. Because of the disparity in signal vectors in the subset files, we sample 3 trials at each level to get a more general sense of how this tool performs overall.

### Creation timing

GNU `time` is used to measure the wallclock time required to create SUT ("strictly upper triangular") and square matrix byte-store files, raw and compressed. As described above, a byte-store file is just a string of bytes representing encoded floating point values between -1.00 and +1.00. For the purpose of this application, specifically, these scores are Pearson's r calculations. Scores are calculated and encoded from the score vectors in the input subsamples.

For visualization, we measure the ratio of elapsed time to number of processed elements, which yields a seconds-per-element rate or average cost for byte-store file creation.

### Query timing

Queries of raw and compressed byte-store files are performed at each level, for each trial, to measure the retrieval time for all encoded, pairwise correlation scores. A query writes the two paired regions and their respective correlation score to BED7 (BED3 + BED3 + encoded score); since the operation is symmetric (and to more easily facilitate downstream BEDOPS set operations), the query result is written twice: once as A-B-score, and again as B-A-score.

For visualization, we measure the ratio of elapsed time to number of processed elements, which gives an average seconds-per-element rate or cost for querying a byte-store file.

### Compression efficiency

To evaluate the potential longer-term archival of a SUT or square-matrix byte-store file, we compress it with open-source `bzip2` and `gzip` compression methods and measure the efficiency ratio as defined by compressed filesize divided by raw filesize.

## Results

Intermediate test files are written to a 3 TB LaCie d2 Quadra USB3 external hard drive (7200 RPM) connected to a MacBook Pro (2.8 GHz Intel Core i5, 8 GB RAM) over a USB 2 connection. Test results can be written to an alternative destination by overriding the `SAMPLEDIR` variable setting in the `makefile`.

### Creation

With fewer bytes to write, creation of an uncompressed, "raw" SUT byte store takes less average time per element than creation of an uncompressed square-matrix store for all encoding strategies.

Compression of a square byte store with ``bzip2`` (either as a monolithic file or a series of "split" per-block files) writes fewer bytes as the encoding strategy goes from full to m.q.z. to "custom". This is because the "custom" strategy pushes more score values to zeroes, which increases redundancy and improves compression efficiency (with the tradeoff of lost information). The average per-element creation time for a compressed archive is lower than for the raw (uncompressed) store types, which suggests that store creation is an I/O-bound process.

<a href="url"><img src="https://dl.dropboxusercontent.com/u/31495717/byte-store-test.store_creation_rate.png" align="left" width="640" ></a>

### Querying

Querying elements from an uncompressed, "raw" SUT byte store quickly gets slower (more "expensive" per element) than lookups to a raw or compressed square-matrix store, as the number of elements increases. Query times are virtually identical for all three encoding strategies, and there is practically little difference between raw and ``bzip2``-compressed square matrix retrieval times.

<a href="url"><img src="https://dl.dropboxusercontent.com/u/31495717/byte-store-test.store_query_rate.png" align="left" width="640" ></a>

### Compression

For the "full" encoding strategy, compressing a raw SUT or square-matrix byte-store with `gzip` (default parameters) gives better results than `bzip2` (default parameters). Byte-stores of typical encoded scores can be compressed down to roughly 76-80% of the original size. However, use of the "mid-quarter-zero" encoding strategy improves the compression efficiency to ~20%, with `bzip2` offering more optimal results. The "custom" strategy more broadly encodes correlation values of (-0.50, +0.50) to 0, leading to a compression efficiency of ~4% when used with `bzip2` routines.

<a href="url"><img src="https://dl.dropboxusercontent.com/u/31495717/byte-store-test.compression_efficiency.png" align="left" width="640" ></a>
