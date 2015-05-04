PWD     := $(shell pwd)
TESTDIR  = $(PWD)/test
BLDFLAGS = -Wall -Wextra -pedantic -std=c99
CFLAGS   = -D__STDC_CONSTANT_MACROS -D__STDINT_MACROS -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE=1 -O3
LIBS     = -lm

all:
	$(CC) -g $(BLDFLAGS) $(CFLAGS) -c mt19937.c -o mt19937.o $(LIBS)
	$(AR) rcs mt19937.a mt19937.o
	$(CC) -g $(BLDFLAGS) $(CFLAGS) -c byte-store.c -o byte-store.o $(LIBS)
	$(CC) -g $(BLDFLAGS) $(CFLAGS) byte-store.o -o byte-store mt19937.a

test-pearsonr-sut-1k:
	./byte-store -t pearson-r-sut -c -l $(TESTDIR)/vec_test1000.bed -s $(TESTDIR)/vec_test1000.sut.bs
	./byte-store -t pearson-r-sut -q -l $(TESTDIR)/vec_test1000.bed -s $(TESTDIR)/vec_test1000.sut.bs -i 1-999 | awk '$$7>=1.00'

test-pearsonr-sut-10k:
	./byte-store -t pearson-r-sut -c -l $(TESTDIR)/vec_test10k.bed -s $(TESTDIR)/vec_test10k.sut.bs
	./byte-store -t pearson-r-sut -q -l $(TESTDIR)/vec_test10k.bed -s $(TESTDIR)/vec_test10k.sut.bs -i 0-9999 | awk '$$7>=1.00'

test-random-sut:
	./byte-store -t random-sut -c -l $(TESTDIR)/test1000.bed -s $(TESTDIR)/test1000.sut.bs
	./byte-store -t random-sut -q -l $(TESTDIR)/test1000.bed -s $(TESTDIR)/test1000.sut.bs -i 0-999 | awk '$$7>=0.99'

test-pearsonr-sqr-1k:
	./byte-store -t pearson-r-sqr -c -l $(TESTDIR)/vec_test1000.bed -s $(TESTDIR)/vec_test1000.sqr.bs
	./byte-store -t pearson-r-sqr -q -l $(TESTDIR)/vec_test1000.bed -s $(TESTDIR)/vec_test1000.sqr.bs -i 0-999 | awk '$$7>=1.00'

test-pearsonr-sqr-10k:
	./byte-store -t pearson-r-sqr -c -l $(TESTDIR)/vec_test10k.bed -s $(TESTDIR)/vec_test10k.sqr.bs
	./byte-store -t pearson-r-sqr -q -l $(TESTDIR)/vec_test10k.bed -s $(TESTDIR)/vec_test10k.sqr.bs -i 0-9999 | awk '$$7>=1.00'

test-random-sqr:
	./byte-store -t random-sqr -c -l $(TESTDIR)/test1000.bed -s $(TESTDIR)/test1000.sqr.bs
	./byte-store -t random-sqr -q -l $(TESTDIR)/test1000.bed -s $(TESTDIR)/test1000.sqr.bs -i 0-999 | awk '$$7>=0.99'

test-random-buffered-sqr:
	./byte-store -t random-buffered-sqr -c -l $(TESTDIR)/test1000.bed -s $(TESTDIR)/test1000.sqr.bs
	./byte-store -t random-buffered-sqr -q -l $(TESTDIR)/test1000.bed -s $(TESTDIR)/test1000.sqr.bs -i 0-999 | awk '$$7>=0.99'

clean:
	rm -rf byte-store
	rm -rf *.a
	rm -rf *.o
	rm -rf *~
	rm -rf $(TESTDIR)/*.bs