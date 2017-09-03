#!/usr/bin/env python

import sys
import os
import argparse
import errno
import struct
import ctypes
import logging

name = "count_encoded_bytes"
usage = "  $ count_encoded_bytes container-file > counts.txt"
help = """
  The 'count_encoded_bytes' script counts bytes in the specified
  byte-store container file. 
"""

nan = float('nan')
encoded_byte_to_score = [
         -1.00, -0.99, -0.98, -0.97, -0.96, -0.95, -0.94, -0.93, -0.92, -0.91, -0.90, -0.89, -0.88, -0.87, -0.86, -0.85,
         -0.84, -0.83, -0.82, -0.81, -0.80, -0.79, -0.78, -0.77, -0.76, -0.75, -0.74, -0.73, -0.72, -0.71, -0.70, -0.69,
         -0.68, -0.67, -0.66, -0.65, -0.64, -0.63, -0.62, -0.61, -0.60, -0.59, -0.58, -0.57, -0.56, -0.55, -0.54, -0.53,
         -0.52, -0.51, -0.50, -0.49, -0.48, -0.47, -0.46, -0.45, -0.44, -0.43, -0.42, -0.41, -0.40, -0.39, -0.38, -0.37,
         -0.36, -0.35, -0.34, -0.33, -0.32, -0.31, -0.30, -0.29, -0.28, -0.27, -0.26, -0.25, -0.24, -0.23, -0.22, -0.21,
         -0.20, -0.19, -0.18, -0.17, -0.16, -0.15, -0.14, -0.13, -0.12, -0.11, -0.10, -0.09, -0.08, -0.07, -0.06, -0.05,
         -0.04, -0.03, -0.02, -0.01, -0.00, +0.00, +0.01, +0.02, +0.03, +0.04, +0.05, +0.06, +0.07, +0.08, +0.09, +0.10,
         +0.11, +0.12, +0.13, +0.14, +0.15, +0.16, +0.17, +0.18, +0.19, +0.20, +0.21, +0.22, +0.23, +0.24, +0.25, +0.26,
         +0.27, +0.28, +0.29, +0.30, +0.31, +0.32, +0.33, +0.34, +0.35, +0.36, +0.37, +0.38, +0.39, +0.40, +0.41, +0.42,
         +0.43, +0.44, +0.45, +0.46, +0.47, +0.48, +0.49, +0.50, +0.51, +0.52, +0.53, +0.54, +0.55, +0.56, +0.57, +0.58,
         +0.59, +0.60, +0.61, +0.62, +0.63, +0.64, +0.65, +0.66, +0.67, +0.68, +0.69, +0.70, +0.71, +0.72, +0.73, +0.74,
         +0.75, +0.76, +0.77, +0.78, +0.79, +0.80, +0.81, +0.82, +0.83, +0.84, +0.85, +0.86, +0.87, +0.88, +0.89, +0.90,
         +0.91, +0.92, +0.93, +0.94, +0.95, +0.96, +0.97, +0.98, +0.99, +1.00,   nan, +0.00, +0.00, +0.00, +0.00, +0.00,
         +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00,
         +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00,
         +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00];

def main():
    parser = argparse.ArgumentParser(prog=name, usage=usage, add_help=False)
    parser.add_argument('--help', '-h', action='store_true', dest='help')
    parser.add_argument('--debug', '-d', action='store_true', dest='debug')
    parser.add_argument('file', type=argparse.FileType('rb'), nargs='*')
    args = parser.parse_args()

    if args.help or len(args.file) != 1:
        sys.stdout.write(name + '\n\n')
        sys.stdout.write(usage + '\n')
        sys.stdout.write(help)
        if args.help:
            sys.exit(os.EX_OK)
        else:
            sys.stdout.write("\nERROR: Please specify a byte-store container filename\n")
            sys.exit(errno.EINVAL)

    if args.debug:
        logging.basicConfig(level=logging.DEBUG)
        logger = logging.getLogger(__name__)

    archive_handle = args.file[0]
    if not os.path.exists(archive_handle.name) or not os.path.isfile(archive_handle.name):
        sys.stdout.write("ERROR: Input [%s] is not a file or does not exist\n" % (archive_handle.name))
        sys.stdout.write("%s\n" % (usage))
        sys.exit(errno.ENOENT)

    bins = 2**(ctypes.c_ubyte(1).value * 8)
    if args.debug: logger.info("Debug: Making [%d] zero-ed bins...\n" % (bins))
    bin_counter = [0] * bins

    n_bytes = 1024*1024
    total_bytes = 0

    while True:
        if args.debug: logger.info("Debug: Attempting to read [%d] bytes...\n" % (n_bytes))
        bytes = archive_handle.read(n_bytes)
        read_bytes = len(bytes)
        total_bytes += read_bytes
        if args.debug: logger.info("Debug: Read [%d] bytes\n" % (read_bytes))
        if read_bytes == 0:
            break
        # cf. https://docs.python.org/2.7/library/struct.html
        struct_format = "=%dB" % (read_bytes)
        for index in struct.unpack(struct_format, bytes):
            bin_counter[index] += 1

    if args.debug: logger.info("Debug: Read a total of [%d] bytes\n" % (total_bytes))
    
    # write out the counts (inclusive)
    inclusive_range = lambda start, end: range(start, end+1)
    for bin_index in inclusive_range(0, 201):
        sys.stdout.write("%3.2f\t%d\t%3.6f\n" % (
                encoded_byte_to_score[bin_index],
                bin_counter[bin_index],
                float(bin_counter[bin_index]) / total_bytes))

    sys.exit(os.EX_OK)

if __name__ == "__main__":
    main()
