#!/usr/bin/env python

import getopt, sys, math

def main():

    a_vec = None
    b_vec = None

    try:
        opts, args = getopt.getopt(sys.argv[1:], "a:b:", ["a-vector=", "b-vector="])
    except getopt.GetoptError as err:
        print str(err)
        usage()
        sys.exit(-1)
    for o, argval in opts:
        if o in ("-a", "--a-vector"):
            a_vec = map(lambda x: int(x), argval.split(","))
        elif o in ("-b", "--b-vector"):
            b_vec = map(lambda x: int(x), argval.split(","))
        else:
            assert False, "unhandled option"

    if a_vec is None or b_vec is None:
        usage()
        sys.exit(-1)
    if len(a_vec) != len(b_vec):
        usage()
        assert False, "Vectors A and B have unequal lengths"

    a_mean = mean(a_vec)
    b_mean = mean(b_vec)
    a_sd = sd(a_vec, a_mean)
    b_sd = sd(b_vec, b_mean)

    if a_sd == 0 or b_sd == 0:
        assert False, "Vectors must have non-zero standard deviation"

    s = 0
    for a, b in zip(a_vec, b_vec):
        s += (a - a_mean) * (b - b_mean)
    s /= ((len(a_vec) - 1) * a_sd * b_sd)

    print s

    sys.exit(0)

def mean(v):
    return sum(v)/len(v)
    
def sd(v, m):
    s = 0
    for val in v:
        s += (val - m) * (val - m)
    return math.sqrt(s / (len(v) - 1))

def usage():
    print "./pearson-check.py -a 1,2,3,... -b 5,6,7,..."

if __name__ == "__main__":
    main()
