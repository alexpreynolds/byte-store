#!/bin/bash -x

PWD=`pwd`

# test difference
xxd $PWD/vec_test1000.sqr.bs > $PWD/vec_test1000.sqr.bs.hex
cat $PWD/vec_test1000.sqr.bs250.rbs.blocks/0* | xxd > $PWD/vec_test1000.sqr.bs250.rbs.blocks/vec_test1000.sqr.bs.hex
diff -s $PWD/vec_test1000.sqr.bs.hex $PWD/vec_test1000.sqr.bs250.rbs.blocks/vec_test1000.sqr.bs.hex
cat $PWD/vec_test1000.sqr.bs300.rbs.qsub.blocks/0* | xxd > $PWD/vec_test1000.sqr.bs300.rbs.qsub.blocks/vec_test1000.sqr.bs.hex
diff -s $PWD/vec_test1000.sqr.bs.hex $PWD/vec_test1000.sqr.bs300.rbs.qsub.blocks/vec_test1000.sqr.bs.hex
rm -f $PWD/vec_test1000.sqr.bs.hex
rm -f $PWD/vec_test1000.sqr.bs250.rbs.blocks/vec_test1000.sqr.bs.hex
rm -f $PWD/vec_test1000.sqr.bs300.rbs.qsub.blocks/vec_test1000.sqr.bs.hex
