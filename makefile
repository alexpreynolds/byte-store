BLDFLAGS = -Wall -Wextra -pedantic -std=c99 -Wno-unused-function
CFLAGS   = -D__STDC_CONSTANT_MACROS -D__STDINT_MACROS -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE=1 -O3

all:
	$(CC) $(BLDFLAGS) $(CFLAGS) -c mt19937.c -o mt19937.o
	$(AR) rcs mt19937.a mt19937.o
	$(CC) $(BLDFLAGS) $(CFLAGS) -c byte-store.c -o byte-store.o
	$(CC) $(BLDFLAGS) $(CFLAGS) byte-store.o -o byte-store mt19937.a

test:
	./byte-store -t sut -c -l test1000.bed -s test1000.bs
	./byte-store -t sut -q -l test1000.bed -s test1000.bs -i 0-999 | awk '$$7>=0.91'

clean:
	rm -rf byte-store
	rm -rf *.a
	rm -rf *.o
	rm -rf *~
	rm -rf *.bs