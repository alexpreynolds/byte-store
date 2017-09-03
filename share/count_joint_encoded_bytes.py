#!/usr/bin/env python

import sys
import os
import argparse
import errno
import struct
import ctypes
import logging

name = "count_split_joint_encoded_bytes"
usage = "  $ count_split_joint_encoded_byte --elementCount=L container-dir-1 container-dir-2 > counts.txt"
help = """
  The 'count_split_joint_encoded_bytes' script counts the joint
  frequency of bytes in two "split" byte-store containers. 

  Use --elementCount to specify the number of columns in a byte-store
  container split chunk. This should be the same value for both
  containers (or they aren't containers of the same size!).
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
    parser.add_argument('--elementCount', '-e', type=int, action="store")
    parser.add_argument('--debug', '-d', action='store_true', dest='debug')
    parser.add_argument('dirs', type=str, nargs='*')
    args = parser.parse_args()

    if args.help or len(args.dirs) != 2:
        sys.stdout.write(name + '\n\n')
        sys.stdout.write(usage + '\n')
        sys.stdout.write(help)
        if args.help:
            sys.exit(os.EX_OK)
        else:
            sys.stdout.write("\nERROR: Please specify two byte-store container directory prefixes\n")
            sys.exit(errno.EINVAL)

    if args.debug:
        logging.basicConfig(level=logging.DEBUG)
        logger = logging.getLogger(__name__)

    try:
        dirs = args.dirs
        element_count = args.elementCount
    except AttributeError as e:
        sys.stderr.write("ERROR: Missing options!\n\n")
        sys.stderr.write(usage + '\n')
        sys.stderr.write(help)
        sys.exit(errno.EINVAL)

    # 255^2 bins for a 2d joint distribution
    bins_per_signal_type = 2**(ctypes.c_ubyte(1).value * 8)
    bins = bins_per_signal_type**2
    if args.debug: logger.info("Debug: Making [%d] zero-ed bins...\n" % (bins))
    bin_counter = [0] * bins

    n_bytes = element_count
    total_bytes = 0

    # open initial handles
    first_block_index = 0
    first_fn = os.path.join(dirs[0], "%013d.rbs" % (first_block_index))
    if not os.path.exists(first_fn):
        sys.stderr.write("ERROR: Missing first block [%s]\n" % (first_fn))
        sys.exit(errno.EINVAL)
    first_handle = open(first_fn, 'rb')
    second_block_index = 0
    second_fn = os.path.join(dirs[1], "%013d.rbs" % (second_block_index))
    if not os.path.exists(second_fn):
        sys.stderr.write("ERROR: Missing second block [%s]\n" % (second_fn))
        sys.exit(errno.EINVAL)
    second_handle = open(second_fn, 'rb')

    # read through blocks
    struct_format = "=%dB" % (n_bytes)
    while True:
        # read n_bytes from first_handle and second_handle
        # if first_bytes_read == 0 or second_bytes_read == 0, close that handle and try to open a new handle
        # otherwise, update bin_counter for joint frequency
        if args.debug: logger.info("Debug: Attempting to read [%d] bytes from first handle...\n" % (n_bytes))
        first_bytes = first_handle.read(n_bytes)
        first_bytes_read = len(first_bytes)
        if args.debug: logger.info("Debug: Read [%d] bytes from first handle\n" % (first_bytes_read))
        if args.debug: logger.info("Debug: Attempting to read [%d] bytes from second handle...\n" % (n_bytes))
        second_bytes = second_handle.read(n_bytes)
        second_bytes_read = len(second_bytes)
        if args.debug: logger.info("Debug: Read [%d] bytes from second handle\n" % (second_bytes_read))
        
        # leave the read loop
        if first_bytes_read == 0 and second_bytes_read == 0:
            first_handle.close()
            second_handle.close()
            break
        # prime the pump (first buffer)
        elif first_bytes_read == 0:
            first_handle.close()
            first_block_index += 1
            first_fn = os.path.join(dirs[0], "%013d.rbs" % (first_block_index))
            try:
                first_handle = open(first_fn, 'rb')
            except IOError:
                sys.stderr.write("ERROR: Could not open first block [%s]\n" % (first_fn))
                sys.exit(errno.ENOENT)
            first_bytes = first_handle.read(n_bytes)
        # prime the pump (second buffer)
        elif second_bytes_read == 0:
            second_handle.close()
            second_block_index += 1
            second_fn = os.path.join(dirs[1], "%013d.rbs" % (second_block_index))
            try:
                second_handle = open(second_fn, 'rb')
            except IOError:
                sys.stderr.write("ERROR: Could not open second block [%s]\n" % (second_fn))
                sys.exit(errno.ENOENT)
            second_bytes = second_handle.read(n_bytes)

        # unpack bytes
        first_bytes_unpacked = struct.unpack(struct_format, first_bytes)
        second_bytes_unpacked = struct.unpack(struct_format, second_bytes)
        
        # increment counts
        for index in range(0, element_count):
            first_byte = first_bytes_unpacked[index]
            second_byte = second_bytes_unpacked[index]
            offset = first_byte * bins_per_signal_type + second_byte
            try:
                bin_counter[offset] += 1
            except IndexError:
                sys.stderr.write("ERROR: offset: [%d] bins_per_signal_type: [%d] bins: [%d]\n" % (offset, bins_per_signal_type, bins))
                sys.exit(errno.EINVAL)

        # increment total byte counter
        total_bytes += n_bytes

    if args.debug: logger.info("Debug: Read a total of [%d] bytes\n" % (total_bytes))

    # write out the counts (inclusive)
    inclusive_range = lambda start, end: range(start, end+1)
    for first_bin_index in inclusive_range(0, 201):
        for second_bin_index in inclusive_range(0, 201):
            offset = first_bin_index * bins_per_signal_type + second_bin_index
            sys.stdout.write("%3.2f\t%3.2f\t%d\t%3.6f\n" % (
                encoded_byte_to_score[first_bin_index],
                encoded_byte_to_score[second_bin_index],
                bin_counter[offset],
                float(bin_counter[offset]) / total_bytes))

    sys.exit(os.EX_OK)

if __name__ == "__main__":
    main()
