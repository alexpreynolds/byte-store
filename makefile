SHELL      := /bin/bash
PWD        := $(shell pwd)
BLDFLAGS    = -Wall -Wextra -pedantic -std=c99
CFLAGS      = -D__STDC_CONSTANT_MACROS -D__STDINT_MACROS -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE=1 -O3
LIBS        = -lm -lbz2
.PHONY      = test
SAMPLE     := $(shell `which sample` --help 2> /dev/null)
TESTDIR     = $(PWD)/test
PDFDIR      = $(TESTDIR)/pdf
#SAMPLEDIR   = /Volumes/Data/byte-store
SAMPLEDIR   = /tmp/byte-store
BZIP2ARC    = $(PWD)/bzip2-1.0.6.tar.gz
BZIP2DIR    = $(PWD)/bzip2-1.0.6
BZIP2SYMDIR = $(PWD)/bzip2
BZIP2LIBDIR = $(BZIP2SYMDIR)
UNAME      := $(shell uname -s)

ifeq ($(UNAME),Darwin)
	CC = clang
	CXX = clang++
	FLAGS += -Weverything
endif

all: byte-store

byte-store: bzip2
	$(CC) -g $(BLDFLAGS) $(CFLAGS) -c mt19937.c -o mt19937.o
	$(AR) rcs mt19937.a mt19937.o
	$(CC) -g $(BLDFLAGS) $(CFLAGS) -c byte-store.c -o byte-store.o
	$(CC) -g $(BLDFLAGS) $(CFLAGS) -I$(BZIP2SYMDIR) -L$(BZIP2LIBDIR) byte-store.o -o byte-store mt19937.a $(LIBS)

bzip2:
	if [ ! -d "$(BZIP2DIR)" ]; then mkdir "$(BZIP2DIR)"; fi
	tar zxvf "$(BZIP2ARC)" -C "$(PWD)"
	ln -sf $(BZIP2DIR) $(BZIP2SYMDIR)
	$(MAKE) -C $(BZIP2SYMDIR) libbz2.a CC=$(CC) 

test-sample:
ifdef SAMPLE
	$(MAKE) test-sample-performance
	$(MAKE) test-sample-graphs
else
	@$(error 'sample' not found -- please clone and install from Github: https://github.com/alexpreynolds/sample)
endif

test/sample_bs_input.starch:
	wget -qO- https://dl.dropboxusercontent.com/u/31495717/byte-store-test.starch > $@

test/sample_bs_input.bed: test/sample_bs_input.starch
	unstarch $^ > $@

test-sample-performance: test/sample_bs_input.bed byte-store 
	mkdir -p $(SAMPLEDIR)
	$(TESTDIR)/generate_samples.sh test/sample_bs_input.bed $(SAMPLEDIR)
	$(TESTDIR)/time_store_creation.sh pearson-r-sut $(SAMPLEDIR) $(PWD)/byte-store
	$(TESTDIR)/time_store_query_all_sut.sh pearson-r-sut $(SAMPLEDIR) $(PWD)/byte-store
	$(TESTDIR)/compress_store.sh pearson-r-sut $(SAMPLEDIR)
	$(TESTDIR)/measure_compression_ratios.sh pearson-r-sut $(SAMPLEDIR)
	$(TESTDIR)/measure_frequency.sh pearson-r-sut $(SAMPLEDIR) $(PWD)/byte-store
	$(TESTDIR)/accumulate_creation_times.sh pearson-r-sut $(SAMPLEDIR)
	$(TESTDIR)/accumulate_query_times.sh pearson-r-sut $(SAMPLEDIR)
	$(TESTDIR)/time_store_creation.sh pearson-r-sqr $(SAMPLEDIR) $(PWD)/byte-store
	$(TESTDIR)/time_store_query_all.sh pearson-r-sqr $(SAMPLEDIR) $(PWD)/byte-store
	$(TESTDIR)/compress_store.sh pearson-r-sqr $(SAMPLEDIR)
	$(TESTDIR)/measure_compression_ratios.sh pearson-r-sqr $(SAMPLEDIR)
	$(TESTDIR)/measure_frequency.sh pearson-r-sqr $(SAMPLEDIR) $(PWD)/byte-store
	$(TESTDIR)/accumulate_creation_times.sh pearson-r-sqr $(SAMPLEDIR)
	$(TESTDIR)/accumulate_query_times.sh pearson-r-sqr $(SAMPLEDIR)
	cat $(SAMPLEDIR)/*.create_times > $(SAMPLEDIR)/create_times.txt
	cat $(SAMPLEDIR)/*.query_all_times > $(SAMPLEDIR)/query_all_times.txt
	cat $(SAMPLEDIR)/*.compression_ratios > $(SAMPLEDIR)/compression_ratios.txt

test-sample-graphs: test-sample-graphs-timing test-sample-graphs-compression test-sample-graphs-frequencies
	mkdir -p $(PDFDIR)
	mv $(SAMPLEDIR)/*.pdf $(PDFDIR)

test-sample-graphs-timing:
	$(TESTDIR)/graph_timing.Rscript -i $(SAMPLEDIR)/create_times.txt -o $(SAMPLEDIR)/create_times -t "Store creation cost" -y "Creation rate (sec/element)"
	$(TESTDIR)/graph_timing.Rscript -i $(SAMPLEDIR)/query_all_times.txt -o $(SAMPLEDIR)/query_all_times -t "Store query cost" -y "Query rate (sec/element)"

test-sample-graphs-compression:
	$(TESTDIR)/graph_compression.Rscript -i $(SAMPLEDIR)/compression_ratios.txt -o $(SAMPLEDIR)/compression_ratios -t "Store compression efficiency" -y "Compression ratio"

test-sample-graphs-frequencies:
	$(TESTDIR)/graph_frequencies.sh $(TESTDIR)/graph_frequencies.Rscript $(SAMPLEDIR) pearson-r-sut
	$(TESTDIR)/graph_frequencies.sh $(TESTDIR)/graph_frequencies.Rscript $(SAMPLEDIR) pearson-r-sqr

test-pearsonr-sut-4:
	$(PWD)/byte-store -t pearson-r-sut -c -l $(TESTDIR)/vec_test4.bed -s $(TESTDIR)/vec_test4.sut.bs
	$(PWD)/byte-store -t pearson-r-sut -q -l $(TESTDIR)/vec_test4.bed -s $(TESTDIR)/vec_test4.sut.bs -i 0-3 | sort-bed -

test-pearsonr-sut-1k:
	$(PWD)/byte-store -t pearson-r-sut -c -l $(TESTDIR)/vec_test1000.bed -s $(TESTDIR)/vec_test1000.sut.bs
	$(PWD)/byte-store -t pearson-r-sut -q -l $(TESTDIR)/vec_test1000.bed -s $(TESTDIR)/vec_test1000.sut.bs -i 1-999 | awk '$$7>=1.00'

test-pearsonr-sut-10k:
	$(PWD)/byte-store -t pearson-r-sut -c -l $(TESTDIR)/vec_test10k.bed -s $(TESTDIR)/vec_test10k.sut.bs
	$(PWD)/byte-store -t pearson-r-sut -q -l $(TESTDIR)/vec_test10k.bed -s $(TESTDIR)/vec_test10k.sut.bs -i 0-9999 | awk '$$7>=1.00'

test-random-sut:
	$(PWD)/byte-store -t random-sut -c -l $(TESTDIR)/test1000.bed -s $(TESTDIR)/test1000.sut.bs
	$(PWD)/byte-store -t random-sut -q -l $(TESTDIR)/test1000.bed -s $(TESTDIR)/test1000.sut.bs -i 0-999 | awk '$$7>=0.99'

test-pearsonr-sqr-4:
	$(PWD)/byte-store -t pearson-r-sqr -c -l $(TESTDIR)/vec_test4.bed -s $(TESTDIR)/vec_test4.sqr.bs
	$(PWD)/byte-store -t pearson-r-sqr -q -l $(TESTDIR)/vec_test4.bed -s $(TESTDIR)/vec_test4.sqr.bs -i 0-3 | sort-bed -

test-pearsonr-sqr-1k:
	$(PWD)/byte-store -t pearson-r-sqr -c -l $(TESTDIR)/vec_test1000.bed -s $(TESTDIR)/vec_test1000.sqr.bs
	$(PWD)/byte-store -t pearson-r-sqr -q -l $(TESTDIR)/vec_test1000.bed -s $(TESTDIR)/vec_test1000.sqr.bs -i 0-999 | awk '$$7>=1.00'

test-pearsonr-sqr-10k:
	$(PWD)/byte-store -t pearson-r-sqr -c -l $(TESTDIR)/vec_test10k.bed -s $(TESTDIR)/vec_test10k.sqr.bs
	$(PWD)/byte-store -t pearson-r-sqr -q -l $(TESTDIR)/vec_test10k.bed -s $(TESTDIR)/vec_test10k.sqr.bs -i 0-9999 | awk '$$7>=1.00'

test-random-sqr:
	$(PWD)/byte-store -t random-sqr -c -l $(TESTDIR)/test1000.bed -s $(TESTDIR)/test1000.sqr.bs
	$(PWD)/byte-store -t random-sqr -q -l $(TESTDIR)/test1000.bed -s $(TESTDIR)/test1000.sqr.bs -i 0-999 | awk '$$7>=0.99'

test-random-buffered-sqr:
	$(PWD)/byte-store -t random-buffered-sqr -c -l $(TESTDIR)/test1000.bed -s $(TESTDIR)/test1000.sqr.bs
	$(PWD)/byte-store -t random-buffered-sqr -q -l $(TESTDIR)/test1000.bed -s $(TESTDIR)/test1000.sqr.bs -i 0-999 | awk '$$7>=0.99'

clean:
	rm -rf byte-store
	rm -rf *.a
	rm -rf *.o
	rm -rf *~
	rm -rf $(TESTDIR)/*~
	rm -rf $(TESTDIR)/*.bs
	rm -rf $(PDFDIR)
	rm -rf test/sample_bs_input.starch
	rm -rf test/sample_bs_input.bed
	rm -rf $(SAMPLEDIR)
	rm -rf $(BZIP2DIR)
	rm -rf $(BZIP2SYMDIR)
