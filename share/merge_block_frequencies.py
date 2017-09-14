#!/usr/bin/env python

import sys
import os

block_dir = sys.argv[1]

suffix = "rbs.frequency.txt"

bins = []
totals = {}
total_count = 0
for i in range(0, 1000000):
    prefix = "%013d" % i
    freq_fn = os.path.join(block_dir, "%s.%s" % (prefix, suffix))
    if not os.path.exists(freq_fn):
        break
    sys.stderr.write("%s\n" % (freq_fn))
    with open(freq_fn, "r") as ifh:
        for line in ifh:
            (bin, count, freq) = line.strip().split('\t')
            if not bin in totals:
                totals[bin] = 0
                bins.append(bin)
            totals[bin] += int(count)
            total_count += int(count)


for bin in bins:
    sys.stdout.write("%s\t%d\t%8.6f\n" % (bin, totals[bin], float(totals[bin])/total_count))
