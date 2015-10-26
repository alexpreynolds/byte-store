#!/bin/bash -x

PWD=`pwd`

# create single data store from vec_test4.bed
rm -f $PWD/vec_test4.sqr.bs
$PWD/../byte-store -t pearson-r-sqr -c -l $PWD/vec_test4.bed -s $PWD/vec_test4.sqr.bs

# create data store from vec_test4.bed, where each chunk is made up of one line
rm -rf $PWD/vec_test4.sqr.bs1.rbs.blocks
$PWD/../byte-store -t pearson-r-sqr-split -c -l $PWD/vec_test4.bed -s $PWD/vec_test4.sqr.bs1.rbs -r 1

# create data store from qsub'ed tasks, each chunk made on one computational node
rm -rf $PWD/vec_test4.sqr.bs1.rbs.qsub.blocks
qsub -N bs-test-0 -b y -cwd -o "bs_test_0_run.out" -e "bs_test_0_run.err" -notify -V $PWD/../byte-store -t pearson-r-sqr-split-single-chunk -c -l $PWD/vec_test4.bed -s $PWD/vec_test4.sqr.bs1.rbs.qsub -r 1 -k 0
qsub -N bs-test-1 -b y -cwd -o "bs_test_1_run.out" -e "bs_test_1_run.err" -notify -V $PWD/../byte-store -t pearson-r-sqr-split-single-chunk -c -l $PWD/vec_test4.bed -s $PWD/vec_test4.sqr.bs1.rbs.qsub -r 1 -k 1
qsub -N bs-test-2 -b y -cwd -o "bs_test_2_run.out" -e "bs_test_2_run.err" -notify -V $PWD/../byte-store -t pearson-r-sqr-split-single-chunk -c -l $PWD/vec_test4.bed -s $PWD/vec_test4.sqr.bs1.rbs.qsub -r 1 -k 2
qsub -N bs-test-3 -b y -cwd -o "bs_test_3_run.out" -e "bs_test_3_run.err" -notify -V $PWD/../byte-store -t pearson-r-sqr-split-single-chunk -c -l $PWD/vec_test4.bed -s $PWD/vec_test4.sqr.bs1.rbs.qsub -r 1 -k 3
qsub -hold_jid bs-test-0,bs-test-1,bs-test-2,bs-test-3 -N bs-test-md -b y -cwd -o "bs_test_md_run.out" -e "bs_test_md_run.err" -notify -V $PWD/../byte-store -t pearson-r-sqr-split-single-chunk-metadata -c -l $PWD/vec_test4.bed -s $PWD/vec_test4.sqr.bs1.rbs.qsub -r 1
qsub -hold_jid bs-test-md -N bs-test-diff -S /bin/bash -cwd -o "bs_test_diff_run.out" -e "bs_test_diff_run.err" -notify -V $PWD/parallel_creation_test_diff.sh

