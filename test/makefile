SHELL           := /bin/bash
PWD             := $(shell pwd)
SAMPLE          := $(shell `which sample` --help 2> /dev/null)
TEST_DIR         = $(PWD)
TEST_HTTPD_PORT  = 8000
PDF_DIR          = $(TEST_DIR)/pdf
#SAMPLE_DIR      = /Volumes/Data/byte-store
SAMPLE_DIR       = /tmp/byte-store
AWK_VERSION     := $(shell awk --version | grep "GNU Awk")
BIN_DIR          = ../

clean:
	rm -rf $(TEST_DIR)/*~
	rm -rf $(TEST_DIR)/*.bs
	rm -rf $(TEST_DIR)/*.bs.blocks
	rm -rf $(TEST_DIR)/*.rbs
	rm -rf $(TEST_DIR)/*.rbs.blocks
	rm -rf $(TEST_DIR)/*.cbs
	rm -rf $(TEST_DIR)/*.cbs.blocks
	rm -rf $(TEST_DIR)/*.key
	rm -rf $(TEST_DIR)/*.pem
	rm -rf $(PDF_DIR)
#   rm -rf test/sample_bs_input.starch
#   rm -rf test/sample_bs_input.bed
#   rm -rf $(SAMPLE_DIR)

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

sample_bs_input.starch:
	wget -qO- https://dl.dropboxusercontent.com/u/31495717/byte-store-test.starch > $@

sample_bs_input.bed: test/sample_bs_input.starch
	unstarch $^ > $@

test-sample-performance: sample_bs_input.bed byte-store test-sample-performance-prep test-sample-performance-generate-samples test-sample-performance-sut test-sample-performance-sqr test-sample-performance-sqr-bzip2 test-sample-performance-sqr-bzip2-split test-sample-performance-cat-data

test-sample-performance-prep: 
	mkdir -p $(SAMPLE_DIR)

test-sample-performance-generate-samples:
	$(TEST_DIR)/generate_samples.sh sample_bs_input.bed $(SAMPLE_DIR)

test-sample-performance-sut:
	$(TEST_DIR)/time_store_creation.sh pearson-r-sut $(SAMPLE_DIR) $(BIN_DIR)/byte-store
	$(TEST_DIR)/time_store_query_all_sut.sh pearson-r-sut $(SAMPLE_DIR) $(BIN_DIR)/byte-store
	$(TEST_DIR)/compress_store.sh pearson-r-sut $(SAMPLE_DIR)
	$(TEST_DIR)/measure_compression_ratios.sh pearson-r-sut $(SAMPLE_DIR)
	$(TEST_DIR)/measure_frequency.sh pearson-r-sut $(SAMPLE_DIR) $(BIN_DIR)/byte-store
	$(TEST_DIR)/accumulate_creation_times.sh pearson-r-sut $(SAMPLE_DIR)
	$(TEST_DIR)/accumulate_query_times.sh pearson-r-sut $(SAMPLE_DIR)

test-sample-performance-sqr:
	$(TEST_DIR)/time_store_creation.sh pearson-r-sqr $(SAMPLE_DIR) $(BIN_DIR)/byte-store
	$(TEST_DIR)/time_store_query_all.sh pearson-r-sqr $(SAMPLE_DIR) $(BIN_DIR)/byte-store
	$(TEST_DIR)/compress_store.sh pearson-r-sqr $(SAMPLE_DIR)
	$(TEST_DIR)/measure_compression_ratios.sh pearson-r-sqr $(SAMPLE_DIR)
	$(TEST_DIR)/measure_frequency.sh pearson-r-sqr $(SAMPLE_DIR) $(BIN_DIR)/byte-store
	$(TEST_DIR)/accumulate_creation_times.sh pearson-r-sqr $(SAMPLE_DIR)
	$(TEST_DIR)/accumulate_query_times.sh pearson-r-sqr $(SAMPLE_DIR)

test-sample-performance-sqr-bzip2:
	$(TEST_DIR)/time_store_creation_compressed.sh pearson-r-sqr-bzip2 $(SAMPLE_DIR) $(BIN_DIR)/byte-store
	$(TEST_DIR)/time_store_query_all_compressed.sh pearson-r-sqr-bzip2 $(SAMPLE_DIR) $(BIN_DIR)/byte-store
	$(TEST_DIR)/accumulate_creation_times.sh pearson-r-sqr-bzip2 $(SAMPLE_DIR)
	$(TEST_DIR)/accumulate_query_times.sh pearson-r-sqr-bzip2 $(SAMPLE_DIR)

test-sample-performance-sqr-bzip2-split:
	$(TEST_DIR)/time_store_creation_compressed.sh pearson-r-sqr-bzip2-split $(SAMPLE_DIR) $(BIN_DIR)/byte-store
	$(TEST_DIR)/time_store_query_all_compressed.sh pearson-r-sqr-bzip2-split $(SAMPLE_DIR) $(BIN_DIR)/byte-store
	$(TEST_DIR)/accumulate_creation_times_compressed.sh pearson-r-sqr-bzip2-split $(SAMPLE_DIR)
	$(TEST_DIR)/accumulate_query_times_compressed.sh pearson-r-sqr-bzip2-split $(SAMPLE_DIR)

test-sample-performance-cat-data:
	cat $(SAMPLE_DIR)/*.create_times > $(SAMPLE_DIR)/create_times.txt
	cat $(SAMPLE_DIR)/*.query_all_times > $(SAMPLE_DIR)/query_all_times.txt
	cat $(SAMPLE_DIR)/*.compression_ratios > $(SAMPLE_DIR)/compression_ratios.txt

test-sample-graphs: test-sample-graphs-timing test-sample-graphs-compression test-sample-graphs-frequencies
	mkdir -p $(PDF_DIR)
	mv $(SAMPLE_DIR)/*.pdf $(PDF_DIR)

test-sample-graphs-timing: test-sample-graphs-creation-timing test-sample-graphs-query-timing

test-sample-graphs-creation-timing:
	$(TEST_DIR)/graph_create_timing.Rscript -i $(SAMPLE_DIR)/create_times.txt -o $(SAMPLE_DIR)/create_times -t "Store creation cost" -y "Avg. creation rate (sec/element)"

test-sample-graphs-query-timing:
	$(TEST_DIR)/graph_query_timing.Rscript -i $(SAMPLE_DIR)/query_all_times.txt -o $(SAMPLE_DIR)/query_all_times -t "Store query cost" -y "Avg. query rate (sec/element)"
	$(TEST_DIR)/graph_query_timing_pb.Rscript -i $(SAMPLE_DIR)/query_all_times.txt -o $(SAMPLE_DIR)/query_all_times_pb -t "Store query cost" -y "Avg. query rate (sec/element/byte)"
	grep -v "pearson-r-sut" $(SAMPLE_DIR)/query_all_times.txt > $(SAMPLE_DIR)/query_no_sut_times.txt
	$(TEST_DIR)/graph_query_timing.Rscript -i $(SAMPLE_DIR)/query_no_sut_times.txt -o $(SAMPLE_DIR)/query_no_sut_times -t "Store query cost" -y "Avg. query rate (sec/element)"
	$(TEST_DIR)/graph_query_timing_pb.Rscript -i $(SAMPLE_DIR)/query_no_sut_times.txt -o $(SAMPLE_DIR)/query_no_sut_times_pb -t "Store query cost" -y "Avg. query rate (sec/element/byte)"

test-sample-graphs-compression:
	$(TEST_DIR)/graph_compression.Rscript -i $(SAMPLE_DIR)/compression_ratios.txt -o $(SAMPLE_DIR)/compression_ratios -t "Store compression efficiency" -y "Compression ratio"

test-sample-graphs-frequencies:
	$(TEST_DIR)/graph_frequencies.sh $(TEST_DIR)/graph_frequencies.Rscript $(SAMPLE_DIR) pearson-r-sut
	$(TEST_DIR)/graph_frequencies.sh $(TEST_DIR)/graph_frequencies.Rscript $(SAMPLE_DIR) pearson-r-sqr

# -----------------
# Special raw tests
# -----------------

test-pearsonr-sqr-raw-split-5K: test-pearsonr-sqr-raw-split-5K-create-bs512

test-pearsonr-sqr-raw-split-5K-create-bs512: byte-store test-pearsonr-sqr-raw-5K-prep

test-pearsonr-sqr-raw-5K-prep: sample_bs_input.starch sample_bs_input.bed test-sample-performance-prep
	sample -k 5000 --cstdio --preserve-order test/sample_bs_input.bed > $(SAMPLE_DIR)/vec_test5K.bed

test-pearsonr-sqr-raw-5K-create-bs512: test-pearsonr-sqr-raw-5K-prep
	$(BIN_DIR)/byte-store -t pearson-r-sqr-split -c -l $(SAMPLE_DIR)/vec_test5K.bed -s $(SAMPLE_DIR)/vec_test5K.sqr.bs512.rbs -r 512

test-pearsonr-sqr-raw-5K-query-bs512: 
	$(BIN_DIR)/byte-store -t pearson-r-sqr-split -q -l $(SAMPLE_DIR)/vec_test5K.bed -s $(SAMPLE_DIR)/vec_test5K.sqr.bs512.rbs -i 0-0

test-pearsonr-sqr-raw-5K-query-range-bs512: 
	$(BIN_DIR)/byte-store -t pearson-r-sqr-split -q -l $(SAMPLE_DIR)/vec_test5K.bed -s $(SAMPLE_DIR)/vec_test5K.sqr.bs512.rbs --query-range chr1:10000-chr1:50000

test-pearsonr-sqr-raw-split-1M: test-pearsonr-sqr-raw-split-1M-create-bs512

test-pearsonr-sqr-raw-split-1M-create-bs512: test-pearsonr-sqr-raw-1M-prep

test-pearsonr-sqr-raw-1M-prep: sample_bs_input.starch sample_bs_input.bed test-sample-performance-prep
#   sample -k 1000000 --cstdio --preserve-order test/sample_bs_input.bed > $(SAMPLE_DIR)/vec_test1M.bed

test-pearsonr-sqr-raw-1M-create-bs512: test-pearsonr-sqr-raw-1M-prep
	$(BIN_DIR)/byte-store -t pearson-r-sqr-split -c -l $(SAMPLE_DIR)/vec_test1M.bed -s $(SAMPLE_DIR)/vec_test1M.sqr.bs512.rbs -r 512

test-pearsonr-sqr-raw-1M-query-bs512:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-split -q -l $(SAMPLE_DIR)/vec_test1M.bed -s $(SAMPLE_DIR)/vec_test1M.sqr.bs512.rbs -i 0-0

test-pearsonr-sqr-raw-1M-query-range-bs512:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-split -q -l $(SAMPLE_DIR)/vec_test1M.bed -s $(SAMPLE_DIR)/vec_test1M.sqr.bs512.rbs --range-query chr1:10000-chr1:50000

test-pearsonr-sqr-raw-1M-create-bs1048576: test-pearsonr-sqr-raw-1M-prep
	$(BIN_DIR)/byte-store -t pearson-r-sqr-split -c -l $(SAMPLE_DIR)/vec_test1M.bed -s $(SAMPLE_DIR)/vec_test1M.sqr.bs1048576.rbs -r 1048576

test-pearsonr-sqr-raw-1M-query-bs1048576:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-split -q -l $(SAMPLE_DIR)/vec_test1M.bed -s $(SAMPLE_DIR)/vec_test1M.sqr.bs1048576.rbs -i 0-0

test-pearsonr-sqr-raw-1M-query-range-bs1048576:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-split -q -l $(SAMPLE_DIR)/vec_test1M.bed -s $(SAMPLE_DIR)/vec_test1M.sqr.bs1048576.rbs --range-query chr1:10000-chr1:50000

# ------------------------
# Special compressed tests
# ------------------------

test-pearsonr-sqr-bzip2-1M: test-pearsonr-sqr-bzip2-1M-create-bs512

test-pearsonr-sqr-bzip2-1M-prep: sample_bs_input.starch sample_bs_input.bed test-sample-performance-prep
	sample -k 1000000 --preserve-order test/sample_bs_input.bed > $(SAMPLE_DIR)/vec_test1M.bed

test-pearsonr-sqr-bzip2-1M-create-bs512: test-pearsonr-sqr-bzip2-1M-prep
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2 -c -l $(SAMPLE_DIR)/vec_test1M.bed -s $(SAMPLE_DIR)/vec_test1M.sqr.bs512.cbs -r 512

test-pearsonr-sqr-bzip2-1M-query-bs512: 
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2 -q -l $(SAMPLE_DIR)/vec_test1M.bed -s $(SAMPLE_DIR)/vec_test1M.sqr.bs512.cbs -i 0-0

test-pearsonr-sqr-bzip2-split-1M: test-pearsonr-sqr-bzip2-split-1M-create-bs512 test-pearsonr-sqr-bzip2-split-1M-create-bs256

test-pearsonr-sqr-bzip2-split-1M-prep: test/sample_bs_input.starch test/sample_bs_input.bed test-sample-performance-prep
	sample -k 1000000 --preserve-order test/sample_bs_input.bed > $(SAMPLE_DIR)/vec_test1M.bed

test-pearsonr-sqr-bzip2-split-1M-create-bs512: test-pearsonr-sqr-bzip2-split-1M-prep
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2-split -c -l $(SAMPLE_DIR)/vec_test1M.bed -s $(SAMPLE_DIR)/vec_test1M.sqr.split.bs512.cbs -r 512

test-pearsonr-sqr-bzip2-split-1M-query-bs512:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2-split -q -l $(SAMPLE_DIR)/vec_test1M.bed -s $(SAMPLE_DIR)/vec_test1M.sqr.split.bs512.cbs -i 0-0

test-pearsonr-sqr-bzip2-split-1M-create-bs256: test-pearsonr-sqr-bzip2-split-1M-prep
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2-split -c -l $(SAMPLE_DIR)/vec_test1M.bed -s $(SAMPLE_DIR)/vec_test1M.sqr.split.bs256.cbs -r 256

test-pearsonr-sqr-bzip2-split-1M-query-bs256:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2-split -q -l $(SAMPLE_DIR)/vec_test1M.bed -s $(SAMPLE_DIR)/vec_test1M.sqr.split.bs256.cbs -i 0-0

# -------------
# General tests
# -------------

test-pearsonr-sut-4:
	$(BIN_DIR)/byte-store -t pearson-r-sut -c -l $(TEST_DIR)/vec_test4.bed -s $(TEST_DIR)/vec_test4.sut.bs
	$(BIN_DIR)/byte-store -t pearson-r-sut -q -l $(TEST_DIR)/vec_test4.bed -s $(TEST_DIR)/vec_test4.sut.bs -i 0-3

test-pearsonr-sut-1k:
	$(BIN_DIR)/byte-store -t pearson-r-sut -c -l $(TEST_DIR)/vec_test1000.bed -s $(TEST_DIR)/vec_test1000.sut.bs
	$(BIN_DIR)/byte-store -t pearson-r-sut -q -l $(TEST_DIR)/vec_test1000.bed -s $(TEST_DIR)/vec_test1000.sut.bs -i 1-999 | awk '$$7>=1.00'

test-pearsonr-sut-10k:
	$(BIN_DIR)/byte-store -t pearson-r-sut -c -l $(TEST_DIR)/vec_test10k.bed -s $(TEST_DIR)/vec_test10k.sut.bs
	$(BIN_DIR)/byte-store -t pearson-r-sut -q -l $(TEST_DIR)/vec_test10k.bed -s $(TEST_DIR)/vec_test10k.sut.bs -i 0-9999 | awk '$$7>=1.00'

test-random-sut:
	$(BIN_DIR)/byte-store -t random-sut -c -l $(TEST_DIR)/test1000.bed -s $(TEST_DIR)/test1000.sut.bs
	$(BIN_DIR)/byte-store -t random-sut -q -l $(TEST_DIR)/test1000.bed -s $(TEST_DIR)/test1000.sut.bs -i 0-999 | awk '$$7>=0.99'

test-pearsonr-sqr-4:
	$(BIN_DIR)/byte-store -t pearson-r-sqr -c -l $(TEST_DIR)/vec_test4.bed -s $(TEST_DIR)/vec_test4.sqr.bs
	$(BIN_DIR)/byte-store -t pearson-r-sqr -q -l $(TEST_DIR)/vec_test4.bed -s $(TEST_DIR)/vec_test4.sqr.bs -i 0-3

test-pearsonr-sqr-bzip2-4: test-pearsonr-sqr-bzip2-4-create test-pearsonr-sqr-bzip2-4-query

test-pearsonr-sqr-bzip2-4-create:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2 -c -l $(TEST_DIR)/vec_test4.bed -s $(TEST_DIR)/vec_test4.sqr.cbs -r 1

test-pearsonr-sqr-bzip2-4-query:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2 -q -l $(TEST_DIR)/vec_test4.bed -s $(TEST_DIR)/vec_test4.sqr.cbs -i 0-3

test-pearsonr-sqr-bzip2-4-frequency:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2 -f -l $(TEST_DIR)/vec_test4.bed -s $(TEST_DIR)/vec_test4.sqr.cbs

test-pearsonr-sqr-bzip2-split-4: test-pearsonr-sqr-bzip2-split-4-create test-pearsonr-sqr-bzip2-split-4-query

test-pearsonr-sqr-bzip2-split-4-create:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2-split -c -l $(TEST_DIR)/vec_test4.bed -s $(TEST_DIR)/vec_test4.sqr.cbs -r 1

test-pearsonr-sqr-bzip2-split-4-query:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2-split -q -l $(TEST_DIR)/vec_test4.bed -s $(TEST_DIR)/vec_test4.sqr.cbs -i 0-3

test-pearsonr-sqr-bzip2-split-4-frequency:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2-split -f -l $(TEST_DIR)/vec_test4.bed -s $(TEST_DIR)/vec_test4.sqr.cbs

test-pearsonr-sqr-1k: test-pearsonr-sqr-1k-create test-pearsonr-sqr-1k-query

test-pearsonr-sqr-1k-create:
	$(BIN_DIR)/byte-store -t pearson-r-sqr -c -l $(TEST_DIR)/vec_test1000.bed -s $(TEST_DIR)/vec_test1000.sqr.bs

test-pearsonr-sqr-1k-query:
	$(BIN_DIR)/byte-store -t pearson-r-sqr -q -l $(TEST_DIR)/vec_test1000.bed -s $(TEST_DIR)/vec_test1000.sqr.bs -i 0-999 | awk '$$7>=1.00'

test-pearsonr-sqr-bzip2-1k: test-pearsonr-sqr-bzip2-1k-create test-pearsonr-sqr-bzip2-1k-query

test-pearsonr-sqr-bzip2-1k-create:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2 -c -l $(TEST_DIR)/vec_test1000.bed -s $(TEST_DIR)/vec_test1000.sqr.cbs -r 512

test-pearsonr-sqr-bzip2-1k-query:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2 -q -l $(TEST_DIR)/vec_test1000.bed -s $(TEST_DIR)/vec_test1000.sqr.cbs -i 0-999 | awk '$$7>=1.00'

test-pearsonr-sqr-bzip2-1k-custom: test-pearsonr-sqr-bzip2-1k-custom-create test-pearsonr-sqr-bzip2-1k-custom-query

test-pearsonr-sqr-bzip2-1k-custom-create:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2 -c -l $(TEST_DIR)/vec_test1000.bed -s $(TEST_DIR)/vec_test1000.sqr.custom.cbs -r 512 -e custom -n -0.21 -x 0.49

test-pearsonr-sqr-bzip2-1k-custom-query:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2 -q -l $(TEST_DIR)/vec_test1000.bed -s $(TEST_DIR)/vec_test1000.sqr.custom.cbs -i 0-999 | awk '$$7>=1.00'

test-pearsonr-sqr-10k: test-pearsonr-sqr-10k-create test-pearsonr-sqr-10k-query

test-pearsonr-sqr-10k-create:
	$(BIN_DIR)/byte-store -t pearson-r-sqr -c -l $(TEST_DIR)/vec_test10k.bed -s $(TEST_DIR)/vec_test10k.sqr.bs

test-pearsonr-sqr-10k-query:
	$(BIN_DIR)/byte-store -t pearson-r-sqr -q -l $(TEST_DIR)/vec_test10k.bed -s $(TEST_DIR)/vec_test10k.sqr.bs -i 0-9999 | awk '$$7>=1.00'

test-pearsonr-sqr-bzip2-10k: test-pearsonr-sqr-bzip2-10k-create test-pearsonr-sqr-bzip2-10k-query

test-pearsonr-sqr-bzip2-10k-create:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2 -c -l $(TEST_DIR)/vec_test10k.bed -s $(TEST_DIR)/vec_test10k.sqr.cbs -r 512

test-pearsonr-sqr-bzip2-10k-query:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2 -q -l $(TEST_DIR)/vec_test10k.bed -s $(TEST_DIR)/vec_test10k.sqr.cbs -i 0-9999 | awk '$$7>=1.00'

test-pearsonr-sqr-bzip2-10k-custom: test-pearsonr-sqr-bzip2-10k-custom-create test-pearsonr-sqr-bzip2-10k-custom-query

test-pearsonr-sqr-bzip2-10k-custom-create:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2 -c -l $(TEST_DIR)/vec_test10k.bed -s $(TEST_DIR)/vec_test10k.sqr.custom.cbs -r 512 -e custom -n -0.5 -x 0.5

test-pearsonr-sqr-bzip2-10k-custom-query:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-bzip2 -q -l $(TEST_DIR)/vec_test10k.bed -s $(TEST_DIR)/vec_test10k.sqr.custom.cbs -i 0-9999 | awk '$$7>=1.00'

test-random-sqr:
	$(BIN_DIR)/byte-store -t random-sqr -c -l $(TEST_DIR)/test1000.bed -s $(TEST_DIR)/test1000.sqr.bs
	$(BIN_DIR)/byte-store -t random-sqr -q -l $(TEST_DIR)/test1000.bed -s $(TEST_DIR)/test1000.sqr.bs -i 0-999 | awk '$$7>=0.99'

test-random-buffered-sqr:
	$(BIN_DIR)/byte-store -t random-buffered-sqr -c -l $(TEST_DIR)/test1000.bed -s $(TEST_DIR)/test1000.sqr.bs
	$(BIN_DIR)/byte-store -t random-buffered-sqr -q -l $(TEST_DIR)/test1000.bed -s $(TEST_DIR)/test1000.sqr.bs -i 0-999 | awk '$$7>=0.99'

# --------------
# Metadata tests
# --------------

metadata-test-pearson-split-create:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-split -c -l $(TEST_DIR)/master_with_signal_h40.bed -s $(TEST_DIR)/master.12r.pearson.bs -r 12

metadata-test-pearson-split-query:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-split -q -l $(TEST_DIR)/master_with_signal_h40.bed -s $(TEST_DIR)/master.12r.pearson.bs -i 0-0

metadata-test-pearson-split-query-daemon:
	$(BIN_DIR)/byte-store -t pearson-r-sqr-split -Q $(TEST_HTTPD_PORT) -l $(TEST_DIR)/master_with_signal_h40.bed -s $(TEST_DIR)/master.12r.pearson.bs

metadata-test-spearman-split-create:
	$(BIN_DIR)/byte-store -t spearman-rho-sqr-split -c -l $(TEST_DIR)/master_with_signal_h40.bed -s $(TEST_DIR)/master.12r.spearman.bs -r 12

metadata-test-spearman-split-query:
	$(BIN_DIR)/byte-store -t spearman-rho-sqr-split -q -l $(TEST_DIR)/master_with_signal_h40.bed -s $(TEST_DIR)/master.12r.spearman.bs -i 0-0

metadata-test-spearman-split-query-http-daemon:
	$(BIN_DIR)/byte-store -t spearman-rho-sqr-split -Q $(TEST_HTTPD_PORT) -l $(TEST_DIR)/master_with_signal_h40.col4nr.bed -s $(TEST_DIR)/master.12r.spearman.bs

self-signed-certs:
	openssl genrsa -out test/server.key 1024
	openssl req -days 365 -out test/server.pem -new -x509 -key test/server.key

metadata-test-spearman-split-query-https-daemon:
	$(BIN_DIR)/byte-store -t spearman-rho-sqr-split -Q $(TEST_HTTPD_PORT) -l $(TEST_DIR)/master_with_signal_h40.col4nr.bed -s $(TEST_DIR)/master.12r.spearman.bs -E -K test/server.key -C test/server.pem

metadata-test-jaccard-split-create:
	$(BIN_DIR)/byte-store -t jaccard-index-sqr-split -c -l $(TEST_DIR)/master_with_signal_h40.bed -s $(TEST_DIR)/master.12r.jaccard.bs -r 12

metadata-test-jaccard-split-query:
	$(BIN_DIR)/byte-store -t jaccard-index-sqr-split -q -l $(TEST_DIR)/master_with_signal_h40.bed -s $(TEST_DIR)/master.12r.jaccard.bs -i 0-0

# ------------
# Mutual tests
# ------------

mutual-test-jaccard-split-create-with-valgrind:
	valgrind --track-origins=yes --keep-stacktraces=alloc-and-free --leak-check=full --show-leak-kinds=all $(BIN_DIR)/byte-store -t jaccard-index-sqr-split -c -l $(TEST_DIR)/mutual_jaccard_test.bed -s $(TEST_DIR)/mutual_jaccard_split.bs -r 2

mutual-test-jaccard-split-create:
	$(BIN_DIR)/byte-store -t jaccard-index-sqr-split -c -l $(TEST_DIR)/mutual_jaccard_test.bed -s $(TEST_DIR)/mutual_jaccard_split.bs -r 2

mutual-test-jaccard-create:
	$(BIN_DIR)/byte-store -t jaccard-index-sqr -c -l $(TEST_DIR)/mutual_jaccard_test.bed -s $(TEST_DIR)/mutual_jaccard.bs

mutual-test-jaccard-split-query:
	$(BIN_DIR)/byte-store -t jaccard-index-sqr-split -q -l $(TEST_DIR)/mutual_jaccard_test.bed -s $(TEST_DIR)/mutual_jaccard_split.bs -i 1-3 --mutual-query

mutual-test-jaccard-query:
	$(BIN_DIR)/byte-store -t jaccard-index-sqr -q -l $(TEST_DIR)/mutual_jaccard_test.bed -s $(TEST_DIR)/mutual_jaccard.bs -i 1-3 --mutual-query

mutual-test-jaccard-query-filter:
	$(BIN_DIR)/byte-store -t jaccard-index-sqr -q -l $(TEST_DIR)/mutual_jaccard_test.bed -s $(TEST_DIR)/mutual_jaccard.bs -i 1-3 --mutual-query --score-filter-gt 0.5
