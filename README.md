byte-store
===============

[![Build Status](https://travis-ci.org/alexpreynolds/byte-store.svg)](https://travis-ci.org/alexpreynolds/byte-store)

Generation and querying of upper triangular and square matrices storing 1-byte-encoded (correlation) score values between -1.0 and +1.0. 

Score values are either randomly generated or sourced from a BED4 file, which contains a comma-delimited string of number values from which Pearson's R correlation scores are calculated and encoded.
