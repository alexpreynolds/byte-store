BLDFLAGS = -Wall -Wextra -pedantic -std=c99
CFLAGS   = -D__STDC_CONSTANT_MACROS -D__STDINT_MACROS -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE=1 -O3

all:
	$(CC) $(BLDFLAGS) $(CFLAGS) -c mt19937.c -o mt19937.o
	$(AR) rcs mt19937.a mt19937.o
	$(CC) $(BLDFLAGS) $(CFLAGS) -c byte-square.c -o byte-square.o
	$(CC) $(BLDFLAGS) $(CFLAGS) byte-square.o -o byte-square mt19937.a

clean:
	rm -rf byte-square
	rm -rf *.a
	rm -rf *.o
	rm -rf *~
