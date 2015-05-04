byte-store
===============

[![Build Status](https://travis-ci.org/alexpreynolds/byte-store.svg)](https://travis-ci.org/alexpreynolds/byte-store)

Generation and querying of upper triangular and square matrices storing 1-byte encoded correlation score values between -1.0 and +1.0, in 0.01 increments.

Score values are either simulated with the [MT19937 RNG](http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html) or sourced from a BED4 file ([example](https://raw.githubusercontent.com/alexpreynolds/byte-store/master/test/vec_test1000.bed)), which contains a comma-delimited string of number values from which [Pearson's r](http://en.wikipedia.org/wiki/Pearson_product-moment_correlation_coefficient) correlation scores are calculated and encoded.
