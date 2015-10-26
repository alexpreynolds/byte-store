#!/bin/bash -x

PWD=`pwd`

# test difference
xxd $PWD/vec_test4.sqr.bs > $PWD/vec_test4.sqr.bs.hex
cat $PWD/vec_test4.sqr.bs1.rbs.blocks/0* | xxd > $PWD/vec_test4.sqr.bs1.rbs.blocks/vec_test4.sqr.bs.hex
diff -s $PWD/vec_test4.sqr.bs.hex $PWD/vec_test4.sqr.bs1.rbs.blocks/vec_test4.sqr.bs.hex
cat $PWD/vec_test4.sqr.bs1.rbs.qsub.blocks/0* | xxd > $PWD/vec_test4.sqr.bs1.rbs.qsub.blocks/vec_test4.sqr.bs.hex
diff -s $PWD/vec_test4.sqr.bs.hex $PWD/vec_test4.sqr.bs1.rbs.qsub.blocks/vec_test4.sqr.bs.hex
rm -f $PWD/vec_test4.sqr.bs.hex
rm -f $PWD/vec_test4.sqr.bs1.rbs.blocks/vec_test4.sqr.bs.hex
rm -f $PWD/vec_test4.sqr.bs1.rbs.qsub.blocks/vec_test4.sqr.bs.hex
