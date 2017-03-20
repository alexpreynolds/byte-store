SHELL           := /bin/bash
PWD             := $(shell pwd)
CC               = gcc
BLDFLAGS         = -Wall -Wextra -std=c99
BLDDFLAGS        = -Wall -Wextra -std=c99 -pedantic
CFLAGS           = -D__USE_POSIX -D__STDC_CONSTANT_MACROS -D__STDINT_MACROS -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE=1 -O3
CDFLAGS          = -D__USE_POSIX -D__STDC_CONSTANT_MACROS -D__STDINT_MACROS -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE=1 -DDEBUG=1 -O
LIBS             = -lrt -lm -lbz2 -lmicrohttpd
.PHONY           = test
SAMPLE          := $(shell `which sample` --help 2> /dev/null)
TESTDIR          = $(PWD)/test
TEST_HTTPD_PORT  = 8000
PDFDIR           = $(TESTDIR)/pdf
#SAMPLEDIR       = /Volumes/Data/byte-store
SAMPLEDIR        = /tmp/byte-store
UNAME           := $(shell uname -s)
INCLUDES         = /usr/include
AWK_VERSION     := $(shell awk --version | grep "GNU Awk")
THIRD_PARTY      = $(PWD)/third-party
BZIP2_ARC        = $(THIRD_PARTY)/bzip2-1.0.6.tar.gz
BZIP2_DIR        = $(THIRD_PARTY)/bzip2-1.0.6
BZIP2_SYM_DIR    = $(THIRD_PARTY)/bzip2
BZIP2_INC_DIR    = $(BZIP2_SYM_DIR)
BZIP2_LIB_DIR    = $(BZIP2_SYM_DIR)
HTTPD_ARC        = $(THIRD_PARTY)/libmicrohttpd-0.9.51.tar.gz
HTTPD_DIR        = $(THIRD_PARTY)/libmicrohttpd-0.9.51
HTTPD_SYM_DIR    = $(THIRD_PARTY)/libmicrohttpd
HTTPD_INC_DIR    = $(HTTPD_SYM_DIR)/include
HTTPD_LIB_DIR    = $(HTTPD_SYM_DIR)/lib

# --------------------------------------
# OS X Clang can't build static binaries
# --------------------------------------

ifeq ($(UNAME),Darwin)
	CC = clang
	CXX = clang++
	FLAGS += -Weverything
endif
ifeq ($(UNAME), Linux)
	CFLAGS += -pthread -static -static-libgcc
	CDFLAGS += -pthread -static -static-libgcc
endif

all: byte-store

# -----------
# Application
# -----------

prep: bzip2 libmicrohttpd

bzip2:
	@if [ ! -d "${BZIP2_DIR}" ]; then \
		mkdir "${BZIP2_DIR}"; \
		tar zxvf "${BZIP2_ARC}" -C "${THIRD_PARTY}"; \
		ln -sf ${BZIP2_DIR} ${BZIP2_SYM_DIR}; \
		${MAKE} -C ${BZIP2_SYM_DIR} libbz2.a CC=${CC}; \
	fi

libmicrohttpd:
	@if [ ! -d "${HTTPD_DIR}" ]; then \
		mkdir "${HTTPD_DIR}"; \
		tar zxvf "${HTTPD_ARC}" -C "${THIRD_PARTY}"; \
		ln -sf ${HTTPD_DIR} ${HTTPD_SYM_DIR}; \
		cd ${HTTPD_SYM_DIR}; \
		./configure --enable-static --prefix=${HTTPD_SYM_DIR}; \
		${MAKE} && ${MAKE} install; \
		cd ${PWD}; \
	fi

byte-store: prep
	$(CC) -g $(BLDFLAGS) $(CFLAGS) -c mt19937.c -o mt19937.o
	$(AR) rcs mt19937.a mt19937.o
	$(CC) -g $(BLDFLAGS) $(CFLAGS) -I${BZIP2_INC_DIR} -I${HTTPD_INC_DIR} -c byte-store.c -o byte-store.o
	$(CC) -g $(BLDFLAGS) $(CFLAGS) -I$(INCLUDES) -I${BZIP2_INC_DIR} -I${HTTPD_INC_DIR} -L"${BZIP2_LIB_DIR}" -L"${HTTPD_LIB_DIR}" byte-store.o -o byte-store mt19937.a $(LIBS)

debug-byte-store: prep
	$(CC) -g $(BLDDFLAGS) $(CDFLAGS) -c mt19937.c -o mt19937.o
	$(AR) rcs mt19937.a mt19937.o
	$(CC) -g $(BLDDFLAGS) $(CDFLAGS) -I${BZIP2_INC_DIR} -c byte-store.c -o byte-store.o
	$(CC) -g $(BLDDFLAGS) $(CDFLAGS) -I$(INCLUDES) -I${BZIP2_INC_DIR} -L"${BZIP2_LIB_DIR}" byte-store.o -o byte-store mt19937.a $(LIBS)

clean:
	rm -rf byte-store
	rm -rf *.a
	rm -rf *.o
	rm -rf *~
	rm -rf ${BZIP2_DIR}
	rm -rf ${BZIP2_SYM_DIR}
	rm -rf ${HTTPD_DIR}
	rm -rf ${HTTPD_SYM_DIR}
	rm -rf $(TESTDIR)/*~
	rm -rf $(TESTDIR)/*.bs
	rm -rf $(TESTDIR)/*.bs.blocks
	rm -rf $(TESTDIR)/*.rbs
	rm -rf $(TESTDIR)/*.rbs.blocks
	rm -rf $(TESTDIR)/*.cbs
	rm -rf $(TESTDIR)/*.cbs.blocks
	rm -rf $(PDFDIR)
#	rm -rf test/sample_bs_input.starch
#	rm -rf test/sample_bs_input.bed
#	rm -rf $(SAMPLEDIR)

# -----------------
# Performance tests
# -----------------

test-sample:
ifndef AWK_VERSION
	@$(error GNU 'awk' not found -- please install GNU awk!)
endif
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

test-sample-performance: test/sample_bs_input.bed byte-store test-sample-performance-prep test-sample-performance-generate-samples test-sample-performance-sut test-sample-performance-sqr test-sample-performance-sqr-bzip2 test-sample-performance-sqr-bzip2-split test-sample-performance-cat-data

test-sample-performance-prep: 
	mkdir -p $(SAMPLEDIR)

test-sample-performance-generate-samples:
	$(TESTDIR)/generate_samples.sh test/sample_bs_input.bed $(SAMPLEDIR)

test-sample-performance-sut:
	$(TESTDIR)/time_store_creation.sh pearson-r-sut $(SAMPLEDIR) $(PWD)/byte-store
	$(TESTDIR)/time_store_query_all_sut.sh pearson-r-sut $(SAMPLEDIR) $(PWD)/byte-store
	$(TESTDIR)/compress_store.sh pearson-r-sut $(SAMPLEDIR)
	$(TESTDIR)/measure_compression_ratios.sh pearson-r-sut $(SAMPLEDIR)
	$(TESTDIR)/measure_frequency.sh pearson-r-sut $(SAMPLEDIR) $(PWD)/byte-store
	$(TESTDIR)/accumulate_creation_times.sh pearson-r-sut $(SAMPLEDIR)
	$(TESTDIR)/accumulate_query_times.sh pearson-r-sut $(SAMPLEDIR)

test-sample-performance-sqr:
	$(TESTDIR)/time_store_creation.sh pearson-r-sqr $(SAMPLEDIR) $(PWD)/byte-store
	$(TESTDIR)/time_store_query_all.sh pearson-r-sqr $(SAMPLEDIR) $(PWD)/byte-store
	$(TESTDIR)/compress_store.sh pearson-r-sqr $(SAMPLEDIR)
	$(TESTDIR)/measure_compression_ratios.sh pearson-r-sqr $(SAMPLEDIR)
	$(TESTDIR)/measure_frequency.sh pearson-r-sqr $(SAMPLEDIR) $(PWD)/byte-store
	$(TESTDIR)/accumulate_creation_times.sh pearson-r-sqr $(SAMPLEDIR)
	$(TESTDIR)/accumulate_query_times.sh pearson-r-sqr $(SAMPLEDIR)

test-sample-performance-sqr-bzip2:
	$(TESTDIR)/time_store_creation_compressed.sh pearson-r-sqr-bzip2 $(SAMPLEDIR) $(PWD)/byte-store
	$(TESTDIR)/time_store_query_all_compressed.sh pearson-r-sqr-bzip2 $(SAMPLEDIR) $(PWD)/byte-store
	$(TESTDIR)/accumulate_creation_times.sh pearson-r-sqr-bzip2 $(SAMPLEDIR)
	$(TESTDIR)/accumulate_query_times.sh pearson-r-sqr-bzip2 $(SAMPLEDIR)

test-sample-performance-sqr-bzip2-split:
	$(TESTDIR)/time_store_creation_compressed.sh pearson-r-sqr-bzip2-split $(SAMPLEDIR) $(PWD)/byte-store
	$(TESTDIR)/time_store_query_all_compressed.sh pearson-r-sqr-bzip2-split $(SAMPLEDIR) $(PWD)/byte-store
	$(TESTDIR)/accumulate_creation_times_compressed.sh pearson-r-sqr-bzip2-split $(SAMPLEDIR)
	$(TESTDIR)/accumulate_query_times_compressed.sh pearson-r-sqr-bzip2-split $(SAMPLEDIR)

test-sample-performance-cat-data:
	cat $(SAMPLEDIR)/*.create_times > $(SAMPLEDIR)/create_times.txt
	cat $(SAMPLEDIR)/*.query_all_times > $(SAMPLEDIR)/query_all_times.txt
	cat $(SAMPLEDIR)/*.compression_ratios > $(SAMPLEDIR)/compression_ratios.txt

test-sample-graphs: test-sample-graphs-timing test-sample-graphs-compression test-sample-graphs-frequencies
	mkdir -p $(PDFDIR)
	mv $(SAMPLEDIR)/*.pdf $(PDFDIR)

test-sample-graphs-timing: test-sample-graphs-creation-timing test-sample-graphs-query-timing

test-sample-graphs-creation-timing:
	$(TESTDIR)/graph_create_timing.Rscript -i $(SAMPLEDIR)/create_times.txt -o $(SAMPLEDIR)/create_times -t "Store creation cost" -y "Avg. creation rate (sec/element)"

test-sample-graphs-query-timing:
	$(TESTDIR)/graph_query_timing.Rscript -i $(SAMPLEDIR)/query_all_times.txt -o $(SAMPLEDIR)/query_all_times -t "Store query cost" -y "Avg. query rate (sec/element)"
	$(TESTDIR)/graph_query_timing_pb.Rscript -i $(SAMPLEDIR)/query_all_times.txt -o $(SAMPLEDIR)/query_all_times_pb -t "Store query cost" -y "Avg. query rate (sec/element/byte)"
	grep -v "pearson-r-sut" $(SAMPLEDIR)/query_all_times.txt > $(SAMPLEDIR)/query_no_sut_times.txt
	$(TESTDIR)/graph_query_timing.Rscript -i $(SAMPLEDIR)/query_no_sut_times.txt -o $(SAMPLEDIR)/query_no_sut_times -t "Store query cost" -y "Avg. query rate (sec/element)"
	$(TESTDIR)/graph_query_timing_pb.Rscript -i $(SAMPLEDIR)/query_no_sut_times.txt -o $(SAMPLEDIR)/query_no_sut_times_pb -t "Store query cost" -y "Avg. query rate (sec/element/byte)"

test-sample-graphs-compression:
	$(TESTDIR)/graph_compression.Rscript -i $(SAMPLEDIR)/compression_ratios.txt -o $(SAMPLEDIR)/compression_ratios -t "Store compression efficiency" -y "Compression ratio"

test-sample-graphs-frequencies:
	$(TESTDIR)/graph_frequencies.sh $(TESTDIR)/graph_frequencies.Rscript $(SAMPLEDIR) pearson-r-sut
	$(TESTDIR)/graph_frequencies.sh $(TESTDIR)/graph_frequencies.Rscript $(SAMPLEDIR) pearson-r-sqr

# -----------------
# Special raw tests
# -----------------

test-pearsonr-sqr-raw-split-5K: test-pearsonr-sqr-raw-split-5K-create-bs512

test-pearsonr-sqr-raw-split-5K-create-bs512: byte-store test-pearsonr-sqr-raw-5K-prep

test-pearsonr-sqr-raw-5K-prep: test/sample_bs_input.starch test/sample_bs_input.bed test-sample-performance-prep
	sample -k 5000 --cstdio --preserve-order test/sample_bs_input.bed > $(SAMPLEDIR)/vec_test5K.bed

test-pearsonr-sqr-raw-5K-create-bs512: debug-byte-store test-pearsonr-sqr-raw-5K-prep
	$(PWD)/byte-store -t pearson-r-sqr-split -c -l $(SAMPLEDIR)/vec_test5K.bed -s $(SAMPLEDIR)/vec_test5K.sqr.bs512.rbs -r 512

test-pearsonr-sqr-raw-5K-query-bs512: 
	$(PWD)/byte-store -t pearson-r-sqr-split -q -l $(SAMPLEDIR)/vec_test5K.bed -s $(SAMPLEDIR)/vec_test5K.sqr.bs512.rbs -i 0-0

test-pearsonr-sqr-raw-5K-query-range-bs512: 
	$(PWD)/byte-store -t pearson-r-sqr-split -q -l $(SAMPLEDIR)/vec_test5K.bed -s $(SAMPLEDIR)/vec_test5K.sqr.bs512.rbs --query-range chr1:10000-chr1:50000

test-pearsonr-sqr-raw-split-1M: test-pearsonr-sqr-raw-split-1M-create-bs512

test-pearsonr-sqr-raw-split-1M-create-bs512: byte-store test-pearsonr-sqr-raw-1M-prep

test-pearsonr-sqr-raw-1M-prep: test/sample_bs_input.starch test/sample_bs_input.bed test-sample-performance-prep
#	sample -k 1000000 --cstdio --preserve-order test/sample_bs_input.bed > $(SAMPLEDIR)/vec_test1M.bed

test-pearsonr-sqr-raw-1M-create-bs512: debug-byte-store test-pearsonr-sqr-raw-1M-prep
	$(PWD)/byte-store -t pearson-r-sqr-split -c -l $(SAMPLEDIR)/vec_test1M.bed -s $(SAMPLEDIR)/vec_test1M.sqr.bs512.rbs -r 512

test-pearsonr-sqr-raw-1M-query-bs512:
	$(PWD)/byte-store -t pearson-r-sqr-split -q -l $(SAMPLEDIR)/vec_test1M.bed -s $(SAMPLEDIR)/vec_test1M.sqr.bs512.rbs -i 0-0

test-pearsonr-sqr-raw-1M-query-range-bs512:
	$(PWD)/byte-store -t pearson-r-sqr-split -q -l $(SAMPLEDIR)/vec_test1M.bed -s $(SAMPLEDIR)/vec_test1M.sqr.bs512.rbs --range-query chr1:10000-chr1:50000

test-pearsonr-sqr-raw-1M-create-bs1048576: debug-byte-store test-pearsonr-sqr-raw-1M-prep
	$(PWD)/byte-store -t pearson-r-sqr-split -c -l $(SAMPLEDIR)/vec_test1M.bed -s $(SAMPLEDIR)/vec_test1M.sqr.bs1048576.rbs -r 1048576

test-pearsonr-sqr-raw-1M-query-bs1048576:
	$(PWD)/byte-store -t pearson-r-sqr-split -q -l $(SAMPLEDIR)/vec_test1M.bed -s $(SAMPLEDIR)/vec_test1M.sqr.bs1048576.rbs -i 0-0

test-pearsonr-sqr-raw-1M-query-range-bs1048576:
	$(PWD)/byte-store -t pearson-r-sqr-split -q -l $(SAMPLEDIR)/vec_test1M.bed -s $(SAMPLEDIR)/vec_test1M.sqr.bs1048576.rbs --range-query chr1:10000-chr1:50000

# ------------------------
# Special compressed tests
# ------------------------

test-pearsonr-sqr-bzip2-1M: test-pearsonr-sqr-bzip2-1M-create-bs512

test-pearsonr-sqr-bzip2-1M-prep: test/sample_bs_input.starch test/sample_bs_input.bed test-sample-performance-prep
	sample -k 1000000 --preserve-order test/sample_bs_input.bed > $(SAMPLEDIR)/vec_test1M.bed

test-pearsonr-sqr-bzip2-1M-create-bs512: byte-store test-pearsonr-sqr-bzip2-1M-prep
	$(PWD)/byte-store -t pearson-r-sqr-bzip2 -c -l $(SAMPLEDIR)/vec_test1M.bed -s $(SAMPLEDIR)/vec_test1M.sqr.bs512.cbs -r 512

test-pearsonr-sqr-bzip2-1M-query-bs512: 
	$(PWD)/byte-store -t pearson-r-sqr-bzip2 -q -l $(SAMPLEDIR)/vec_test1M.bed -s $(SAMPLEDIR)/vec_test1M.sqr.bs512.cbs -i 0-0

test-pearsonr-sqr-bzip2-split-1M: test-pearsonr-sqr-bzip2-split-1M-create-bs512 test-pearsonr-sqr-bzip2-split-1M-create-bs256

test-pearsonr-sqr-bzip2-split-1M-prep: test/sample_bs_input.starch test/sample_bs_input.bed test-sample-performance-prep
	sample -k 1000000 --preserve-order test/sample_bs_input.bed > $(SAMPLEDIR)/vec_test1M.bed

test-pearsonr-sqr-bzip2-split-1M-create-bs512: byte-store test-pearsonr-sqr-bzip2-split-1M-prep
	$(PWD)/byte-store -t pearson-r-sqr-bzip2-split -c -l $(SAMPLEDIR)/vec_test1M.bed -s $(SAMPLEDIR)/vec_test1M.sqr.split.bs512.cbs -r 512

test-pearsonr-sqr-bzip2-split-1M-query-bs512:
	$(PWD)/byte-store -t pearson-r-sqr-bzip2-split -q -l $(SAMPLEDIR)/vec_test1M.bed -s $(SAMPLEDIR)/vec_test1M.sqr.split.bs512.cbs -i 0-0

test-pearsonr-sqr-bzip2-split-1M-create-bs256: byte-store test-pearsonr-sqr-bzip2-split-1M-prep
	$(PWD)/byte-store -t pearson-r-sqr-bzip2-split -c -l $(SAMPLEDIR)/vec_test1M.bed -s $(SAMPLEDIR)/vec_test1M.sqr.split.bs256.cbs -r 256

test-pearsonr-sqr-bzip2-split-1M-query-bs256:
	$(PWD)/byte-store -t pearson-r-sqr-bzip2-split -q -l $(SAMPLEDIR)/vec_test1M.bed -s $(SAMPLEDIR)/vec_test1M.sqr.split.bs256.cbs -i 0-0

# -------------
# General tests
# -------------

test-pearsonr-sut-4:
	$(PWD)/byte-store -t pearson-r-sut -c -l $(TESTDIR)/vec_test4.bed -s $(TESTDIR)/vec_test4.sut.bs
	$(PWD)/byte-store -t pearson-r-sut -q -l $(TESTDIR)/vec_test4.bed -s $(TESTDIR)/vec_test4.sut.bs -i 0-3

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
	$(PWD)/byte-store -t pearson-r-sqr -q -l $(TESTDIR)/vec_test4.bed -s $(TESTDIR)/vec_test4.sqr.bs -i 0-3

test-pearsonr-sqr-bzip2-4: test-pearsonr-sqr-bzip2-4-create test-pearsonr-sqr-bzip2-4-query

test-pearsonr-sqr-bzip2-4-create:
	$(PWD)/byte-store -t pearson-r-sqr-bzip2 -c -l $(TESTDIR)/vec_test4.bed -s $(TESTDIR)/vec_test4.sqr.cbs -r 1

test-pearsonr-sqr-bzip2-4-query:
	$(PWD)/byte-store -t pearson-r-sqr-bzip2 -q -l $(TESTDIR)/vec_test4.bed -s $(TESTDIR)/vec_test4.sqr.cbs -i 0-3

test-pearsonr-sqr-bzip2-4-frequency:
	$(PWD)/byte-store -t pearson-r-sqr-bzip2 -f -l $(TESTDIR)/vec_test4.bed -s $(TESTDIR)/vec_test4.sqr.cbs

test-pearsonr-sqr-bzip2-split-4: test-pearsonr-sqr-bzip2-split-4-create test-pearsonr-sqr-bzip2-split-4-query

test-pearsonr-sqr-bzip2-split-4-create:
	$(PWD)/byte-store -t pearson-r-sqr-bzip2-split -c -l $(TESTDIR)/vec_test4.bed -s $(TESTDIR)/vec_test4.sqr.cbs -r 1

test-pearsonr-sqr-bzip2-split-4-query:
	$(PWD)/byte-store -t pearson-r-sqr-bzip2-split -q -l $(TESTDIR)/vec_test4.bed -s $(TESTDIR)/vec_test4.sqr.cbs -i 0-3

test-pearsonr-sqr-bzip2-split-4-frequency:
	$(PWD)/byte-store -t pearson-r-sqr-bzip2-split -f -l $(TESTDIR)/vec_test4.bed -s $(TESTDIR)/vec_test4.sqr.cbs

test-pearsonr-sqr-1k: test-pearsonr-sqr-1k-create test-pearsonr-sqr-1k-query

test-pearsonr-sqr-1k-create:
	$(PWD)/byte-store -t pearson-r-sqr -c -l $(TESTDIR)/vec_test1000.bed -s $(TESTDIR)/vec_test1000.sqr.bs

test-pearsonr-sqr-1k-query:
	$(PWD)/byte-store -t pearson-r-sqr -q -l $(TESTDIR)/vec_test1000.bed -s $(TESTDIR)/vec_test1000.sqr.bs -i 0-999 | awk '$$7>=1.00'

test-pearsonr-sqr-bzip2-1k: test-pearsonr-sqr-bzip2-1k-create test-pearsonr-sqr-bzip2-1k-query

test-pearsonr-sqr-bzip2-1k-create:
	$(PWD)/byte-store -t pearson-r-sqr-bzip2 -c -l $(TESTDIR)/vec_test1000.bed -s $(TESTDIR)/vec_test1000.sqr.cbs -r 512

test-pearsonr-sqr-bzip2-1k-query:
	$(PWD)/byte-store -t pearson-r-sqr-bzip2 -q -l $(TESTDIR)/vec_test1000.bed -s $(TESTDIR)/vec_test1000.sqr.cbs -i 0-999 | awk '$$7>=1.00'

test-pearsonr-sqr-bzip2-1k-custom: test-pearsonr-sqr-bzip2-1k-custom-create test-pearsonr-sqr-bzip2-1k-custom-query

test-pearsonr-sqr-bzip2-1k-custom-create:
	$(PWD)/byte-store -t pearson-r-sqr-bzip2 -c -l $(TESTDIR)/vec_test1000.bed -s $(TESTDIR)/vec_test1000.sqr.custom.cbs -r 512 -e custom -n -0.21 -x 0.49

test-pearsonr-sqr-bzip2-1k-custom-query:
	$(PWD)/byte-store -t pearson-r-sqr-bzip2 -q -l $(TESTDIR)/vec_test1000.bed -s $(TESTDIR)/vec_test1000.sqr.custom.cbs -i 0-999 | awk '$$7>=1.00'

test-pearsonr-sqr-10k: test-pearsonr-sqr-10k-create test-pearsonr-sqr-10k-query

test-pearsonr-sqr-10k-create:
	$(PWD)/byte-store -t pearson-r-sqr -c -l $(TESTDIR)/vec_test10k.bed -s $(TESTDIR)/vec_test10k.sqr.bs

test-pearsonr-sqr-10k-query:
	$(PWD)/byte-store -t pearson-r-sqr -q -l $(TESTDIR)/vec_test10k.bed -s $(TESTDIR)/vec_test10k.sqr.bs -i 0-9999 | awk '$$7>=1.00'

test-pearsonr-sqr-bzip2-10k: test-pearsonr-sqr-bzip2-10k-create test-pearsonr-sqr-bzip2-10k-query

test-pearsonr-sqr-bzip2-10k-create:
	$(PWD)/byte-store -t pearson-r-sqr-bzip2 -c -l $(TESTDIR)/vec_test10k.bed -s $(TESTDIR)/vec_test10k.sqr.cbs -r 512

test-pearsonr-sqr-bzip2-10k-query:
	$(PWD)/byte-store -t pearson-r-sqr-bzip2 -q -l $(TESTDIR)/vec_test10k.bed -s $(TESTDIR)/vec_test10k.sqr.cbs -i 0-9999 | awk '$$7>=1.00'

test-pearsonr-sqr-bzip2-10k-custom: test-pearsonr-sqr-bzip2-10k-custom-create test-pearsonr-sqr-bzip2-10k-custom-query

test-pearsonr-sqr-bzip2-10k-custom-create:
	$(PWD)/byte-store -t pearson-r-sqr-bzip2 -c -l $(TESTDIR)/vec_test10k.bed -s $(TESTDIR)/vec_test10k.sqr.custom.cbs -r 512 -e custom -n -0.5 -x 0.5

test-pearsonr-sqr-bzip2-10k-custom-query:
	$(PWD)/byte-store -t pearson-r-sqr-bzip2 -q -l $(TESTDIR)/vec_test10k.bed -s $(TESTDIR)/vec_test10k.sqr.custom.cbs -i 0-9999 | awk '$$7>=1.00'

test-random-sqr:
	$(PWD)/byte-store -t random-sqr -c -l $(TESTDIR)/test1000.bed -s $(TESTDIR)/test1000.sqr.bs
	$(PWD)/byte-store -t random-sqr -q -l $(TESTDIR)/test1000.bed -s $(TESTDIR)/test1000.sqr.bs -i 0-999 | awk '$$7>=0.99'

test-random-buffered-sqr:
	$(PWD)/byte-store -t random-buffered-sqr -c -l $(TESTDIR)/test1000.bed -s $(TESTDIR)/test1000.sqr.bs
	$(PWD)/byte-store -t random-buffered-sqr -q -l $(TESTDIR)/test1000.bed -s $(TESTDIR)/test1000.sqr.bs -i 0-999 | awk '$$7>=0.99'

# ----------
# More tests
# ----------

metadata-test-pearson-split-create:
	$(PWD)/byte-store -t pearson-r-sqr-split -c -l $(TESTDIR)/master_with_signal_h40.bed -s $(TESTDIR)/master.12r.pearson.bs -r 12

metadata-test-pearson-split-query:
	$(PWD)/byte-store -t pearson-r-sqr-split -q -l $(TESTDIR)/master_with_signal_h40.bed -s $(TESTDIR)/master.12r.pearson.bs -i 0-0

metadata-test-pearson-split-query-daemon:
	$(PWD)/byte-store -t pearson-r-sqr-split -Q $(TEST_HTTPD_PORT) -l $(TESTDIR)/master_with_signal_h40.bed -s $(TESTDIR)/master.12r.pearson.bs

metadata-test-spearman-split-create:
	$(PWD)/byte-store -t spearman-rho-sqr-split -c -l $(TESTDIR)/master_with_signal_h40.bed -s $(TESTDIR)/master.12r.spearman.bs -r 12

metadata-test-spearman-split-query:
	$(PWD)/byte-store -t spearman-rho-sqr-split -q -l $(TESTDIR)/master_with_signal_h40.bed -s $(TESTDIR)/master.12r.spearman.bs -i 0-0

metadata-test-spearman-split-query-daemon:
	$(PWD)/byte-store -t spearman-rho-sqr-split -Q $(TEST_HTTPD_PORT) -l $(TESTDIR)/master_with_signal_h40.bed -s $(TESTDIR)/master.12r.spearman.bs
