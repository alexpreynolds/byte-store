BLDFLAGS = -Wall -Wextra -pedantic -std=c99 -Wno-unused-function
CFLAGS   = -D__STDC_CONSTANT_MACROS -D__STDINT_MACROS -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE=1 -O3

all:
	$(CC) $(BLDFLAGS) $(CFLAGS) -c mt19937.c -o mt19937.o
	$(AR) rcs mt19937.a mt19937.o
	$(CC) $(BLDFLAGS) $(CFLAGS) -c byte-store.c -o byte-store.o
	$(CC) $(BLDFLAGS) $(CFLAGS) byte-store.o -o byte-store mt19937.a

test-pearsonr-sut:
	./byte-store -t pearson-r-sut -c -l vec_test1000.bed -s vec_test1000.sut.bs
	./byte-store -t pearson-r-sut -q -l vec_test1000.bed -s vec_test1000.sut.bs -i 0-999 | awk '$$7>=1.00'

test-random-sut:
	./byte-store -t random-sut -c -l test1000.bed -s test1000.sut.bs
	./byte-store -t random-sut -q -l test1000.bed -s test1000.sut.bs -i 0-999 | awk '$$7>=0.99'

test-pearsonr-sqr:
	./byte-store -t pearson-r-sqr -c -l vec_test1000.bed -s vec_test1000.sqr.bs
	./byte-store -t pearson-r-sqr -q -l vec_test1000.bed -s vec_test1000.sqr.bs -i 0-999 | awk '$$7>=1.00'

test-random-sqr:
	./byte-store -t random-sqr -c -l test1000.bed -s test1000.sqr.bs
	./byte-store -t random-sqr -q -l test1000.bed -s test1000.sqr.bs -i 0-999 | awk '$$7>=0.99'

test-random-buffered-sqr:
	./byte-store -t random-buffered-sqr -c -l test1000.bed -s test1000.sqr.bs
	./byte-store -t random-buffered-sqr -q -l test1000.bed -s test1000.sqr.bs -i 0-999 | awk '$$7>=0.99'

clean:
	rm -rf byte-store
	rm -rf *.a
	rm -rf *.o
	rm -rf *~
	rm -rf *.bs