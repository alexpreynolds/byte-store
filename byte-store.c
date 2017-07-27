#include "byte-store.h"

int
main(int argc, char** argv)
{
    bs_init_globals();
    bs_init_command_line_options(argc, argv);

    sut_store_t* sut_store = NULL;
    sqr_store_t* sqr_store = NULL;
    lookup_t* lookup = NULL;

    /* initialize lookup table */

    if ((bs_globals.store_type == kStorePearsonRSquareMatrixSplitSingleChunkMetadata) ||
        (bs_globals.store_type == kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadata) ||
        (bs_globals.store_type == kStoreJaccardIndexSquareMatrixSplitSingleChunkMetadata) ||
        (bs_globals.store_type == kStoreOchiaiSimilaritySquareMatrixSplitSingleChunkMetadata) ||
        (bs_globals.store_type == kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunkMetadata) ||
        (bs_globals.store_type == kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunkMetadata) ||
        (bs_globals.store_type == kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunkMetadata)) {
        lookup = bs_init_lookup(bs_globals.lookup_fn,
                                kFalse,
                                kFalse,
                                kFalse);
    }
    else if ((bs_globals.store_type == kStorePearsonRSquareMatrix) ||
             (bs_globals.store_type == kStorePearsonRSquareMatrixSplit) ||
             (bs_globals.store_type == kStorePearsonRSquareMatrixSplitSingleChunk) ||
             (bs_globals.store_type == kStorePearsonRSquareMatrixBzip2) ||
             (bs_globals.store_type == kStorePearsonRSquareMatrixBzip2Split)) {
        lookup = bs_init_lookup(bs_globals.lookup_fn,
                                !bs_globals.store_query_flag,
                                kTrue,
                                kFalse);
    }
    else if (((bs_globals.store_type == kStoreSpearmanRhoSquareMatrix) ||
             (bs_globals.store_type == kStoreSpearmanRhoSquareMatrixSplit) ||
             (bs_globals.store_type == kStoreSpearmanRhoSquareMatrixSplitSingleChunk) ||
             (bs_globals.store_type == kStoreSpearmanRhoSquareMatrixBzip2) ||
             (bs_globals.store_type == kStoreSpearmanRhoSquareMatrixBzip2Split)) &&
             (!bs_globals.store_query_daemon_flag)) {
        lookup = bs_init_lookup(bs_globals.lookup_fn,
                                !bs_globals.store_query_flag,
                                kTrue,
                                kTrue);
    }
    else if ((bs_globals.store_type == kStoreJaccardIndexSquareMatrix) ||
             (bs_globals.store_type == kStoreJaccardIndexSquareMatrixSplit) ||
             (bs_globals.store_type == kStoreJaccardIndexSquareMatrixSplitSingleChunk) ||
             (bs_globals.store_type == kStoreOchiaiSimilaritySquareMatrix) ||
             (bs_globals.store_type == kStoreOchiaiSimilaritySquareMatrixSplit) ||
             (bs_globals.store_type == kStoreOchiaiSimilaritySquareMatrixSplitSingleChunk) ||
             (bs_globals.store_type == kStorePearsonPhiSimilaritySquareMatrix) ||
             (bs_globals.store_type == kStorePearsonPhiSimilaritySquareMatrixSplit) ||
             (bs_globals.store_type == kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunk) ||
             (bs_globals.store_type == kStoreRogersAndTanimotoSimilaritySquareMatrix) ||
             (bs_globals.store_type == kStoreRogersAndTanimotoSimilaritySquareMatrixSplit) ||
             (bs_globals.store_type == kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunk) ||
             (bs_globals.store_type == kStoreNormalizedPointwiseMutualInformationSquareMatrix) ||
             (bs_globals.store_type == kStoreNormalizedPointwiseMutualInformationSquareMatrixSplit) ||
             (bs_globals.store_type == kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunk)) {
        lookup = bs_init_lookup(bs_globals.lookup_fn,
                                !bs_globals.store_query_flag,
                                kFalse,
                                kFalse);
    }
    else if (bs_globals.store_query_daemon_flag) {
        lookup = bs_init_lookup(bs_globals.lookup_fn,
                                kFalse,
                                kFalse,
                                kFalse);
    }
    else {
        lookup = bs_init_lookup(bs_globals.lookup_fn,
                                !bs_globals.store_query_flag,
                                kFalse,
                                kFalse);
    }

    /* create or query byte-store container */

    switch (bs_globals.store_type) {
    case kStorePearsonRSUT:
    case kStoreRandomSUT:
        sut_store = bs_init_sut_store(lookup->nelems);
        if (bs_globals.store_create_flag) {
            switch (bs_globals.store_type) {
            case kStoreRandomSUT:
                bs_populate_sut_store_with_random_scores(sut_store);
                break;
            case kStorePearsonRSUT:
                bs_populate_sut_store(sut_store, 
                                      lookup, 
                                      &bs_pearson_r_signal);
                break;
            case kStoreRandomBufferedSquareMatrix:
            case kStoreRandomSquareMatrix:
            case kStorePearsonRSquareMatrix:
            case kStorePearsonRSquareMatrixSplit:
            case kStorePearsonRSquareMatrixSplitSingleChunk:
            case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
            case kStorePearsonRSquareMatrixBzip2:
            case kStorePearsonRSquareMatrixBzip2Split:
            case kStoreSpearmanRhoSquareMatrix:
            case kStoreSpearmanRhoSquareMatrixSplit:
            case kStoreSpearmanRhoSquareMatrixSplitSingleChunk:
            case kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadata:
            case kStoreSpearmanRhoSquareMatrixBzip2:
            case kStoreSpearmanRhoSquareMatrixBzip2Split:
            case kStoreJaccardIndexSquareMatrix:
            case kStoreJaccardIndexSquareMatrixSplit:
            case kStoreJaccardIndexSquareMatrixSplitSingleChunk:
            case kStoreJaccardIndexSquareMatrixSplitSingleChunkMetadata:
            case kStoreOchiaiSimilaritySquareMatrix:
            case kStoreOchiaiSimilaritySquareMatrixSplit:
            case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunk:
            case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunkMetadata:
            case kStorePearsonPhiSimilaritySquareMatrix:
            case kStorePearsonPhiSimilaritySquareMatrixSplit:
            case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunk:
            case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunkMetadata:
            case kStoreRogersAndTanimotoSimilaritySquareMatrix:
            case kStoreRogersAndTanimotoSimilaritySquareMatrixSplit:
            case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunk:
            case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunkMetadata:
            case kStoreNormalizedPointwiseMutualInformationSquareMatrix:
            case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplit:
            case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunk:
            case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunkMetadata:
            case kStoreUndefined:
                fprintf(stderr, "Error: You should never see this error! (A)\n");
                exit(EXIT_FAILURE);
            }
        }
        else if (bs_globals.store_query_flag && !bs_globals.store_query_daemon_flag) {
            boolean_t rows_found = kFalse;
            switch (bs_globals.store_query_kind) {
            case kQueryKindIndex:
                rows_found = bs_parse_query_index_str(lookup);
                break;
            case kQueryKindRange:
                rows_found = bs_parse_query_range_str(lookup,
                                                      bs_globals.store_query_str,
                                                      &bs_globals.store_query_idx_start,
                                                      &bs_globals.store_query_idx_end);
                break;
            case kQueryKindMultipleIndices:
            case kQueryKindMultipleIndicesFromFile:
            case kQueryKindUndefined:
                fprintf(stderr, "Error: Query type unsupported with SUT!\n");
                exit(EXIT_FAILURE);
            }
            if (rows_found && (bs_globals.store_filter == kScoreFilterNone))
                bs_print_sut_store_to_bed7(lookup, 
                                           sut_store, 
                                           stdout);
            else if (rows_found)
                bs_print_sut_filtered_store_to_bed7(lookup, 
                                                    sut_store, 
                                                    stdout, 
                                                    bs_globals.score_filter_cutoff, 
                                                    bs_globals.store_filter);
        }
        else if (bs_globals.store_frequency_flag) {
            bs_print_sut_frequency_to_txt(lookup, 
                                          sut_store, 
                                          stdout);
        }
        bs_delete_sut_store(&sut_store);
        break;
    case kStorePearsonRSquareMatrix:
    case kStorePearsonRSquareMatrixSplit:
    case kStorePearsonRSquareMatrixSplitSingleChunk:
    case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
    case kStorePearsonRSquareMatrixBzip2:
    case kStorePearsonRSquareMatrixBzip2Split:
    case kStoreSpearmanRhoSquareMatrix:
    case kStoreSpearmanRhoSquareMatrixSplit:
    case kStoreSpearmanRhoSquareMatrixSplitSingleChunk:
    case kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadata:
    case kStoreSpearmanRhoSquareMatrixBzip2:
    case kStoreSpearmanRhoSquareMatrixBzip2Split:
    case kStoreJaccardIndexSquareMatrix:
    case kStoreJaccardIndexSquareMatrixSplit:
    case kStoreJaccardIndexSquareMatrixSplitSingleChunk:
    case kStoreJaccardIndexSquareMatrixSplitSingleChunkMetadata:
    case kStoreOchiaiSimilaritySquareMatrix:
    case kStoreOchiaiSimilaritySquareMatrixSplit:
    case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunk:
    case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunkMetadata:
    case kStorePearsonPhiSimilaritySquareMatrix:
    case kStorePearsonPhiSimilaritySquareMatrixSplit:
    case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunk:
    case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunkMetadata:
    case kStoreRogersAndTanimotoSimilaritySquareMatrix:
    case kStoreRogersAndTanimotoSimilaritySquareMatrixSplit:
    case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunk:
    case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunkMetadata:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrix:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplit:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunk:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunkMetadata:
    case kStoreRandomSquareMatrix:
    case kStoreRandomBufferedSquareMatrix:
        sqr_store = bs_init_sqr_store(lookup->nelems);
        if (bs_globals.store_create_flag) {
            switch (bs_globals.store_type) {
            case kStoreRandomBufferedSquareMatrix:
                bs_populate_sqr_store_with_buffered_random_scores(sqr_store);
                break;
            case kStoreRandomSquareMatrix:
                bs_populate_sqr_store_with_random_scores(sqr_store);
                break;
            case kStorePearsonRSquareMatrix:
                bs_populate_sqr_store(sqr_store, 
                                      lookup, 
                                      &bs_pearson_r_signal);
                break;
            case kStorePearsonRSquareMatrixSplit:
                bs_populate_sqr_split_store(sqr_store, 
                                            lookup, 
                                            bs_globals.store_row_chunk_size, 
                                            &bs_pearson_r_signal, 
                                            kScoreVarietyPearsonR);
                break;
            case kStorePearsonRSquareMatrixSplitSingleChunk:
                bs_populate_sqr_split_store_chunk(sqr_store, 
                                                  lookup, 
                                                  bs_globals.store_row_chunk_size, 
                                                  bs_globals.store_row_chunk_offset, 
                                                  &bs_pearson_r_signal);
                break;
            case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
                bs_populate_sqr_split_store_chunk_metadata(sqr_store, 
                                                           lookup, 
                                                           bs_globals.store_row_chunk_size, 
                                                           kScoreVarietyPearsonR);
                break;
            case kStorePearsonRSquareMatrixBzip2:
                bs_populate_sqr_bzip2_store(sqr_store, 
                                            lookup, 
                                            bs_globals.store_row_chunk_size, 
                                            &bs_pearson_r_signal, 
                                            kScoreVarietyPearsonR);
                break;
            case kStorePearsonRSquareMatrixBzip2Split:
                bs_populate_sqr_bzip2_split_store(sqr_store, 
                                                  lookup, 
                                                  bs_globals.store_row_chunk_size, 
                                                  &bs_pearson_r_signal, 
                                                  kScoreVarietyPearsonR);
                break;
            case kStoreSpearmanRhoSquareMatrix:
                bs_populate_sqr_store(sqr_store, 
                                      lookup, 
                                      &bs_spearman_rho_signal_v2);
                break;
            case kStoreSpearmanRhoSquareMatrixSplit:
                bs_populate_sqr_split_store(sqr_store, 
                                            lookup, 
                                            bs_globals.store_row_chunk_size, 
                                            &bs_spearman_rho_signal_v2, 
                                            kScoreVarietySpearmanRho);
                break;
            case kStoreSpearmanRhoSquareMatrixSplitSingleChunk:
                bs_populate_sqr_split_store_chunk(sqr_store, 
                                                  lookup, 
                                                  bs_globals.store_row_chunk_size, 
                                                  bs_globals.store_row_chunk_offset, 
                                                  &bs_spearman_rho_signal_v2);
                break;
            case kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadata:
                bs_populate_sqr_split_store_chunk_metadata(sqr_store, 
                                                           lookup, 
                                                           bs_globals.store_row_chunk_size, 
                                                           kScoreVarietySpearmanRho);
                break;
            case kStoreSpearmanRhoSquareMatrixBzip2:
                bs_populate_sqr_bzip2_store(sqr_store, 
                                            lookup, 
                                            bs_globals.store_row_chunk_size, 
                                            &bs_spearman_rho_signal_v2, 
                                            kScoreVarietySpearmanRho);
                break;
            case kStoreSpearmanRhoSquareMatrixBzip2Split:
                bs_populate_sqr_bzip2_split_store(sqr_store, 
                                                  lookup, 
                                                  bs_globals.store_row_chunk_size, 
                                                  &bs_spearman_rho_signal_v2, 
                                                  kScoreVarietySpearmanRho);
                break;
            case kStoreJaccardIndexSquareMatrix:
                bs_populate_sqr_store(sqr_store, 
                                      lookup, 
                                      &bs_jaccard_index_signal);
                break;
            case kStoreJaccardIndexSquareMatrixSplit:
                bs_populate_sqr_split_store(sqr_store, 
                                            lookup, 
                                            bs_globals.store_row_chunk_size, 
                                            &bs_jaccard_index_signal, 
                                            kScoreVarietyJaccardIndex);
                break;
            case kStoreJaccardIndexSquareMatrixSplitSingleChunk:
                bs_populate_sqr_split_store_chunk(sqr_store, 
                                                  lookup, 
                                                  bs_globals.store_row_chunk_size, 
                                                  bs_globals.store_row_chunk_offset, 
                                                  &bs_jaccard_index_signal);
                break;
            case kStoreJaccardIndexSquareMatrixSplitSingleChunkMetadata:
                bs_populate_sqr_split_store_chunk_metadata(sqr_store, 
                                                           lookup, 
                                                           bs_globals.store_row_chunk_size, 
                                                           kScoreVarietyJaccardIndex);
                break;
            case kStoreOchiaiSimilaritySquareMatrix:
                bs_populate_sqr_store(sqr_store, 
                                      lookup, 
                                      &bs_ochiai_similarity_signal);
                break;
            case kStoreOchiaiSimilaritySquareMatrixSplit:
                bs_populate_sqr_split_store(sqr_store, 
                                            lookup, 
                                            bs_globals.store_row_chunk_size, 
                                            &bs_ochiai_similarity_signal, 
                                            kScoreVarietyOchiaiSimilarity);
                break;
            case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunk:
                bs_populate_sqr_split_store_chunk(sqr_store, 
                                                  lookup, 
                                                  bs_globals.store_row_chunk_size, 
                                                  bs_globals.store_row_chunk_offset, 
                                                  &bs_ochiai_similarity_signal);
                break;
            case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunkMetadata:
                bs_populate_sqr_split_store_chunk_metadata(sqr_store, 
                                                           lookup, 
                                                           bs_globals.store_row_chunk_size, 
                                                           kScoreVarietyOchiaiSimilarity);
                break;
            case kStorePearsonPhiSimilaritySquareMatrix:
                bs_populate_sqr_store(sqr_store, 
                                      lookup, 
                                      &bs_pearson_phi_similarity_signal);
                break;
            case kStorePearsonPhiSimilaritySquareMatrixSplit:
                bs_populate_sqr_split_store(sqr_store, 
                                            lookup, 
                                            bs_globals.store_row_chunk_size, 
                                            &bs_pearson_phi_similarity_signal, 
                                            kScoreVarietyPearsonPhiSimilarity);
                break;
            case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunk:
                bs_populate_sqr_split_store_chunk(sqr_store, 
                                                  lookup, 
                                                  bs_globals.store_row_chunk_size, 
                                                  bs_globals.store_row_chunk_offset, 
                                                  &bs_pearson_phi_similarity_signal);
                break;
            case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunkMetadata:
                bs_populate_sqr_split_store_chunk_metadata(sqr_store, 
                                                           lookup, 
                                                           bs_globals.store_row_chunk_size, 
                                                           kScoreVarietyPearsonPhiSimilarity);
                break;
            case kStoreRogersAndTanimotoSimilaritySquareMatrix:
                bs_populate_sqr_store(sqr_store, 
                                      lookup, 
                                      &bs_rogers_and_tanimoto_similarity_signal);
                break;
            case kStoreRogersAndTanimotoSimilaritySquareMatrixSplit:
                bs_populate_sqr_split_store(sqr_store, 
                                            lookup, 
                                            bs_globals.store_row_chunk_size, 
                                            &bs_rogers_and_tanimoto_similarity_signal, 
                                            kScoreVarietyRogersAndTanimotoSimilarity);
                break;
            case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunk:
                bs_populate_sqr_split_store_chunk(sqr_store, 
                                                  lookup, 
                                                  bs_globals.store_row_chunk_size, 
                                                  bs_globals.store_row_chunk_offset, 
                                                  &bs_rogers_and_tanimoto_similarity_signal);
                break;
            case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunkMetadata:
                bs_populate_sqr_split_store_chunk_metadata(sqr_store, 
                                                           lookup, 
                                                           bs_globals.store_row_chunk_size, 
                                                           kScoreVarietyRogersAndTanimotoSimilarity);
                break;
            case kStoreNormalizedPointwiseMutualInformationSquareMatrix:
                bs_populate_sqr_store(sqr_store, 
                                      lookup, 
                                      &bs_normalized_pointwise_mutual_information_signal);
                break;
            case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplit:
                bs_populate_sqr_split_store(sqr_store, 
                                            lookup, 
                                            bs_globals.store_row_chunk_size, 
                                            &bs_normalized_pointwise_mutual_information_signal, 
                                            kScoreVarietyNormalizedPointwiseMutualInformation);
                break;
            case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunk:
                bs_populate_sqr_split_store_chunk(sqr_store, 
                                                  lookup, 
                                                  bs_globals.store_row_chunk_size, 
                                                  bs_globals.store_row_chunk_offset, 
                                                  &bs_normalized_pointwise_mutual_information_signal);
                break;
            case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunkMetadata:
                bs_populate_sqr_split_store_chunk_metadata(sqr_store, 
                                                           lookup, 
                                                           bs_globals.store_row_chunk_size, 
                                                           kScoreVarietyNormalizedPointwiseMutualInformation);
                break;
            case kStorePearsonRSUT:
            case kStoreRandomSUT:
            case kStoreUndefined:
                fprintf(stderr, "Error: You should never see this error! (B)\n");
                exit(EXIT_FAILURE);
            }
        }
        else if (bs_globals.store_query_flag && !bs_globals.store_query_daemon_flag) {
            /* set up query type */
            boolean_t contiguous_rows_found = kFalse;
            boolean_t separate_rows_found = kFalse;
            switch (bs_globals.store_query_kind) {
            case kQueryKindIndex:
                contiguous_rows_found = bs_parse_query_index_str(lookup);
                break;
            case kQueryKindRange:
                contiguous_rows_found = bs_parse_query_range_str(lookup,
                                                                 bs_globals.store_query_str,
                                                                 &bs_globals.store_query_idx_start,
                                                                 &bs_globals.store_query_idx_end);
                break;
            case kQueryKindMultipleIndices:
                separate_rows_found = bs_parse_query_multiple_index_str(lookup, 
                                                                        bs_globals.store_query_str);
                break;
            case kQueryKindMultipleIndicesFromFile:
                separate_rows_found = bs_parse_query_multiple_index_file(lookup, 
                                                                         bs_globals.store_query_str);
                break;
            case kQueryKindUndefined:
                fprintf(stderr, "Error: Query type unsupported!\n");
                exit(EXIT_FAILURE);
            }

            if (contiguous_rows_found) {
                /* set up ROI */
                size_t* query_roi = NULL;
                if (bs_globals.store_query_idx_end < bs_globals.store_query_idx_start) {
                    fprintf(stderr, "Error: Query index bounds not ordered\n");
                    exit(EXIT_FAILURE);
                }
                size_t query_roi_num = bs_globals.store_query_idx_end - bs_globals.store_query_idx_start + 1;
                query_roi = malloc(query_roi_num * sizeof(*query_roi));
                if (!query_roi) {
                    fprintf(stderr, "Error: Could not allocate space for query ROI array (client)\n");
                    exit(EXIT_FAILURE);
                }
                for (size_t idx = 0; idx < query_roi_num; idx++) {
                    query_roi[idx] = bs_globals.store_query_idx_start + idx;
                }
                /* extract from raw or uncompressed square matrix */
                switch (bs_globals.store_type) {
                case kStoreRandomBufferedSquareMatrix:
                case kStoreRandomSquareMatrix:
                case kStorePearsonRSquareMatrix:
                case kStoreSpearmanRhoSquareMatrix:
                case kStoreJaccardIndexSquareMatrix:
                case kStoreOchiaiSimilaritySquareMatrix:
                case kStorePearsonPhiSimilaritySquareMatrix:
                case kStoreRogersAndTanimotoSimilaritySquareMatrix:
                case kStoreNormalizedPointwiseMutualInformationSquareMatrix:
                    if (bs_globals.store_filter == kScoreFilterNone)
                        bs_print_sqr_store_to_bed7(lookup,
                                                   sqr_store,
                                                   bs_globals.store_filter_mutual_set,
                                                   stdout,
                                                   bs_globals.store_query_idx_start,
                                                   bs_globals.store_query_idx_end);
                    else
                        bs_print_sqr_filtered_store_to_bed7(lookup,
                                                            sqr_store,
                                                            bs_globals.store_filter_mutual_set,
                                                            stdout,
                                                            bs_globals.score_filter_cutoff,
                                                            bs_globals.score_filter_cutoff_lower_bound,
                                                            bs_globals.score_filter_cutoff_upper_bound,
                                                            bs_globals.store_filter,
                                                            bs_globals.store_query_idx_start,
                                                            bs_globals.store_query_idx_end);
                    break;
                case kStorePearsonRSquareMatrixSplit:
                case kStoreSpearmanRhoSquareMatrixSplit:
                case kStoreJaccardIndexSquareMatrixSplit:
                case kStoreOchiaiSimilaritySquareMatrixSplit:
                case kStorePearsonPhiSimilaritySquareMatrixSplit:
                case kStoreRogersAndTanimotoSimilaritySquareMatrixSplit:
                case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplit:
                    if (bs_globals.store_filter == kScoreFilterNone) {
                        bs_print_sqr_split_store_separate_rows_to_bed7(lookup,
                                                                       sqr_store,
                                                                       bs_globals.store_filter_mutual_set,
                                                                       stdout,
                                                                       query_roi,
                                                                       query_roi,
                                                                       query_roi_num);
                    }
                    else {
                        bs_print_sqr_filtered_split_store_separate_rows_to_bed7(lookup,
                                                                                sqr_store,
                                                                                stdout,
                                                                                query_roi,
                                                                                query_roi,
                                                                                query_roi_num,
                                                                                bs_globals.score_filter_cutoff,
                                                                                bs_globals.score_filter_cutoff_lower_bound,
                                                                                bs_globals.score_filter_cutoff_upper_bound,
                                                                                bs_globals.store_filter);
                    }
                    break;
                case kStorePearsonRSquareMatrixBzip2:
                case kStoreSpearmanRhoSquareMatrixBzip2:
                    if (bs_globals.store_filter == kScoreFilterNone)
                        bs_print_sqr_bzip2_store_to_bed7(lookup, 
                                                         sqr_store, 
                                                         stdout);
                    else
                        bs_print_sqr_filtered_bzip2_store_to_bed7(lookup, 
                                                                  sqr_store, 
                                                                  stdout, 
                                                                  bs_globals.score_filter_cutoff, 
                                                                  bs_globals.score_filter_cutoff_lower_bound, 
                                                                  bs_globals.score_filter_cutoff_upper_bound, 
                                                                  bs_globals.store_filter);
                    break;
                case kStorePearsonRSquareMatrixBzip2Split:
                case kStoreSpearmanRhoSquareMatrixBzip2Split:
                    if (bs_globals.store_filter == kScoreFilterNone)
                        bs_print_sqr_bzip2_split_store_to_bed7(lookup, 
                                                               sqr_store, 
                                                               stdout);
                    else
                        bs_print_sqr_filtered_bzip2_split_store_to_bed7(lookup, 
                                                                        sqr_store, 
                                                                        stdout, 
                                                                        bs_globals.score_filter_cutoff, 
                                                                        bs_globals.score_filter_cutoff_lower_bound, 
                                                                        bs_globals.score_filter_cutoff_upper_bound, 
                                                                        bs_globals.store_filter);
                    break;
                case kStorePearsonRSUT:
                case kStoreRandomSUT:
                case kStorePearsonRSquareMatrixSplitSingleChunk:
                case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
                case kStoreSpearmanRhoSquareMatrixSplitSingleChunk:
                case kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadata:
                case kStoreJaccardIndexSquareMatrixSplitSingleChunk:
                case kStoreJaccardIndexSquareMatrixSplitSingleChunkMetadata:
                case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunk:
                case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunkMetadata:
                case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunk:
                case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunkMetadata:
                case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunk:
                case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunkMetadata:
                case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunk:
                case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunkMetadata:
                case kStoreUndefined:
                    fprintf(stderr, "Error: You should never see this error! (C1)\n");
                    exit(EXIT_FAILURE);
                }
                /* clean-up */
                free(query_roi);
                query_roi = NULL;
            }
            if (separate_rows_found) {
                switch (bs_globals.store_type) {
                case kStorePearsonRSquareMatrixSplit:
                case kStoreSpearmanRhoSquareMatrixSplit:
                case kStoreJaccardIndexSquareMatrixSplit:
                case kStoreOchiaiSimilaritySquareMatrixSplit:
                case kStorePearsonPhiSimilaritySquareMatrixSplit:
                case kStoreRogersAndTanimotoSimilaritySquareMatrixSplit:
                case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplit:
                    switch (bs_globals.store_query_kind) {
                    case kQueryKindMultipleIndicesFromFile:
                        if (bs_globals.store_filter == kScoreFilterNone) {
                            bs_print_sqr_split_store_separate_rows_to_bed7_file(lookup,
                                                                                sqr_store,
                                                                                bs_globals.store_query_str,
                                                                                bs_globals.store_filter_mutual_set,
                                                                                stdout);
                        }
                        else {
                            bs_print_sqr_filtered_split_store_separate_rows_to_bed7_file(lookup,
                                                                                         sqr_store,
                                                                                         bs_globals.store_query_str,
                                                                                         bs_globals.store_filter_mutual_set,
                                                                                         stdout,
                                                                                         bs_globals.score_filter_cutoff,
                                                                                         bs_globals.score_filter_cutoff_lower_bound,
                                                                                         bs_globals.score_filter_cutoff_upper_bound,
                                                                                         bs_globals.store_filter);
                            }
                        break;
                    default:
                        if (bs_globals.store_filter == kScoreFilterNone) {
                            bs_print_sqr_split_store_separate_rows_to_bed7(lookup,
                                                                           sqr_store,
                                                                           bs_globals.store_filter_mutual_set,
                                                                           stdout,
                                                                           bs_globals.store_query_indices_starts,
                                                                           bs_globals.store_query_indices_ends,
                                                                           bs_globals.store_query_indices_num);
                        }
                        else {
                            bs_print_sqr_filtered_split_store_separate_rows_to_bed7(lookup,
                                                                                    sqr_store,
                                                                                    stdout,
                                                                                    bs_globals.store_query_indices_starts,
                                                                                    bs_globals.store_query_indices_ends,
                                                                                    bs_globals.store_query_indices_num,
                                                                                    bs_globals.score_filter_cutoff,
                                                                                    bs_globals.score_filter_cutoff_lower_bound,
                                                                                    bs_globals.score_filter_cutoff_upper_bound,
                                                                                    bs_globals.store_filter);
                        }
                        break;
                    }
                    break;
                case kStoreRandomBufferedSquareMatrix:
                case kStoreRandomSquareMatrix:
                case kStorePearsonRSquareMatrix:
                case kStorePearsonRSquareMatrixSplitSingleChunk:
                case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
                case kStorePearsonRSquareMatrixBzip2:
                case kStorePearsonRSquareMatrixBzip2Split:
                case kStorePearsonRSUT:
                case kStoreSpearmanRhoSquareMatrix:
                case kStoreSpearmanRhoSquareMatrixSplitSingleChunk:
                case kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadata:
                case kStoreSpearmanRhoSquareMatrixBzip2:
                case kStoreSpearmanRhoSquareMatrixBzip2Split:
                case kStoreJaccardIndexSquareMatrix:
                case kStoreJaccardIndexSquareMatrixSplitSingleChunk:
                case kStoreJaccardIndexSquareMatrixSplitSingleChunkMetadata:
                case kStoreOchiaiSimilaritySquareMatrix:
                case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunk:
                case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunkMetadata:
                case kStorePearsonPhiSimilaritySquareMatrix:
                case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunk:
                case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunkMetadata:
                case kStoreRogersAndTanimotoSimilaritySquareMatrix:
                case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunk:
                case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunkMetadata:
                case kStoreNormalizedPointwiseMutualInformationSquareMatrix:
                case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunk:
                case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunkMetadata:
                case kStoreRandomSUT:
                case kStoreUndefined:
                    fprintf(stderr, "Error: You should never see this error! (C2)\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
        else if (bs_globals.store_frequency_flag) {
            switch (bs_globals.store_type) {
            case kStoreRandomBufferedSquareMatrix:
            case kStoreRandomSquareMatrix:
            case kStorePearsonRSquareMatrix:
            case kStoreSpearmanRhoSquareMatrix:
            case kStoreJaccardIndexSquareMatrix:
            case kStoreOchiaiSimilaritySquareMatrix:
            case kStorePearsonPhiSimilaritySquareMatrix:
            case kStoreRogersAndTanimotoSimilaritySquareMatrix:
            case kStoreNormalizedPointwiseMutualInformationSquareMatrix:
                bs_print_sqr_store_frequency_to_txt(lookup, 
                                                    sqr_store, 
                                                    stdout);
                break;
            case kStorePearsonRSquareMatrixSplit:
            case kStoreSpearmanRhoSquareMatrixSplit:
            case kStoreJaccardIndexSquareMatrixSplit:
            case kStoreOchiaiSimilaritySquareMatrixSplit:
            case kStorePearsonPhiSimilaritySquareMatrixSplit:
            case kStoreRogersAndTanimotoSimilaritySquareMatrixSplit:
            case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplit:
                bs_print_sqr_split_store_frequency_to_txt(lookup, 
                                                          sqr_store, 
                                                          stdout);
                break;
            case kStorePearsonRSquareMatrixBzip2:
            case kStoreSpearmanRhoSquareMatrixBzip2:
                bs_print_sqr_bzip2_store_frequency_to_txt(lookup, 
                                                          sqr_store, 
                                                          stdout);
                break;
            case kStorePearsonRSquareMatrixBzip2Split:
            case kStoreSpearmanRhoSquareMatrixBzip2Split:
                bs_print_sqr_bzip2_split_store_frequency_to_txt(lookup, 
                                                                sqr_store, 
                                                                stdout);
                break;
            case kStorePearsonRSUT:
            case kStoreRandomSUT:
            case kStorePearsonRSquareMatrixSplitSingleChunk:
            case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
            case kStoreSpearmanRhoSquareMatrixSplitSingleChunk:
            case kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadata:
            case kStoreJaccardIndexSquareMatrixSplitSingleChunk:
            case kStoreJaccardIndexSquareMatrixSplitSingleChunkMetadata:
            case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunk:
            case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunkMetadata:
            case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunk:
            case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunkMetadata:
            case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunk:
            case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunkMetadata:
            case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunk:
            case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunkMetadata:
            case kStoreUndefined:
                fprintf(stderr, "Error: You should never see this error! (D)\n");
                exit(EXIT_FAILURE);
            }
        }
        else if (bs_globals.store_query_daemon_flag) {
            bs_qd_test_dependencies();
            fprintf(stderr, "Info: bedextract is located at: [%s]\n", bs_globals.bedextract_path);
            fprintf(stderr, "Info: bedops is located at:     [%s]\n", bs_globals.bedops_path);
            fprintf(stderr, "Info: bedmap is located at:     [%s]\n", bs_globals.bedmap_path);
            fprintf(stderr, "Info: sort-bed is located at:   [%s]\n", bs_globals.sortbed_path);
            bs_globals.lookup_ptr = lookup;
            bs_globals.sqr_store_ptr = sqr_store;
            struct MHD_Daemon *daemon = NULL;
            if (!bs_globals.enable_ssl) {
                daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION,
                                          bs_globals.store_query_daemon_port,
                                          NULL,
                                          NULL,
                                          &bs_qd_answer_to_connection,
                                          NULL,
                                          MHD_OPTION_NOTIFY_COMPLETED, &bs_qd_request_completed,
                                          NULL,
                                          MHD_OPTION_END);
            }
            else {
                daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION | MHD_USE_SSL,
                                          bs_globals.store_query_daemon_port,
                                          NULL,
                                          NULL,
                                          &bs_qd_answer_to_connection,
                                          NULL,
                                          MHD_OPTION_HTTPS_MEM_KEY, bs_globals.ssl_key_pem,
                                          MHD_OPTION_HTTPS_MEM_CERT, bs_globals.ssl_cert_pem,
                                          MHD_OPTION_NOTIFY_COMPLETED, &bs_qd_request_completed,
                                          NULL,
                                          MHD_OPTION_END);
            }

            if (!daemon) {
                fprintf(stderr, "Error: Unable to initialize query daemon!\n");
                exit(EXIT_FAILURE);
            }
            fprintf(stdout, "Info: Initialized query httpd...\n");
            if (bs_globals.store_query_daemon_hostname) {
                fprintf(stdout, "Info: Examples of requests include:\n"\
                    "\t'$ curl -i -X GET \"%s://%s:%d\"'\n" \
                    "Or:\n"\
                    "\t'$ curl -i -X GET \"%s://%s:%d/random?filter-type=greater-than-inclusive&filter-value=0.50\"'\n"\
                    "Or:\n"\
                    "\t'$ curl -i -X POST -H \"Content-Type: multipart/form-data\" -F \"file=@test/elements.bed\" \"%s://%s:%d/elements\"'\n"\
                    "Or:\n"\
                    "\t'$ curl -i -X POST -H \"Content-Type: multipart/form-data\" -F \"file=@test/elements.bed\" \"%s://%s:%d/elements?filter-type=within-exclusive&filter-value=0.35:1\"'\n"\
                    "Etc.\n\n"\
                    "%s",
                    (bs_globals.enable_ssl ? "https" : "http"),
                    bs_globals.store_query_daemon_hostname,
                    bs_globals.store_query_daemon_port,
                    (bs_globals.enable_ssl ? "https" : "http"),
                    bs_globals.store_query_daemon_hostname,
                    bs_globals.store_query_daemon_port,
                    (bs_globals.enable_ssl ? "https" : "http"),
                    bs_globals.store_query_daemon_hostname,
                    bs_globals.store_query_daemon_port,
                    (bs_globals.enable_ssl ? "https" : "http"),
                    bs_globals.store_query_daemon_hostname,
                    bs_globals.store_query_daemon_port,
                    (bs_globals.enable_ssl ? "(Add the \"-k\" option if using SSL with self-signed certificates.)\n" : ""));
            }
            else {
                fprintf(stdout, "Info: Examples of requests include:\n"\
                    "\t'$ curl -i -X GET \"%s://hostname:port\"'\n"\
                    "Or:\n"\
                    "\t'$ curl -i -X GET \"%s://hostname:port/random?filter-type=greater-than-inclusive&filter-value=0.50\"'\n"\
                    "Or:\n"\
                    "\t'$ curl -i -X POST -H \"Content-Type: multipart/form-data\" -F \"file=@test/elements.bed\" \"%s://hostname:port/elements\"'\n"\
                    "Or:\n"\
                    "\t'$ curl -i -X POST -H \"Content-Type: multipart/form-data\" -F \"file=@test/elements.bed\" \"%s://hostname:port/elements?filter-type=within-exclusive&filter-value=0.35:1\"'\n"\
                    "Etc.\n\n"\
                    "%s",
                    (bs_globals.enable_ssl ? "https" : "http"),
                    (bs_globals.enable_ssl ? "https" : "http"),
                    (bs_globals.enable_ssl ? "https" : "http"),
                    (bs_globals.enable_ssl ? "https" : "http"),
                    (bs_globals.enable_ssl ? "(Add the \"-k\" option if using SSL with self-signed certificates.)\n" : ""));
            }
            fprintf(stdout, "\nPress <Return> to stop the server...\n");
            getchar(); /* wait for it... */
            fprintf(stdout, "Closing %s daemon...\n", (bs_globals.enable_ssl ? "https" : "http"));
            MHD_stop_daemon(daemon);
        }
        bs_delete_sqr_store(&sqr_store);
        break;
    case kStoreUndefined:
        if (bs_globals.lookup_frequency_flag) {
            if (bs_globals.permutation_test_flag) {
                bs_permute_lookup(lookup, stdout);
            }
            else if (!bs_globals.permutation_test_flag) {
                bs_print_lookup_frequency(lookup, stdout);
            }
        }
        else {
            fprintf(stderr, "Error: You should never see this error! (E)\n");
            bs_print_usage(stderr);
            exit(EXIT_FAILURE);
        }
    }

    bs_delete_lookup(&lookup);

    return EXIT_SUCCESS;
}

static char*
bs_qd_load_file(const char* fn)
{
    FILE* fp = NULL;
    char* buf = NULL;
    long size = 0;

    size = bs_qd_get_file_size(fn);
    if (size == 0) {
        return NULL;
    }

    fp = fopen(fn, "rb");
    if (!fp) {
        return NULL;
    }

    buf = malloc(size + 1);
    if (!buf) {
        fclose(fp);
        return NULL;
    }
    buf[size] = '\0';

    if (fread(buf, 1, size, fp) != (size_t) size) {
        free(buf);
        buf = NULL;
    }

    fclose(fp);
    return buf;
}

static long
bs_qd_get_file_size(const char* filename)
{
    FILE* fp = NULL;
    long size = 0;

    fp = fopen(filename, "rb");
    if (fp) {
        if ((fseek(fp, 0, SEEK_END) != 0) || ((size = ftell(fp)) == -1)) {
            size = 0;
        }
        fclose(fp);
    }
    return size;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static void
bs_qd_request_completed(void* cls, struct MHD_Connection* connection, void** con_cls, enum MHD_RequestTerminationCode toe)
{
    uint64_t timestamp;
    bs_qd_connection_info_t *con_info = *con_cls;
    if (con_info) {
        fprintf(stdout, "Request [%" PRIu64 "]: [%s] => [%s] ended\n", con_info->timestamp, bs_qd_connection_method_type_to_str(con_info->method), bs_qd_request_type_to_str(con_info->request_type));
    }
    else {
        fprintf(stdout, "Request: Requested completed without connection information!\n");
        return;
    }

    /* clean up connection information struct */
    if (con_info) {
        timestamp = con_info->timestamp;
        if (con_info->post_processor) {
            fprintf(stdout, "Request [%" PRIu64 "]: Destroying POST processor...\n", timestamp);
            MHD_destroy_post_processor(con_info->post_processor);
            fprintf(stdout, "Request [%" PRIu64 "]: Destroyed POST processor...\n", timestamp);
        }
        if (con_info->upload_fp) {
            fprintf(stdout, "Request [%" PRIu64 "]: fclose()-ing upload FILE*...\n", timestamp);
            fclose(con_info->upload_fp);
            con_info->upload_fp = NULL;
            fprintf(stdout, "Request [%" PRIu64 "]: fclose()-ed upload FILE*...\n", timestamp);
        }
        if (con_info->upload_filename) {
            fprintf(stdout, "Request [%" PRIu64 "]: Attempting to delete upload file...\n", timestamp);
            int err = unlink(con_info->upload_filename);
            if (err == -1) {
                fprintf(stderr, "Error: Could not delete temporary upload file [%s]! Error: [%s]\n", con_info->upload_filename, strerror(errno));
            }
            fprintf(stderr, "Request [%" PRIu64 "]: Deleted temporary upload file [%s]\n", timestamp, con_info->upload_filename);
            free(con_info->upload_filename);
            fprintf(stderr, "Request [%" PRIu64 "]: Freed temporary upload filename char*\n", timestamp);
        }
        if (con_info->query_index_fp) {
            fprintf(stdout, "Request [%" PRIu64 "]: fclose()-ing query index FILE*...\n", timestamp);
            fclose(con_info->query_index_fp);
            con_info->query_index_fp = NULL;
            fprintf(stdout, "Request [%" PRIu64 "]: fclose()-ed query index FILE*...\n", timestamp);
        }
        if (con_info->query_index_filename) {
            fprintf(stdout, "Request [%" PRIu64 "]: Attempting to delete query index file...\n", timestamp);
            int err = unlink(con_info->query_index_filename);
            if (err == -1) {
                fprintf(stderr, "Error: Could not delete temporary query index file [%s]! Error: [%s]\n", con_info->query_index_filename, strerror(errno));
            }
            fprintf(stderr, "Request [%" PRIu64 "]: Deleted temporary query index file [%s]\n", timestamp, con_info->query_index_filename);
            free(con_info->query_index_filename);
            fprintf(stderr, "Request [%" PRIu64 "]: Freed temporary query index filename char*\n", timestamp);
        }
        free(con_info);
        fprintf(stderr, "Request [%" PRIu64 "]: Freed connection information pointer\n", timestamp);
        *con_cls = NULL;
    }

    /* print final status */
    switch(toe) {
        case MHD_REQUEST_TERMINATED_COMPLETED_OK:
            fprintf(stdout, "Request [%" PRIu64 "]: Request completed OK!\n", timestamp);
            break;
        case MHD_REQUEST_TERMINATED_WITH_ERROR:
            fprintf(stdout, "Request [%" PRIu64 "]: Request completed with an error!\n", timestamp);
            break;
        case MHD_REQUEST_TERMINATED_TIMEOUT_REACHED:
            fprintf(stdout, "Request [%" PRIu64 "]: Request timed out!\n", timestamp);
            break;
        case MHD_REQUEST_TERMINATED_DAEMON_SHUTDOWN:
            fprintf(stdout, "Request [%" PRIu64 "]: Request session closed due to server shutdown!\n", timestamp);
            break;
        default:
            break;
    }
    fflush(stdout);
}
#pragma GCC diagnostic pop

static const char*
bs_qd_connection_method_type_to_str(bs_qd_connection_method_t t)
{
    switch(t) {
        case kBSQDConnectionMethodGET:
            return "GET";
        case kBSQDConnectionMethodHEAD:
            return "HEAD";
        case kBSQDConnectionMethodPOST:
            return "POST";
        case kBSQDConnectionMethodUndefined:
            return "Undefined connection method!";
        default:
            break;
    }
    return NULL;
}

static const char*
bs_qd_request_type_to_str(bs_qd_request_t t)
{
    switch(t) {
        case kBSQDRequestNotFound:
            return "Not found";
        case kBSQDRequestParametersNotFound:
            return "Parameters not found";
        case kBSQDRequestGeneric:
            return "Generic request";
        case kBSQDRequestInformation:
            return "Information";
        case kBSQDRequestRandom:
            return "Random";
        case kBSQDRequestRandomViaHeap:
            return "Random (via in-heap buffer)";
        case kBSQDRequestRandomViaTemporaryFile:
            return "Random (via temporary file)";
        case kBSQDRequestElements:
            return "Elements";
        case kBSQDRequestElementsViaHeap:
            return "Elements (via in-heap buffer)";
        case kBSQDRequestElementsViaTemporaryFile:
            return "Elements (via temporary file)";
        case kBSQDRequestUndefined:
            return "Undefined";
        case kBSQDRequestMalformed:
            return "Malformed";
        case kBSQDRequestUploadTooLarge:
            return "Upload file size too large";
        default:
            break;
    }
    return NULL;
}

static uint64_t
bs_qd_timestamp()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;
}

/**
 * @brief      bs_answer_to_connection()
 *
 * @details    Return a response to given query
 *
 * @params     cls                (void*)        argument given together with the function
 *                                               pointer when the handler was registered with MHD
 *             url                (const char*)  the requested url
 *             method             (const char*)  the HTTP method used (#MHD_HTTP_METHOD_GET,
 *                                               #MHD_HTTP_METHOD_PUT, etc.)
 *             version            (const char*)  the HTTP version string (i.e.
 *                                               #MHD_HTTP_VERSION_1_1)
 *             upload_data        (const char*)  the data being uploaded (excluding HEADERS,
 *                                               for a POST that fits into memory and that is encoded
 *                                               with a supported encoding, the POST data will NOT be
 *                                               given in upload_data and is instead available as
 *                                               part of #MHD_get_connection_values; very large POST
 *                                               data *will* be made available incrementally in
 *                                               @a upload_data)
 *             upload_data_size   (size_t*)      set initially to the size of the
 *                                               @a upload_data provided; the method must update this
 *                                               value to the number of bytes NOT processed;
 *             con_cls            (void**)       pointer that the callback can set to some
 *                                               address and that will be preserved by MHD for future
 *                                               calls for this request; since the access handler may
 *                                               be called many times (i.e., for a PUT/POST operation
 *                                               with plenty of upload data) this allows the application
 *                                               to easily associate some request-specific state.
 *                                               If necessary, this state can be cleaned up in the
 *                                               global #MHD_RequestCompletedCallback (which
 *                                               can be set with the #MHD_OPTION_NOTIFY_COMPLETED).
 *                                               Initially, `*con_cls` will be NULL.
 *
 * @return     (int) response success or failure
 */

/* 
    This response does not use some of the parameters in 
    an MHD_AccessHandlerCallback function, so we direct the 
    compiler to ignore unused parameter warnings 
*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static int
bs_qd_answer_to_connection(void* cls, struct MHD_Connection *connection, const char* url, const char* method, const char* version, const char* upload_data, size_t* upload_data_size, void** con_cls)
{
    struct MHD_Response *response;
    unsigned int i;
    int ret = MHD_NO;
    bs_qd_connection_info_t* con_info = NULL;

    i = 0;
    while ((request_pages[i].url != NULL) && (strcmp(request_pages[i].url, url) != 0)) {
        i++;
    }

    /* instantiate and populate connection information struct members */
    /* launch desired URL handler */
    if (! *con_cls) {
        con_info = malloc(sizeof(bs_qd_connection_info_t));
        if (!con_info) {
            return MHD_NO;
        }
        con_info->timestamp = bs_qd_timestamp();
        con_info->request_type = kBSQDRequestUndefined;
        if (strcmp(method, MHD_HTTP_METHOD_GET) == 0) {
            con_info->method = kBSQDConnectionMethodGET;
        }
        else if (strcmp(method, MHD_HTTP_METHOD_HEAD) == 0) {
            con_info->method = kBSQDConnectionMethodHEAD;
        }
        else if (strcmp(method, MHD_HTTP_METHOD_POST) == 0) {
            con_info->method = kBSQDConnectionMethodPOST;
        }
        con_info->post_processor = NULL;
        con_info->upload_fp = NULL;
        con_info->upload_filename = NULL;
        con_info->upload_filesize = 0;
        con_info->query_index_fp = NULL;
        con_info->query_index_filename = NULL;
        *con_cls = (void*) con_info;

        if (!request_pages[i].url) {
            ret = MHD_NO;
        }
        else if ((con_info->method == kBSQDConnectionMethodGET) || (con_info->method == kBSQDConnectionMethodHEAD)) {
            ret = request_pages[i].handler(request_pages[i].handler_cls, request_pages[i].mime, connection, con_info, NULL, NULL);
        }
        else if (con_info->method == kBSQDConnectionMethodPOST) {
            /* allowed POST requests */
            if ((strcmp(request_pages[i].url, kBSQDURLElements) == 0) || (strcmp(request_pages[i].url, kBSQDURLElementsViaHeap) == 0) || (strcmp(request_pages[i].url, kBSQDURLElementsViaTemporaryFile) == 0)) {
                if (strcmp(request_pages[i].url, kBSQDURLElements) == 0) {
                    con_info->request_type = kBSQDRequestElementsViaHeap;
                }
                else if (strcmp(request_pages[i].url, kBSQDURLElementsViaHeap) == 0) {
                    con_info->request_type = kBSQDRequestElementsViaHeap;
                }
                else if (strcmp(request_pages[i].url, kBSQDURLElementsViaTemporaryFile) == 0) {
                    con_info->request_type = kBSQDRequestElementsViaTemporaryFile;
                }
                con_info->post_processor = MHD_create_post_processor(connection, BS_QD_POST_BUFFER_SIZE, &bs_qd_iterate_elements_post, con_info);
                ret = MHD_YES;
            }
        }
        if (ret != MHD_YES) {
            fprintf(stderr, "Error: Failed to handle the following URL `%s` and method `%s`\n", url, bs_qd_connection_method_type_to_str(con_info->method));
            return bs_qd_request_malformed(cls, request_pages[i].mime, connection, con_info, upload_data, upload_data_size);
        }
        return ret;
    }
    else {
        con_info = (bs_qd_connection_info_t*) *con_cls;
        if ((con_info->method == kBSQDConnectionMethodPOST) && ((strcmp(request_pages[i].url, kBSQDURLElements) == 0) || (strcmp(request_pages[i].url, kBSQDURLElementsViaHeap) == 0) || (strcmp(request_pages[i].url, kBSQDURLElementsViaTemporaryFile) == 0))) {
            if (*upload_data_size != 0) {
                if (MHD_post_process(con_info->post_processor, upload_data, *upload_data_size) != MHD_YES) {
                    return bs_qd_request_malformed(cls, request_pages[i].mime, connection, con_info, upload_data, upload_data_size);
                }
                *upload_data_size = 0;
                return MHD_YES;
            }
            else {
                /* close the upload file pointer so that it can be reopened later on */
                fclose(con_info->upload_fp);
                con_info->upload_fp = NULL;
                /* test if the file that was uploaded is larger than the allowed size */
                if (con_info->upload_filesize >= UPLOAD_FILESIZE_MAX) {
                    return bs_qd_request_upload_too_large(cls, request_pages[i].mime, connection, con_info, upload_data, upload_data_size);
                }
                /* now it is safe to open and process file before finishing request */
                if (strcmp(request_pages[i].url, kBSQDURLElements) == 0) {
                    return bs_qd_request_elements_via_heap(cls, request_pages[i].mime, connection, con_info, upload_data, upload_data_size);
                }
                else if (strcmp(request_pages[i].url, kBSQDURLElementsViaHeap) == 0) {
                    return bs_qd_request_elements_via_heap(cls, request_pages[i].mime, connection, con_info, upload_data, upload_data_size);
                }
                else if (strcmp(request_pages[i].url, kBSQDURLElementsViaTemporaryFile) == 0) {
                    return bs_qd_request_elements_via_temporary_file(cls, request_pages[i].mime, connection, con_info, upload_data, upload_data_size);
                }
            }
        }
    }

    /* unsupported HTTP method */
    response = MHD_create_response_from_buffer(strlen (METHOD_ERROR), (void *) METHOD_ERROR, MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection, MHD_HTTP_NOT_ACCEPTABLE, response);
    MHD_destroy_response(response);

    return ret;
}
#pragma GCC diagnostic pop

/* POST handler */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static int
bs_qd_iterate_elements_post(void *coninfo_cls, enum MHD_ValueKind kind, const char *key, const char *filename, const char *content_type, const char *transfer_encoding, const char *data, uint64_t off, size_t size)
{
    bs_qd_connection_info_t* con_info = (bs_qd_connection_info_t*) coninfo_cls;

    if (strcmp(key, "file") != 0) {
        return MHD_NO;
    }

    if (!con_info->upload_fp) {
        /* generate temporary file */
        char write_fn[] = "/tmp/bs_XXXXXX";
        int write_fd = mkstemp(write_fn);
        con_info->upload_fp = fdopen(write_fd, "ab");
        if (!con_info->upload_fp) {
            return MHD_NO;
        }
        con_info->upload_filename = NULL;
        con_info->upload_filename = malloc(strlen(write_fn) + 1);
        if (!con_info->upload_filename) {
            fprintf(stdout, "Request [%" PRIu64 "]: Upload filename could not be allocated to memory\n", con_info->timestamp);
            return MHD_NO;
        }
        memcpy(con_info->upload_filename, write_fn, strlen(write_fn) + 1);
        fprintf(stdout, "Request [%" PRIu64 "]: Writing query intervals to [%s]\n", con_info->timestamp, con_info->upload_filename);
    }

    if (size > 0) {
        //fprintf(stdout, "Request [%" PRIu64 "]: Writing [%zu] bytes from offset [%" PRIu64 "] to [%s]\n", con_info->timestamp, size, off, con_info->upload_filename);
        if (!fwrite(data, sizeof(char), size, con_info->upload_fp)) {
            return MHD_NO;
        }
        con_info->upload_filesize += size;
    }

    return MHD_YES;
}
#pragma GCC diagnostic pop

/**
 * @brief      bs_qd_debug_kv(cls, kind, key, value)
 *
 * @details    Writes key-value pairs to stderr for specified kind 
 *             of type MHD_ValueKind. Useful for debugging.
 *
 * @param      cls    (void*) data passed along with handler
 *             kind   (MHD_ValueKind) variety of key-value pair (see libmicrohttpd documentation)
 *             key    (char*) key
 *             value  (char*) value for key
 *
 * @return     (int) success or failure
 */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-function"
static int
bs_qd_debug_kv(void* cls, enum MHD_ValueKind kind, const char* key, const char* value)
{
    fprintf(stderr, "key [%s] -> value [%s]\n", key, value);
    return MHD_YES;
}
#pragma GCC diagnostic pop

/**
 * @brief      bs_qd_populate_filter_parameters(cls, kind, key, value)
 *
 * @details    Writes matching key-value pairs to filter parameters
 *             and sets flags for later input validation and 
 *             processing.
 *
 * @param      cls    (void*) data passed along with handler
 *             kind   (MHD_ValueKind) variety of key-value pair (here: MHD_GET_ARGUMENT_KIND)
 *             key    (char*) key
 *             value  (char*) value for key
 *
 * @return     (int) success or failure
 */

static int
bs_qd_populate_filter_parameters(void* cls, enum MHD_ValueKind kind, const char* key, const char* value)
{
    if (kind != MHD_GET_ARGUMENT_KIND) {
        return MHD_NO;
    }
    bs_qd_filter_param_t* params = (bs_qd_filter_param_t*) cls;
    if (key && value) {
        if (strcmp(key, "filter-type") == 0) {
            params->type = kScoreFilterUndefined;
            if (strcmp(value, kScoreFilterGtEqStr) == 0)                        { params->type = kScoreFilterGtEq; }
            else if (strcmp(value, kScoreFilterGtStr) == 0)                     { params->type = kScoreFilterGt; }
            else if (strcmp(value, kScoreFilterEqStr) == 0)                     { params->type = kScoreFilterEq; }
            else if (strcmp(value, kScoreFilterLtEqStr) == 0)                   { params->type = kScoreFilterLtEq; }
            else if (strcmp(value, kScoreFilterLtStr) == 0)                     { params->type = kScoreFilterLt; }
            else if (strcmp(value, kScoreFilterRangedWithinExclusiveStr) == 0)  { params->type = kScoreFilterRangedWithinExclusive; }
            else if (strcmp(value, kScoreFilterRangedWithinInclusiveStr) == 0)  { params->type = kScoreFilterRangedWithinInclusive; }
            else if (strcmp(value, kScoreFilterRangedOutsideExclusiveStr) == 0) { params->type = kScoreFilterRangedOutsideExclusive; }
            else if (strcmp(value, kScoreFilterRangedOutsideInclusiveStr) == 0) { params->type = kScoreFilterRangedOutsideInclusive; }
        }

        else if (strcmp(key, "filter-value") == 0) {
            /* two forms of filter values: "float", or "float:float" */
            char* cptr = NULL;
            cptr = strchr(value, ':');
            if (cptr) {
                /* lower bound to upper bound */
                if (sscanf(value, "%f:%f", &params->lower_bound, &params->upper_bound) != 2) {
                    params->bounds_set = kFalse;
                    return MHD_NO;
                }
                params->bounds_set = kTrue;
            }
            else {
                /* lone bound */
                if (sscanf(value, "%f", &params->lone_bound) != 1) {
                    params->bounds_set = kFalse;
                    return MHD_NO;
                }
                params->bounds_set = kTrue;
            }
        }

        else if (strcmp(key, "padding") == 0) {
            if (sscanf(value, "%d", &params->padding) != 1) {
                params->padding_set = kFalse;
                return MHD_NO;
            }
            params->padding_set = kTrue;
        }

        else if (strcmp(key, "pairing") == 0) {
            params->pairing = kBSQDPairingUndefined;
            if (strcmp(value, kBSQDPairingWholeGenomeStr) == 0) { params->pairing = kBSQDPairingWholeGenome; }
            else if (strcmp(value, kBSQDPairingMutualStr) == 0) { params->pairing = kBSQDPairingMutual; }
            if (params->pairing == kBSQDPairingUndefined) {
                params->pairing_set = kFalse;
            }
            else {
                params->pairing_set = kTrue;
            }
        }

        else if (strcmp(key, "postsort") == 0) {
            params->postsort = kBSQDSortUndefined;
            if (strcmp(value, kBSQDSortOffStr) == 0) { params->postsort = kBSQDSortOff; }
            else if (strcmp(value, kBSQDSortIntervalStr) == 0) { params->postsort = kBSQDSortInterval; }
            else if (strcmp(value, kBSQDSortScoreStr) == 0) { params->postsort = kBSQDSortScore; }
            if (params->postsort == kBSQDSortUndefined) {
                params->postsort_set = kFalse;
                params->postsort = kBSQDSortOff;
            }
            else {
                params->postsort_set = kTrue;
            }
        }
    }
    return MHD_YES;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static int
bs_qd_request_generic_information(const void* cls, const char* mime, struct MHD_Connection* connection, bs_qd_connection_info_t* con_info, const char* upload_data, size_t* upload_data_size)
{
    int ret;
    const char *generic_information = cls;
    char* reply = NULL;
    struct MHD_Response *response;
    reply = malloc(strlen(generic_information) + 1);
    if (!reply) {
        return MHD_NO; /* oops */
    }
    memcpy(reply, generic_information, strlen(generic_information) + 1);

    /* update connection information */
    con_info->request_type = kBSQDRequestGeneric;

    /* return static document */
    response = MHD_create_response_from_buffer(strlen(reply), (void *)reply, MHD_RESPMEM_MUST_FREE);
    MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_ENCODING, mime);
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response (response);

    return ret;
}
#pragma GCC diagnostic pop

/* query daemon - elements */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static int
bs_qd_request_elements_via_heap(const void* cls, const char* mime, struct MHD_Connection* connection, bs_qd_connection_info_t* con_info, const char* upload_data, size_t* upload_data_size)
{
    struct MHD_Response *response;
    int ret = MHD_NO;

    /* read filter parameters from query string, if specified */
    bs_qd_filter_param_t* filter_parameters = malloc(sizeof(*filter_parameters));
    filter_parameters->type = kScoreFilterNone;
    filter_parameters->bounds_set = kFalse;
    filter_parameters->padding_set = kFalse;
    filter_parameters->pairing_set = kFalse;
    filter_parameters->postsort_set = kFalse;
    MHD_get_connection_values(connection, MHD_GET_ARGUMENT_KIND, bs_qd_populate_filter_parameters, filter_parameters);

    /* if parameters were not specified correctly, return appropriate error message */
    if ((filter_parameters->type == kScoreFilterUndefined) || ((filter_parameters->type != kScoreFilterNone) && (!filter_parameters->bounds_set))) {
        free(filter_parameters), filter_parameters = NULL;
        return bs_qd_parameters_not_found(cls, mime, connection, con_info, upload_data, upload_data_size);
    }

    /* if uploaded file is empty, then quit early */
    if (bs_file_size(con_info->upload_filename) <= 2) {
        free(filter_parameters);
        filter_parameters = NULL;
        /* write empty buffer to response */
        response = MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_MUST_FREE);
        MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_ENCODING, mime);
        MHD_add_response_header(response, MHD_HTTP_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN, "*");
        if (!response) {
            return MHD_NO;
        }
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return ret;
    }

    /* process the uploaded file to get the element ranges of interest, and then write to output */
    FILE* range_fp = NULL;
    char cmd[PATH_MAX] = {0};

    /* set up a temporary file for storing query indices */
    char write_fn[] = "/tmp/bs_XXXXXX";
    mkstemp(write_fn);
    con_info->query_index_filename = malloc(strlen(write_fn) + 1);
    if (!con_info->query_index_filename) {
        fprintf(stdout, "Request [%" PRIu64 "]: Query index filename could not be allocated to memory\n", con_info->timestamp);
        return bs_qd_request_malformed(cls, mime, connection, con_info, upload_data, upload_data_size);
    }
    memcpy(con_info->query_index_filename, write_fn, strlen(write_fn) + 1);
    fprintf(stdout, "Request [%" PRIu64 "]: Writing query indices to [%s]\n", con_info->timestamp, con_info->query_index_filename);

    /* write query indices via Shane's query-bytestore script -- possible avenue for later optimization */
    if (filter_parameters->padding_set) {
        sprintf(cmd, "echo >> %s && sed '/^$/d' %s | tr -d '\r' | %s - | %s --range %d --everything - | %s --merge - | %s %s - | awk 'BEGIN {fst=-99; lst=-99} ; { if ( int($4) != lst+1 ) { if ( lst >= 0 ) { print fst\"-\"lst; } fst = int($4); lst = int($4); } else { lst = int($4); } } END { if (lst >= 0) { print fst\"-\"lst; } }' > %s",
                con_info->upload_filename,
                con_info->upload_filename,
                bs_globals.sortbed_path,
                bs_globals.bedops_path,
                filter_parameters->padding,
                bs_globals.bedops_path,
                bs_globals.bedextract_path,
                bs_globals.lookup_fn,
                con_info->query_index_filename);
    }
    else {
        sprintf(cmd, "echo >> %s && sed '/^$/d' %s | tr -d '\r' | %s - | %s --merge - | %s %s - | awk 'BEGIN {fst=-99; lst=-99} ; { if ( int($4) != lst+1 ) { if ( lst >= 0 ) { print fst\"-\"lst; } fst = int($4); lst = int($4); } else { lst = int($4); } } END { if (lst >= 0) { print fst\"-\"lst; } }' > %s",
                con_info->upload_filename,
                con_info->upload_filename,
                bs_globals.sortbed_path,
                bs_globals.bedops_path,
                bs_globals.bedextract_path,
                bs_globals.lookup_fn,
                con_info->query_index_filename);
    }
    //fprintf(stdout, "Debug: cmd [%s]\n", cmd);
    if (NULL == (range_fp = popen(cmd, "r"))) {
       fprintf(stdout, "Error: Could not popen bedextract command to generate query indices [%s]\n", cmd);
       return bs_qd_request_malformed(cls, mime, connection, con_info, upload_data, upload_data_size);
    }
    int status = pclose(range_fp);
    if (status == -1) {
        fprintf(stderr, "Error: pclose() failed!\n");
        return bs_qd_request_malformed(cls, mime, connection, con_info, upload_data, upload_data_size);
    }
    if (bs_file_size(con_info->query_index_filename) == 0) {
        free(filter_parameters);
        filter_parameters = NULL;
        /* write empty buffer to response */
        response = MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_MUST_FREE);
        MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_ENCODING, mime);
        MHD_add_response_header(response, MHD_HTTP_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN, "*");
        if (!response) {
            return MHD_NO;
        }
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return ret;
    }

    /* set up temporary buffer */
    char* temporary_buf = NULL;

    /* write output */
    switch (bs_globals.store_type) {
    case kStorePearsonRSquareMatrixSplit:
    case kStoreSpearmanRhoSquareMatrixSplit:
    case kStoreJaccardIndexSquareMatrixSplit:
    case kStoreOchiaiSimilaritySquareMatrixSplit:
    case kStorePearsonPhiSimilaritySquareMatrixSplit:
    case kStoreRogersAndTanimotoSimilaritySquareMatrixSplit:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplit:
        switch (filter_parameters->type) {
            case kScoreFilterNone:
                bs_print_sqr_split_store_separate_rows_to_bed7_file_via_buffer(bs_globals.lookup_ptr,
                                                                               bs_globals.sqr_store_ptr,
                                                                               con_info->query_index_filename,
                                                                               kFalse,
                                                                               &temporary_buf);
                break;
            case kScoreFilterGtEq:
            case kScoreFilterGt:
            case kScoreFilterEq:
            case kScoreFilterLtEq:
            case kScoreFilterLt:
                bs_print_sqr_filtered_split_store_separate_rows_to_bed7_file_via_buffer(bs_globals.lookup_ptr,
                                                                                        bs_globals.sqr_store_ptr,
                                                                                        con_info->query_index_filename,
                                                                                        kFalse,
                                                                                        &temporary_buf,
                                                                                        filter_parameters->lone_bound,
                                                                                        0,
                                                                                        0,
                                                                                        filter_parameters->type);
                break;
            case kScoreFilterRangedWithinExclusive:
            case kScoreFilterRangedWithinInclusive:
            case kScoreFilterRangedOutsideExclusive:
            case kScoreFilterRangedOutsideInclusive:
                bs_print_sqr_filtered_split_store_separate_rows_to_bed7_file_via_buffer(bs_globals.lookup_ptr,
                                                                                        bs_globals.sqr_store_ptr,
                                                                                        con_info->query_index_filename,
                                                                                        kFalse,
                                                                                        &temporary_buf,
                                                                                        0,
                                                                                        filter_parameters->lower_bound,
                                                                                        filter_parameters->upper_bound,
                                                                                        filter_parameters->type);
                break;
            case kScoreFilterUndefined:
                fprintf(stderr, "Error: You should never see this error (qd_A)\n");
                return MHD_NO;
            default:
                break;
        }
        break;
    case kStoreRandomBufferedSquareMatrix:
    case kStoreRandomSquareMatrix:
    case kStorePearsonRSquareMatrix:
    case kStorePearsonRSquareMatrixBzip2:
    case kStorePearsonRSquareMatrixBzip2Split:
    case kStorePearsonRSquareMatrixSplitSingleChunk:
    case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
    case kStoreSpearmanRhoSquareMatrix:
    case kStoreSpearmanRhoSquareMatrixBzip2:
    case kStoreSpearmanRhoSquareMatrixBzip2Split:
    case kStoreSpearmanRhoSquareMatrixSplitSingleChunk:
    case kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadata:
    case kStoreJaccardIndexSquareMatrix:
    case kStoreJaccardIndexSquareMatrixSplitSingleChunk:
    case kStoreJaccardIndexSquareMatrixSplitSingleChunkMetadata:
    case kStoreOchiaiSimilaritySquareMatrix:
    case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunk:
    case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunkMetadata:
    case kStorePearsonPhiSimilaritySquareMatrix:
    case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunk:
    case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunkMetadata:
    case kStoreRogersAndTanimotoSimilaritySquareMatrix:
    case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunk:
    case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunkMetadata:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrix:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunk:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunkMetadata:
    case kStorePearsonRSUT:
    case kStoreRandomSUT:
    case kStoreUndefined:
        /* no data found for specified store type */
        return bs_qd_request_not_found(cls, mime, connection, con_info, upload_data, upload_data_size);
    }

    /* clean up parameters */
    free(filter_parameters);
    filter_parameters = NULL;

    /* write temporary buffer to response */
    response = MHD_create_response_from_buffer(strlen(temporary_buf), temporary_buf, MHD_RESPMEM_MUST_FREE);
    MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_ENCODING, mime);
    MHD_add_response_header(response, MHD_HTTP_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN, "*");
    if (!response) {
        free(temporary_buf);
        return MHD_NO;
    }
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static int
bs_qd_request_elements_via_temporary_file(const void* cls, const char* mime, struct MHD_Connection* connection, bs_qd_connection_info_t* con_info, const char* upload_data, size_t* upload_data_size)
{
    struct MHD_Response *response;
    int ret = MHD_NO;

    /* read filter parameters from query string, if specified */
    bs_qd_filter_param_t* filter_parameters = malloc(sizeof(*filter_parameters));
    filter_parameters->type = kScoreFilterNone;
    filter_parameters->bounds_set = kFalse;
    filter_parameters->padding_set = kFalse;
    filter_parameters->pairing_set = kFalse;
    filter_parameters->postsort_set = kFalse;
    MHD_get_connection_values(connection, MHD_GET_ARGUMENT_KIND, bs_qd_populate_filter_parameters, filter_parameters);

    /* if parameters were not specified correctly, return appropriate error message */
    if ((filter_parameters->type == kScoreFilterUndefined) || ((filter_parameters->type != kScoreFilterNone) && (!filter_parameters->bounds_set))) {
        free(filter_parameters), filter_parameters = NULL;
        return bs_qd_parameters_not_found(cls, mime, connection, con_info, upload_data, upload_data_size);
    }

    /* if uploaded file is empty, then quit early */
    if (bs_file_size(con_info->upload_filename) <= 2) {
        free(filter_parameters);
        filter_parameters = NULL;
        /* write empty buffer to response */
        response = MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_MUST_FREE);
        MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_ENCODING, mime);
        MHD_add_response_header(response, MHD_HTTP_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN, "*");
        if (!response) {
            return MHD_NO;
        }
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return ret;
    }

    /* process the uploaded file to get the element ranges of interest, and then write to output */
    FILE* range_fp = NULL;
    char cmd[PATH_MAX] = {0};

    /* set up a temporary file for storing query indices */
    char indices_fn[] = "/tmp/bs_XXXXXX";
    mkstemp(indices_fn);
    con_info->query_index_filename = malloc(strlen(indices_fn) + 1);
    if (!con_info->query_index_filename) {
        fprintf(stdout, "Request [%" PRIu64 "]: Query index filename could not be allocated to memory\n", con_info->timestamp);
        return bs_qd_request_malformed(cls, mime, connection, con_info, upload_data, upload_data_size);
    }
    memcpy(con_info->query_index_filename, indices_fn, strlen(indices_fn) + 1);
    fprintf(stdout, "Request [%" PRIu64 "]: Writing query indices to [%s]\n", con_info->timestamp, con_info->query_index_filename);

    /* write query indices via Shane's query-bytestore script -- possible avenue for later optimization */
    if (filter_parameters->padding_set) {
        sprintf(cmd, "echo >> %s && sed '/^$/d' %s | tr -d '\r' | %s - | %s --range %d --everything - | %s --merge - | %s %s - | awk 'BEGIN {fst=-99; lst=-99} ; { if ( int($4) != lst+1 ) { if ( lst >= 0 ) { print fst\"-\"lst; } fst = int($4); lst = int($4); } else { lst = int($4); } } END { if (lst >= 0) { print fst\"-\"lst; } }' > %s",
                con_info->upload_filename,
                con_info->upload_filename,
                bs_globals.sortbed_path,
                bs_globals.bedops_path,
                filter_parameters->padding,
                bs_globals.bedops_path,
                bs_globals.bedextract_path,
                bs_globals.lookup_fn,
                con_info->query_index_filename);
    }
    else {
        sprintf(cmd, "echo >> %s && sed '/^$/d' %s | tr -d '\r' | %s - | %s --merge - | %s %s - | awk 'BEGIN {fst=-99; lst=-99} ; { if ( int($4) != lst+1 ) { if ( lst >= 0 ) { print fst\"-\"lst; } fst = int($4); lst = int($4); } else { lst = int($4); } } END { if (lst >= 0) { print fst\"-\"lst; } }' > %s",
                con_info->upload_filename,
                con_info->upload_filename,
                bs_globals.sortbed_path,
                bs_globals.bedops_path,
                bs_globals.bedextract_path,
                bs_globals.lookup_fn,
                con_info->query_index_filename);
    }
    //fprintf(stdout, "Debug: cmd [%s]\n", cmd);
    if (NULL == (range_fp = popen(cmd, "r"))) {
       fprintf(stdout, "Error: Could not popen bedextract command to generate query indices [%s]\n", cmd);
       return bs_qd_request_malformed(cls, mime, connection, con_info, upload_data, upload_data_size);
    }
    int status = pclose(range_fp);
    if (status == -1) {
        fprintf(stderr, "Error: pclose() failed!\n");
        return bs_qd_request_malformed(cls, mime, connection, con_info, upload_data, upload_data_size);
    }
    if (bs_file_size(con_info->query_index_filename) == 0) {
        free(filter_parameters);
        filter_parameters = NULL;
        /* write empty buffer to response */
        response = MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_MUST_FREE);
        MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_ENCODING, mime);
        MHD_add_response_header(response, MHD_HTTP_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN, "*");
        if (!response) {
            return MHD_NO;
        }
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return ret;
    }

    /* create temporary file and write a random element to it */
    char write_fn[] = "/tmp/bs_XXXXXX";
    int write_fd = mkstemp(write_fn);
    FILE* write_fp = fdopen(write_fd, "w");
    fprintf(stderr, "Request [%" PRIu64 "]: Writing elements to temporary file [%s]\n", con_info->timestamp, write_fn);

    /* write output */
    switch (bs_globals.store_type) {
    case kStorePearsonRSquareMatrixSplit:
    case kStoreSpearmanRhoSquareMatrixSplit:
    case kStoreJaccardIndexSquareMatrixSplit:
    case kStoreOchiaiSimilaritySquareMatrixSplit:
    case kStorePearsonPhiSimilaritySquareMatrixSplit:
    case kStoreRogersAndTanimotoSimilaritySquareMatrixSplit:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplit:
        switch (filter_parameters->type) {
            case kScoreFilterNone:
                bs_print_sqr_split_store_separate_rows_to_bed7_file(bs_globals.lookup_ptr,
                                                                    bs_globals.sqr_store_ptr,
                                                                    con_info->query_index_filename,
                                                                    kFalse,
                                                                    write_fp);
                break;
            case kScoreFilterGtEq:
            case kScoreFilterGt:
            case kScoreFilterEq:
            case kScoreFilterLtEq:
            case kScoreFilterLt:
                bs_print_sqr_filtered_split_store_separate_rows_to_bed7_file(bs_globals.lookup_ptr,
                                                                             bs_globals.sqr_store_ptr,
                                                                             con_info->query_index_filename,
                                                                             kFalse,
                                                                             write_fp,
                                                                             filter_parameters->lone_bound,
                                                                             0,
                                                                             0,
                                                                             filter_parameters->type);
                break;
            case kScoreFilterRangedWithinExclusive:
            case kScoreFilterRangedWithinInclusive:
            case kScoreFilterRangedOutsideExclusive:
            case kScoreFilterRangedOutsideInclusive:
                bs_print_sqr_filtered_split_store_separate_rows_to_bed7_file(bs_globals.lookup_ptr,
                                                                             bs_globals.sqr_store_ptr,
                                                                             con_info->query_index_filename,
                                                                             kFalse,
                                                                             write_fp,
                                                                             0,
                                                                             filter_parameters->lower_bound,
                                                                             filter_parameters->upper_bound,
                                                                             filter_parameters->type);
                break;
            case kScoreFilterUndefined:
                fprintf(stderr, "Error: You should never see this error (qd_B)\n");
                return MHD_NO;
            default:
                break;
        }
        break;
    case kStoreRandomBufferedSquareMatrix:
    case kStoreRandomSquareMatrix:
    case kStorePearsonRSquareMatrix:
    case kStorePearsonRSquareMatrixBzip2:
    case kStorePearsonRSquareMatrixBzip2Split:
    case kStorePearsonRSquareMatrixSplitSingleChunk:
    case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
    case kStoreSpearmanRhoSquareMatrix:
    case kStoreSpearmanRhoSquareMatrixBzip2:
    case kStoreSpearmanRhoSquareMatrixBzip2Split:
    case kStoreSpearmanRhoSquareMatrixSplitSingleChunk:
    case kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadata:
    case kStoreJaccardIndexSquareMatrix:
    case kStoreJaccardIndexSquareMatrixSplitSingleChunk:
    case kStoreJaccardIndexSquareMatrixSplitSingleChunkMetadata:
    case kStoreOchiaiSimilaritySquareMatrix:
    case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunk:
    case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunkMetadata:
    case kStorePearsonPhiSimilaritySquareMatrix:
    case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunk:
    case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunkMetadata:
    case kStoreRogersAndTanimotoSimilaritySquareMatrix:
    case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunk:
    case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunkMetadata:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrix:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunk:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunkMetadata:
    case kStorePearsonRSUT:
    case kStoreRandomSUT:
    case kStoreUndefined:
        /* cleanup */
        fclose(write_fp), write_fp = NULL;
        unlink(write_fn);
        /* no data found for specified store type */
        return bs_qd_request_not_found(cls, mime, connection, con_info, upload_data, upload_data_size);
    }

    /* close write_fn pointer */
    fclose(write_fp), write_fp = NULL;

    if (filter_parameters->pairing_set && filter_parameters->pairing == kBSQDPairingMutual) {
        /* 
            If pairing type is mutual, we want any elements from the result, where the 
            first interval ("A1") has a paired interval ("B1"), which in turn overlaps 
            any other first interval ("An").

            One way to do this is to get the paired intervals and run a bedmap statement 
            to map the original intervals against the paired intervals. Where there are
            exact overlaps, we know there is at least one pairing in the original set 
            that is mutual.

            We apply a final sort as the cost of doing this is likely minimal, at this 
            juncture.

            $ awk 'BEGIN{ OFS="\t"; }{ print $4,$5,$6,$1,$2,$3,$7 }' original.bed | sort-bed - | bedops --everything original.bed - | sort-bed - | uniq -d > mutualPairs.bed
        */
        FILE* mutual_fp = NULL;
        char mutual_fn[] = "/tmp/bs_XXXXXX";
        mkstemp(mutual_fn);
        fprintf(stderr, "Request [%" PRIu64 "]: Writing mutual elements to temporary file [%s]\n", con_info->timestamp, mutual_fn);
        sprintf(cmd, "awk 'BEGIN { OFS=\"\t\"; } { print $4, $5, $6, $1, $2, $3, $7; }' %s | %s - | %s --everything %s - | %s - | uniq -d > %s",
            write_fn,
            bs_globals.sortbed_path,
            bs_globals.bedops_path,
            write_fn,
            bs_globals.sortbed_path,
            mutual_fn);
        if (NULL == (mutual_fp = popen(cmd, "r"))) {
           fprintf(stdout, "Error: Could not popen bedops command to generate mutual items [%s]\n", cmd);
           return bs_qd_request_malformed(cls, mime, connection, con_info, upload_data, upload_data_size);
        }
        int status = pclose(mutual_fp);
        if (status == -1) {
            fprintf(stderr, "Error: pclose() failed!\n");
            return bs_qd_request_malformed(cls, mime, connection, con_info, upload_data, upload_data_size);
        }
        /* copy the mutual_fn to the write_fn and delete/unlink mutual_fn */
        /* by using filename tricks, we can eliminate a file copy operation and just copy by way of moving filenames */
        char old_write_fn[] = "/tmp/bs_XXXXXX";
        memcpy(old_write_fn, write_fn, strlen(write_fn) + 1);
        memcpy(write_fn, mutual_fn, strlen(mutual_fn) + 1);
        unlink(old_write_fn);
        fprintf(stderr, "Request [%" PRIu64 "]: Deleted temporary file [%s]\n", con_info->timestamp, old_write_fn);
    }

    /* apply postsort, if specified */
    if (filter_parameters->postsort_set && (filter_parameters->postsort != kBSQDSortOff)) {
        FILE* postsort_fp = NULL;
        char postsort_fn[] = "/tmp/bs_XXXXXX";
        mkstemp(postsort_fn);
        fprintf(stderr, "Request [%" PRIu64 "]: Writing sorted elements to temporary file [%s]\n", con_info->timestamp, postsort_fn);
        if (filter_parameters->postsort == kBSQDSortInterval) {
            sprintf(cmd, "%s %s > %s",
                    bs_globals.sortbed_path,
                    write_fn,
                    postsort_fn);
        }
        else if (filter_parameters->postsort == kBSQDSortScore) {
            sprintf(cmd, "sort -k7 -nr %s > %s",
                    write_fn,
                    postsort_fn);
        }
        if (NULL == (postsort_fp = popen(cmd, "r"))) {
           fprintf(stdout, "Error: Could not popen sorting command to generate sorted items [%s]\n", cmd);
           return bs_qd_request_malformed(cls, mime, connection, con_info, upload_data, upload_data_size);
        }
        int status = pclose(postsort_fp);
        if (status == -1) {
            fprintf(stderr, "Error: pclose() failed!\n");
            return bs_qd_request_malformed(cls, mime, connection, con_info, upload_data, upload_data_size);
        }
        char old_write_fn[] = "/tmp/bs_XXXXXX";
        memcpy(old_write_fn, write_fn, strlen(write_fn) + 1);
        memcpy(write_fn, postsort_fn, strlen(postsort_fn) + 1);
        unlink(old_write_fn);
        fprintf(stderr, "Request [%" PRIu64 "]: Deleted temporary file [%s]\n", con_info->timestamp, old_write_fn);
    }

    /* clean up parameters */
    free(filter_parameters), filter_parameters = NULL;

    /* read from temporary file, or whatever file results from any post-processing */
    fprintf(stderr, "Request [%" PRIu64 "]: Will try to read elements from temporary file [%s]\n", con_info->timestamp, write_fn);
    FILE* read_fp = NULL;
    read_fp = fopen(write_fn, "r");
    int read_fd = fileno(read_fp);
    if (read_fd == -1) {
        (void) fclose(read_fp);
        return MHD_NO; /* internal error */
    }
    struct stat read_buf;
    if ((fstat(read_fd, &read_buf) != 0) || (!S_ISREG(read_buf.st_mode))) {
        /* not a regular file, cleanup */
        fclose(read_fp), read_fp = NULL;
        fclose(write_fp), write_fp = NULL;
        unlink(write_fn);
        fprintf(stderr, "Request [%" PRIu64 "]: Deleted temporary file [%s]\n", con_info->timestamp, write_fn);
    }
    if (!read_fp) {
        return bs_qd_request_not_found(cls, mime, connection, con_info, upload_data, upload_data_size);
    }
    else {
        long sz = sysconf(_SC_PAGESIZE);
        bs_qd_io_t* io = NULL;
        io = malloc(sizeof(bs_qd_io_t));
        io->con_info = con_info;
        io->write_fn = NULL;
        io->write_fn = malloc(strlen(write_fn) + 1);
        if (!io->write_fn) {
            fclose(read_fp);
            return MHD_NO;
        }
        memcpy(io->write_fn, write_fn, strlen(write_fn) + 1);
        io->write_fp = write_fp;
        io->read_fp = read_fp;
        response = MHD_create_response_from_callback(read_buf.st_size, (size_t) sz, &bs_qd_temporary_file_buffer_reader, io, &bs_qd_temporary_file_buffer_callback);
        MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_ENCODING, mime);
        MHD_add_response_header(response, MHD_HTTP_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN, "*");
        if (!response) {
            fclose(read_fp);
            return MHD_NO;
        }
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
    }
    return ret;
}
#pragma GCC diagnostic pop

/* query daemon - random element */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static int
bs_qd_request_random_element_via_temporary_file(const void* cls, const char* mime, struct MHD_Connection* connection, bs_qd_connection_info_t* con_info, const char* upload_data, size_t* upload_data_size)
{
    int ret;
    struct MHD_Response* response = NULL;

    /* update connection information */
    con_info->request_type = kBSQDRequestRandomViaTemporaryFile;

    /* start */
    fprintf(stdout, "Request [%" PRIu64 "]: [%s] => [%s] started\n", con_info->timestamp, bs_qd_connection_method_type_to_str(con_info->method), bs_qd_request_type_to_str(con_info->request_type));

    /* read filter parameters from query string, if specified */
    bs_qd_filter_param_t* filter_parameters = malloc(sizeof(*filter_parameters));
    filter_parameters->type = kScoreFilterNone;
    filter_parameters->bounds_set = kFalse;
    filter_parameters->padding_set = kFalse;
    filter_parameters->pairing_set = kFalse;
    filter_parameters->postsort_set = kFalse;
    MHD_get_connection_values(connection, MHD_GET_ARGUMENT_KIND, bs_qd_populate_filter_parameters, filter_parameters);

    /* if parameters were not specified correctly, return appropriate error message */
    if ((filter_parameters->type == kScoreFilterUndefined) || ((filter_parameters->type != kScoreFilterNone) && (!filter_parameters->bounds_set))) {
        free(filter_parameters), filter_parameters = NULL;
        return bs_qd_parameters_not_found(cls, mime, connection, con_info, upload_data, upload_data_size);
    }

    /* create temporary file and write a random element to it */
    char write_fn[] = "/tmp/bs_XXXXXX";
    int write_fd = mkstemp(write_fn);
    FILE* write_fp = fdopen(write_fd, "w");
    fprintf(stderr, "Request [%" PRIu64 "]: Writing random element to temporary file [%s]\n", con_info->timestamp, write_fn);

    /* seed RNG */
    if (bs_globals.rng_seed_flag)
        mt19937_seed_rng(bs_globals.rng_seed_value);
    else
        mt19937_seed_rng(time(NULL));

    /* generate random row index within bounds */
    size_t query_idx_start = (size_t) (mt19937_generate_random_ulong() % bs_globals.lookup_ptr->nelems);

    /* write index range to query row array */
    size_t* query_roi = NULL;
    size_t query_roi_num = 1; /* for now, this value is 1 */
    query_roi = malloc(query_roi_num * sizeof(*query_roi));
    if (!query_roi) {
        fprintf(stdout, "Request [%" PRIu64 "]: Could not allocate memory for query ROI array\n", con_info->timestamp);
        return bs_qd_request_malformed(cls, mime, connection, con_info, upload_data, upload_data_size);
    }
    query_roi[0] = query_idx_start;

    /* write a random row to the temporary file */
    switch (bs_globals.store_type) {
    case kStorePearsonRSquareMatrixSplit:
    case kStoreSpearmanRhoSquareMatrixSplit:
    case kStoreJaccardIndexSquareMatrixSplit:
    case kStoreOchiaiSimilaritySquareMatrixSplit:
    case kStorePearsonPhiSimilaritySquareMatrixSplit:
    case kStoreRogersAndTanimotoSimilaritySquareMatrixSplit:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplit:
        switch (filter_parameters->type) {
            case kScoreFilterNone:
                bs_print_sqr_split_store_separate_rows_to_bed7(bs_globals.lookup_ptr,
                                                               bs_globals.sqr_store_ptr,
                                                               kFalse,
                                                               write_fp,
                                                               query_roi,
                                                               query_roi,
                                                               query_roi_num);
                break;
            case kScoreFilterGtEq:
            case kScoreFilterGt:
            case kScoreFilterEq:
            case kScoreFilterLtEq:
            case kScoreFilterLt:
                bs_print_sqr_filtered_split_store_separate_rows_to_bed7(bs_globals.lookup_ptr,
                                                                        bs_globals.sqr_store_ptr,
                                                                        write_fp,
                                                                        query_roi,
                                                                        query_roi,
                                                                        query_roi_num,
                                                                        filter_parameters->lone_bound,
                                                                        0,
                                                                        0,
                                                                        filter_parameters->type);
                break;
            case kScoreFilterRangedWithinExclusive:
            case kScoreFilterRangedWithinInclusive:
            case kScoreFilterRangedOutsideExclusive:
            case kScoreFilterRangedOutsideInclusive:
                bs_print_sqr_filtered_split_store_separate_rows_to_bed7(bs_globals.lookup_ptr,
                                                                        bs_globals.sqr_store_ptr,
                                                                        write_fp,
                                                                        query_roi,
                                                                        query_roi,
                                                                        query_roi_num,
                                                                        0,
                                                                        filter_parameters->lower_bound,
                                                                        filter_parameters->upper_bound,
                                                                        filter_parameters->type);
                break;
            case kScoreFilterUndefined:
                fprintf(stderr, "Error: You should never see this error (qd_A)\n");
                return MHD_NO;
            default:
                break;
        }
        break;
    case kStoreRandomBufferedSquareMatrix:
    case kStoreRandomSquareMatrix:
    case kStorePearsonRSquareMatrix:
    case kStorePearsonRSquareMatrixBzip2:
    case kStorePearsonRSquareMatrixBzip2Split:
    case kStorePearsonRSquareMatrixSplitSingleChunk:
    case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
    case kStoreSpearmanRhoSquareMatrix:
    case kStoreSpearmanRhoSquareMatrixBzip2:
    case kStoreSpearmanRhoSquareMatrixBzip2Split:
    case kStoreSpearmanRhoSquareMatrixSplitSingleChunk:
    case kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadata:
    case kStoreJaccardIndexSquareMatrix:
    case kStoreJaccardIndexSquareMatrixSplitSingleChunk:
    case kStoreJaccardIndexSquareMatrixSplitSingleChunkMetadata:
    case kStoreOchiaiSimilaritySquareMatrix:
    case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunk:
    case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunkMetadata:
    case kStorePearsonPhiSimilaritySquareMatrix:
    case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunk:
    case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunkMetadata:
    case kStoreRogersAndTanimotoSimilaritySquareMatrix:
    case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunk:
    case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunkMetadata:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrix:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunk:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunkMetadata:
    case kStorePearsonRSUT:
    case kStoreRandomSUT:
    case kStoreUndefined:
        /* cleanup */
        fclose(write_fp), write_fp = NULL;
        unlink(write_fn);
        /* no data found for specified store type */
        return bs_qd_request_not_found(cls, mime, connection, con_info, upload_data, upload_data_size);
    }

    /* cleanup */
    fclose(write_fp), write_fp = NULL;

    if (filter_parameters->pairing_set && filter_parameters->pairing == kBSQDPairingMutual) {
        /* 
            If pairing type is mutual, we want any elements from the result, where the 
            first interval ("A1") has a paired interval ("B1"), which in turn overlaps 
            any other first interval ("An").

            One way to do this is to get the paired intervals and run a bedmap statement 
            to map the original intervals against the paired intervals. Where there are
            exact overlaps, we know there is at least one pairing in the original set 
            that is mutual.

            We apply a final sort as the cost of doing this is likely minimal, at this 
            juncture.

            $ awk 'BEGIN{ OFS="\t"; }{ print $4,$5,$6,$1,$2,$3,$7 }' original.bed | sort-bed - | bedops --everything original.bed - | sort-bed - | uniq -d > mutualPairs.bed
        */
        char cmd[PATH_MAX] = {0};
        FILE* mutual_fp = NULL;
        char mutual_fn[] = "/tmp/bs_XXXXXX";
        mkstemp(mutual_fn);
        fprintf(stderr, "Request [%" PRIu64 "]: Writing mutual elements to temporary file [%s]\n", con_info->timestamp, mutual_fn);
        sprintf(cmd, "awk 'BEGIN { OFS=\"\t\"; } { print $4, $5, $6, $1, $2, $3, $7; }' %s | %s - | %s --everything %s - | %s - | uniq -d > %s",
            write_fn,
            bs_globals.sortbed_path,
            bs_globals.bedops_path,
            write_fn,
            bs_globals.sortbed_path,
            mutual_fn);
        if (NULL == (mutual_fp = popen(cmd, "r"))) {
           fprintf(stdout, "Error: Could not popen bedops command to generate mutual items [%s]\n", cmd);
           return bs_qd_request_malformed(cls, mime, connection, con_info, upload_data, upload_data_size);
        }
        int status = pclose(mutual_fp);
        if (status == -1) {
            fprintf(stderr, "Error: pclose() failed!\n");
            return bs_qd_request_malformed(cls, mime, connection, con_info, upload_data, upload_data_size);
        }
        /* copy the mutual_fn to the write_fn and delete/unlink mutual_fn */
        /* by using filename tricks, we can eliminate a file copy operation and just copy by way of moving filenames */
        char old_write_fn[] = "/tmp/bs_XXXXXX";
        memcpy(old_write_fn, write_fn, strlen(write_fn) + 1);
        memcpy(write_fn, mutual_fn, strlen(mutual_fn) + 1);
        unlink(old_write_fn);
        fprintf(stderr, "Request [%" PRIu64 "]: Deleted temporary file [%s]\n", con_info->timestamp, old_write_fn);
    }

    /* apply postsort, if specified */
    if (filter_parameters->postsort_set  && (filter_parameters->postsort != kBSQDSortOff)) {
        char cmd[PATH_MAX] = {0};
        FILE* postsort_fp = NULL;
        char postsort_fn[] = "/tmp/bs_XXXXXX";
        mkstemp(postsort_fn);
        fprintf(stderr, "Request [%" PRIu64 "]: Writing sorted elements to temporary file [%s]\n", con_info->timestamp, postsort_fn);
        if (filter_parameters->postsort == kBSQDSortInterval) {
            sprintf(cmd, "%s %s > %s",
                    bs_globals.sortbed_path,
                    write_fn,
                    postsort_fn);
        }
        else if (filter_parameters->postsort == kBSQDSortScore) {
            sprintf(cmd, "sort -k7 -nr %s > %s",
                    write_fn,
                    postsort_fn);
        }
        if (NULL == (postsort_fp = popen(cmd, "r"))) {
           fprintf(stdout, "Error: Could not popen sorting command to generate sorted items [%s]\n", cmd);
           return bs_qd_request_malformed(cls, mime, connection, con_info, upload_data, upload_data_size);
        }
        int status = pclose(postsort_fp);
        if (status == -1) {
            fprintf(stderr, "Error: pclose() failed!\n");
            return bs_qd_request_malformed(cls, mime, connection, con_info, upload_data, upload_data_size);
        }
        char old_write_fn[] = "/tmp/bs_XXXXXX";
        memcpy(old_write_fn, write_fn, strlen(write_fn) + 1);
        memcpy(write_fn, postsort_fn, strlen(postsort_fn) + 1);
        unlink(old_write_fn);
        fprintf(stderr, "Request [%" PRIu64 "]: Deleted temporary file [%s]\n", con_info->timestamp, old_write_fn);
    }

    /* clean up parameters */
    free(filter_parameters), filter_parameters = NULL;

    /* read from temporary file */
    FILE* read_fp = NULL;
    read_fp = fopen(write_fn, "r");
    int read_fd = fileno(read_fp);
    if (read_fd == -1) {
        (void) fclose(read_fp);
        return MHD_NO; /* internal error */
    }
    struct stat read_buf;
    if ((fstat(read_fd, &read_buf) != 0) || (!S_ISREG(read_buf.st_mode))) {
        /* not a regular file, cleanup */
        fclose(read_fp), read_fp = NULL;
        fclose(write_fp), write_fp = NULL;
        unlink(write_fn);
    }
    if (!read_fp) {
        return bs_qd_request_not_found(cls, mime, connection, con_info, upload_data, upload_data_size);
    }
    else {
        long sz = sysconf(_SC_PAGESIZE);
        bs_qd_io_t* io = NULL;
        io = malloc(sizeof(bs_qd_io_t));
        io->con_info = con_info;
        io->write_fn = NULL;
        io->write_fn = malloc(strlen(write_fn) + 1);
        if (!io->write_fn) {
            fclose(read_fp);
            return MHD_NO;
        }
        memcpy(io->write_fn, write_fn, strlen(write_fn) + 1);
        io->write_fp = write_fp;
        io->read_fp = read_fp;
        response = MHD_create_response_from_callback(read_buf.st_size, (size_t) sz, &bs_qd_temporary_file_buffer_reader, io, &bs_qd_temporary_file_buffer_callback);
        MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_ENCODING, mime);
        MHD_add_response_header(response, MHD_HTTP_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN, "*");
        if (!response) {
            fclose(read_fp);
            return MHD_NO;
        }
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
    }
    return ret;
}
#pragma GCC diagnostic pop

/* via buffer */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static int
bs_qd_request_random_element_via_heap(const void* cls, const char* mime, struct MHD_Connection* connection, bs_qd_connection_info_t* con_info, const char* upload_data, size_t* upload_data_size)
{
    int ret;
    struct MHD_Response* response = NULL;

    /* update connection information */
    con_info->request_type = kBSQDRequestRandomViaHeap;

    /* start */
    fprintf(stdout, "Request [%" PRIu64 "]: [%s] => [%s] started\n", con_info->timestamp, bs_qd_connection_method_type_to_str(con_info->method), bs_qd_request_type_to_str(con_info->request_type));

    /* read filter parameters from query string, if specified */
    bs_qd_filter_param_t* filter_parameters = malloc(sizeof(*filter_parameters));
    filter_parameters->type = kScoreFilterNone;
    filter_parameters->bounds_set = kFalse;
    filter_parameters->padding_set = kFalse;
    filter_parameters->pairing_set = kFalse;
    filter_parameters->postsort_set = kFalse;
    MHD_get_connection_values(connection, MHD_GET_ARGUMENT_KIND, bs_qd_populate_filter_parameters, filter_parameters);

    /* if parameters were not specified correctly, return appropriate error message */
    if ((filter_parameters->type == kScoreFilterUndefined) || ((filter_parameters->type != kScoreFilterNone) && (!filter_parameters->bounds_set))) {
        free(filter_parameters), filter_parameters = NULL;
        return bs_qd_parameters_not_found(cls, mime, connection, con_info, upload_data, upload_data_size);
    }

    /* seed RNG */
    if (bs_globals.rng_seed_flag)
        mt19937_seed_rng(bs_globals.rng_seed_value);
    else
        mt19937_seed_rng(time(NULL));

    /* generate random row index within bounds */
    size_t query_idx_start = (size_t) (mt19937_generate_random_ulong() % bs_globals.lookup_ptr->nelems);
    /* uint32_t query_idx_end = query_idx_start; */

    /* write index range to query row array */
    size_t* query_roi = NULL;
    size_t query_roi_num = 1; /* for now, this value is 1 */
    query_roi = malloc(query_roi_num * sizeof(*query_roi));
    if (!query_roi) {
        fprintf(stdout, "Request [%" PRIu64 "]: Could not allocate memory for query ROI array\n", con_info->timestamp);
        return bs_qd_request_malformed(cls, mime, connection, con_info, upload_data, upload_data_size);        
    }
    query_roi[0] = query_idx_start;

    /* set up temporary buffer */
    char* temporary_buf = NULL;

    /* write a random row to a temporary buffer */
    switch (bs_globals.store_type) {
    case kStorePearsonRSquareMatrixSplit:
    case kStoreSpearmanRhoSquareMatrixSplit:
    case kStoreJaccardIndexSquareMatrixSplit:
    case kStoreOchiaiSimilaritySquareMatrixSplit:
    case kStorePearsonPhiSimilaritySquareMatrixSplit:
    case kStoreRogersAndTanimotoSimilaritySquareMatrixSplit:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplit:
        switch (filter_parameters->type) {
            case kScoreFilterNone:
                bs_print_sqr_split_store_separate_rows_to_bed7_via_buffer(bs_globals.lookup_ptr,
                                                                          bs_globals.sqr_store_ptr,
                                                                          kFalse,
                                                                          &temporary_buf,
                                                                          query_roi,
                                                                          query_roi,
                                                                          query_roi_num);
                break;
            case kScoreFilterGtEq:
            case kScoreFilterGt:
            case kScoreFilterEq:
            case kScoreFilterLtEq:
            case kScoreFilterLt:
                bs_print_sqr_filtered_split_store_separate_rows_to_bed7_via_buffer(bs_globals.lookup_ptr,
                                                                                   bs_globals.sqr_store_ptr,
                                                                                   &temporary_buf,
                                                                                   query_roi,
                                                                                   query_roi,
                                                                                   query_roi_num,
                                                                                   filter_parameters->lone_bound,
                                                                                   0,
                                                                                   0,
                                                                                   filter_parameters->type);
                break;
            case kScoreFilterRangedWithinExclusive:
            case kScoreFilterRangedWithinInclusive:
            case kScoreFilterRangedOutsideExclusive:
            case kScoreFilterRangedOutsideInclusive:
                bs_print_sqr_filtered_split_store_separate_rows_to_bed7_via_buffer(bs_globals.lookup_ptr,
                                                                                   bs_globals.sqr_store_ptr,
                                                                                   &temporary_buf,
                                                                                   query_roi,
                                                                                   query_roi,
                                                                                   query_roi_num,
                                                                                   0,
                                                                                   filter_parameters->lower_bound,
                                                                                   filter_parameters->upper_bound,
                                                                                   filter_parameters->type);
                break;
            case kScoreFilterUndefined:
                free(query_roi);
                query_roi = NULL;
                fprintf(stderr, "Error: You should never see this error (qd_A)\n");
                return MHD_NO;
            default:
                break;
        }
        break;
    case kStoreRandomBufferedSquareMatrix:
    case kStoreRandomSquareMatrix:
    case kStorePearsonRSquareMatrix:
    case kStorePearsonRSquareMatrixBzip2:
    case kStorePearsonRSquareMatrixBzip2Split:
    case kStorePearsonRSquareMatrixSplitSingleChunk:
    case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
    case kStoreSpearmanRhoSquareMatrix:
    case kStoreSpearmanRhoSquareMatrixBzip2:
    case kStoreSpearmanRhoSquareMatrixBzip2Split:
    case kStoreSpearmanRhoSquareMatrixSplitSingleChunk:
    case kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadata:
    case kStoreJaccardIndexSquareMatrix:
    case kStoreJaccardIndexSquareMatrixSplitSingleChunk:
    case kStoreJaccardIndexSquareMatrixSplitSingleChunkMetadata:
    case kStoreOchiaiSimilaritySquareMatrix:
    case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunk:
    case kStoreOchiaiSimilaritySquareMatrixSplitSingleChunkMetadata:
    case kStorePearsonPhiSimilaritySquareMatrix:
    case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunk:
    case kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunkMetadata:
    case kStoreRogersAndTanimotoSimilaritySquareMatrix:
    case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunk:
    case kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunkMetadata:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrix:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunk:
    case kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunkMetadata:
    case kStorePearsonRSUT:
    case kStoreRandomSUT:
    case kStoreUndefined:
        /* no data found for specified store type */
        free(query_roi);
        query_roi = NULL;
        free(filter_parameters);
        filter_parameters = NULL;
        return bs_qd_request_not_found(cls, mime, connection, con_info, upload_data, upload_data_size);
    }

    /* clean up parameters */
    free(query_roi);
    query_roi = NULL;
    free(filter_parameters);
    filter_parameters = NULL;

    /* write temporary buffer to response */
    response = MHD_create_response_from_buffer(strlen(temporary_buf), temporary_buf, MHD_RESPMEM_MUST_FREE);
    MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_ENCODING, mime);
    MHD_add_response_header(response, MHD_HTTP_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN, "*");
    if (!response) {
        free(temporary_buf);
        return MHD_NO;
    }
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}
#pragma GCC diagnostic pop

/**
 * @brief      bs_qd_temporary_file_buffer_reader(cls, pos, buf, max)
 *
 * @details    Reads a chunk of data from an intermediate query
 *             write buffer into a response write buffer
 *
 * @param      cls    (void*) I/O context pointer
 *             pos    (uint64_t) byte position into I/O read context
 *             buf    (char*) response write buffer
 *             max    (size_t) callback copies at most max bytes into buf
 *
 * @return     (ssize_t) number of bytes copied
 */

static ssize_t
bs_qd_temporary_file_buffer_reader(void* cls, uint64_t pos, char* buf, size_t max)
{
    bs_qd_io_t* io = (bs_qd_io_t *) cls;
    FILE *read_fp = io->read_fp;
    (void) fseek(read_fp, pos, SEEK_SET);
    return fread(buf, 1, max, read_fp);
}

/**
 * @brief      bs_qd_buffer_callback(cls)
 *
 * @details    Callback gets called at the end of processing
 *             the response, and it is used here to free IO
 *             resources used to read from the byte-store
 *             container.
 *
 * @param      cls    (void*) I/O context pointer
 */

static void
bs_qd_temporary_file_buffer_callback(void* cls)
{
    bs_qd_io_t* io = (bs_qd_io_t *) cls;
    if (io->read_fp)
        fclose(io->read_fp), io->read_fp = NULL;
    if (io->write_fp)
        fclose(io->write_fp), io->write_fp = NULL;
    int err = unlink(io->write_fn);
    if (err == -1) {
        fprintf(stderr, "Error: Could not delete temporary buffer! [%s]\n", strerror(errno));
    }
    fprintf(stderr, "Request [%" PRIu64 "]: Deleted file [%s]\n", io->con_info->timestamp, io->write_fn);
    free(io->write_fn), io->write_fn = NULL;
    free(io), io = NULL;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static int
bs_qd_request_malformed(const void* cls, const char* mime, struct MHD_Connection* connection, bs_qd_connection_info_t* con_info, const char* upload_data, size_t* upload_data_size)
{
    int ret;
    struct MHD_Response *response;

    /* update connection information */
    con_info->request_type = kBSQDRequestMalformed;

    response = MHD_create_response_from_buffer(strlen(MALFORMED_ERROR), (void*) MALFORMED_ERROR, MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
    MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_ENCODING, mime);
    MHD_destroy_response(response);
    return ret;
}
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static int
bs_qd_request_upload_too_large(const void* cls, const char* mime, struct MHD_Connection* connection, bs_qd_connection_info_t* con_info, const char* upload_data, size_t* upload_data_size)
{
    int ret;
    struct MHD_Response *response;

    /* update connection information */
    con_info->request_type = kBSQDRequestUploadTooLarge;

    response = MHD_create_response_from_buffer(strlen(UPLOAD_FILESIZE_TOO_LARGE_ERROR), (void*) UPLOAD_FILESIZE_TOO_LARGE_ERROR, MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
    MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_ENCODING, mime);
    MHD_destroy_response(response);
    return ret;
}
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static int
bs_qd_request_not_found(const void* cls, const char* mime, struct MHD_Connection* connection, bs_qd_connection_info_t* con_info, const char* upload_data, size_t* upload_data_size)
{
    int ret;
    struct MHD_Response *response;

    /* update connection information */
    con_info->request_type = kBSQDRequestNotFound;

    response = MHD_create_response_from_buffer(strlen(NOT_FOUND_ERROR), (void*) NOT_FOUND_ERROR, MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
    MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_ENCODING, mime);
    MHD_destroy_response(response);
    return ret;
}
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static int
bs_qd_parameters_not_found(const void* cls, const char* mime, struct MHD_Connection* connection, bs_qd_connection_info_t* con_info, const char* upload_data, size_t* upload_data_size)
{
    int ret;
    struct MHD_Response *response;
    
    /* update connection information */
    con_info->request_type = kBSQDRequestParametersNotFound;

    response = MHD_create_response_from_buffer(strlen(PARAMETERS_NOT_FOUND_ERROR), (void*) PARAMETERS_NOT_FOUND_ERROR, MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
    MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_ENCODING, NULL);
    MHD_destroy_response(response);
    return ret;
}
#pragma GCC diagnostic pop

/**
 * @brief      bs_test_answer_to_connection()
 *
 * @details    Return a test response to any query
 *
 * @params     cls                (void*)        argument given together with the function
 *                                               pointer when the handler was registered with MHD
 *             url                (const char*)  the requested url
 *             method             (const char*)  the HTTP method used (#MHD_HTTP_METHOD_GET,
 *                                               #MHD_HTTP_METHOD_PUT, etc.)
 *             version            (const char*)  the HTTP version string (i.e.
 *                                               #MHD_HTTP_VERSION_1_1)
 *             upload_data        (const char*)  the data being uploaded (excluding HEADERS,
 *                                               for a POST that fits into memory and that is encoded
 *                                               with a supported encoding, the POST data will NOT be
 *                                               given in upload_data and is instead available as
 *                                               part of #MHD_get_connection_values; very large POST
 *                                               data *will* be made available incrementally in
 *                                               @a upload_data)
 *             upload_data_size   (size_t*)      set initially to the size of the
 *                                               @a upload_data provided; the method must update this
 *                                               value to the number of bytes NOT processed;
 *             con_cls            (void**)       pointer that the callback can set to some
 *                                               address and that will be preserved by MHD for future
 *                                               calls for this request; since the access handler may
 *                                               be called many times (i.e., for a PUT/POST operation
 *                                               with plenty of upload data) this allows the application
 *                                               to easily associate some request-specific state.
 *                                               If necessary, this state can be cleaned up in the
 *                                               global #MHD_RequestCompletedCallback (which
 *                                               can be set with the #MHD_OPTION_NOTIFY_COMPLETED).
 *                                               Initially, `*con_cls` will be NULL.
 *
 * @return     (int) response success or failure
 */

/* 
    This test response does not use most of the parameters in 
    an MHD_AccessHandlerCallback function, so we direct the compiler 
    to ignore unused parameter warnings 
*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static int
bs_qd_test_answer_to_connection(void* cls, struct MHD_Connection *connection, const char* url, const char* method, const char* version, const char* upload_data, size_t* upload_data_size, void** con_cls)
{
    const char *page = "<html><body>Hello from byte-store!</body></html>";
    struct MHD_Response *response;
    int ret;

    response = MHD_create_response_from_buffer(strlen(page), (void *)page, MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop

/**
 * @brief      bs_get_host_fqdn()
 *
 * @details    Attempts to return FQDN of current host
 *
 * @return     (char*) fully-qualified domain name
 */

static char*
bs_qd_get_host_fqdn() 
{
    struct addrinfo hints, *info, *p;
    int gai_err;
    char hn[HOST_NAME_MAX];
    char* fqdn = NULL;

    hn[HOST_NAME_MAX - 1] = '\0';
    gethostname(hn, HOST_NAME_MAX - 1);
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; /* either IPV4 or IPV6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;
    if ((gai_err = getaddrinfo(hn, "http", &hints, &info)) != 0) {
        fprintf(stderr, "Warning: Could not parse hints into hostname [%s: ", gai_strerror(gai_err));
        switch (gai_err) {
        case EAI_AGAIN:
            fprintf(stderr, "EAI_AGAIN]\n");
            break;
        case EAI_BADFLAGS:
            fprintf(stderr, "EAI_BADFLAGS]\n");
            break;
        case EAI_FAIL:
            fprintf(stderr, "EAI_FAIL]\n");
            break;
        case EAI_FAMILY:
            fprintf(stderr, "EAI_FAMILY]\n");
            break;
        case EAI_MEMORY:
            fprintf(stderr, "EAI_MEMORY]\n");
            break;
        case EAI_NONAME:
            fprintf(stderr, "EAI_NONAME]\n");
            break;
        case EAI_SERVICE:
            fprintf(stderr, "EAI_SERVICE]\n");
            break;
        case EAI_SOCKTYPE:
            fprintf(stderr, "EAI_SOCKTYPE]\n");
            break;
        case EAI_SYSTEM:
            fprintf(stderr, "EAI_SYSTEM]\n");
            break;
        default:
            fprintf(stderr, "OTHER]\n");
            break;
        }
    }
    else {
        for (p = info; p != NULL; p = p->ai_next) {
            /* fprintf(stderr, "hostname: %s\n", p->ai_canonname); */
            free(fqdn), fqdn = NULL;
            fqdn = malloc(strlen(hn) + 1);
            memcpy(fqdn, hn, strlen(hn) + 1);
        }
        if (info) {
            freeaddrinfo(info);
        }
        return fqdn;
    }
    return NULL;
}

static boolean_t
bs_qd_test_dependencies()
{
    char* p = NULL;
    char* path = NULL;

    if ((p = getenv("PATH")) == NULL) {
        fprintf(stderr, "Error: Cannot retrieve environment PATH variable\n");
        exit(EINVAL); /* Invalid argument (POSIX.1) */
    }

    path = malloc(strlen(p) + 1);
    if (!path) {
        fprintf(stderr, "Error: Cannot allocate space for path variable copy\n");
        exit(ENOMEM); /* Not enough space (POSIX.1) */
    }
    memcpy(path, p, strlen(p) + 1);

    /* bedextract */

    char *bedextract = NULL;
    bedextract = malloc(strlen(bs_qd_bedextract) + 1);
    if (!bedextract) {
        fprintf(stderr, "Error: Cannot allocate space for bedextract variable copy\n");
        exit(ENOMEM); /* Not enough space (POSIX.1) */
    }
    memcpy(bedextract, bs_qd_bedextract, strlen(bs_qd_bedextract) + 1);

    char *path_bedextract = NULL;
    path_bedextract = malloc(strlen(path) + 1);
    if (!path_bedextract) {
        fprintf(stderr, "Error: Cannot allocate space for path (bedextract) copy\n");
        exit(ENOMEM); /* Not enough space (POSIX.1) */
    }
    memcpy(path_bedextract, path, strlen(path) + 1);

    if (bs_qd_print_matches(path_bedextract, bedextract) != kTrue) {
        fprintf(stderr, "Error: Cannot find bedextract binary required for querying BED\n");
        exit(ENOENT); /* No such file or directory (POSIX.1) */
    }
    free(path_bedextract), path_bedextract = NULL;
    free(bedextract), bedextract = NULL;

    /* bedops */

    char *bedops = NULL;
    bedops = malloc(strlen(bs_qd_bedops) + 1);
    if (!bedops) {
        fprintf(stderr, "Error: Cannot allocate space for bedops variable copy\n");
        exit(ENOMEM); /* Not enough space (POSIX.1) */
    }
    memcpy(bedops, bs_qd_bedops, strlen(bs_qd_bedops) + 1);

    char *path_bedops = NULL;
    path_bedops = malloc(strlen(path) + 1);
    if (!path_bedops) {
        fprintf(stderr, "Error: Cannot allocate space for path (bedops) copy\n");
        exit(ENOMEM); /* Not enough space (POSIX.1) */
    }
    memcpy(path_bedops, path, strlen(path) + 1);

    if (bs_qd_print_matches(path_bedops, bedops) != kTrue) {
        fprintf(stderr, "Error: Cannot find bedops binary required for querying BED\n");
        exit(ENOENT); /* No such file or directory (POSIX.1) */
    }
    free(path_bedops), path_bedops = NULL;
    free(bedops), bedops = NULL;

    /* bedmap */

    char *bedmap = NULL;
    bedmap = malloc(strlen(bs_qd_bedmap) + 1);
    if (!bedmap) {
        fprintf(stderr, "Error: Cannot allocate space for bedmap variable copy\n");
        exit(ENOMEM); /* Not enough space (POSIX.1) */
    }
    memcpy(bedmap, bs_qd_bedmap, strlen(bs_qd_bedmap) + 1);

    char *path_bedmap = NULL;
    path_bedmap = malloc(strlen(path) + 1);
    if (!path_bedmap) {
        fprintf(stderr, "Error: Cannot allocate space for path (bedmap) copy\n");
        exit(ENOMEM); /* Not enough space (POSIX.1) */
    }
    memcpy(path_bedmap, path, strlen(path) + 1);

    if (bs_qd_print_matches(path_bedmap, bedmap) != kTrue) {
        fprintf(stderr, "Error: Cannot find bedmap binary required for querying BED\n");
        exit(ENOENT); /* No such file or directory (POSIX.1) */
    }
    free(path_bedmap), path_bedmap = NULL;
    free(bedmap), bedmap = NULL;

    /* sort-bed */

    char *sortbed = NULL;
    sortbed = malloc(strlen(bs_qd_sortbed) + 1);
    if (!sortbed) {
        fprintf(stderr, "Error: Cannot allocate space for sortbed variable copy\n");
        exit(ENOMEM); /* Not enough space (POSIX.1) */
    }
    memcpy(sortbed, bs_qd_sortbed, strlen(bs_qd_sortbed) + 1);

    char *path_sortbed = NULL;
    path_sortbed = malloc(strlen(path) + 1);
    if (!path_sortbed) {
        fprintf(stderr, "Error: Cannot allocate space for path (sortbed) copy\n");
        exit(ENOMEM); /* Not enough space (POSIX.1) */
    }
    memcpy(path_sortbed, path, strlen(path) + 1);

    if (bs_qd_print_matches(path_sortbed, sortbed) != kTrue) {
        fprintf(stderr, "Error: Cannot find sort-bed binary required for querying BED\n");
        exit(ENOENT); /* No such file or directory (POSIX.1) */
    }
    free(path_sortbed), path_sortbed = NULL;
    free(sortbed), sortbed = NULL;

    return kTrue;
}

static boolean_t
bs_qd_print_matches(char* path, char* fn)
{
    char candidate[PATH_MAX];
    const char* d;
    boolean_t found = kFalse;

    if (strchr(fn, '/') != NULL) {
        return (bs_qd_is_there(fn) ? kTrue : kFalse);
    }

    while ((d = bs_qd_strsep(&path, ":")) != NULL) {
        if (*d == '\0') {
            d = ".";
        }
        if (snprintf(candidate, sizeof(candidate), "%s/%s", d, fn) >= (int) sizeof(candidate)) {
            continue;
        }
        if (bs_qd_is_there(candidate)) {
            found = kTrue;
            /* bedmap */
            if (strcmp(fn, bs_qd_bedextract) == 0) {
                bs_globals.bedextract_path = malloc(strlen(candidate) + 1);
                if (!bs_globals.bedextract_path) {
                    fprintf(stderr, "Error: Could not allocate space for storing bedextract path global\n");
                    exit(ENOMEM); /* Not enough space (POSIX.1) */
                }
                memcpy(bs_globals.bedextract_path, candidate, strlen(candidate));
                bs_globals.bedextract_path[strlen(candidate)] = '\0';
            }
            /* bedops */
            else if (strcmp(fn, bs_qd_bedops) == 0) {
                bs_globals.bedops_path = malloc(strlen(candidate) + 1);
                if (!bs_globals.bedops_path) {
                    fprintf(stderr, "Error: Could not allocate space for storing bedops path global\n");
                    exit(ENOMEM); /* Not enough space (POSIX.1) */
                }
                memcpy(bs_globals.bedops_path, candidate, strlen(candidate));
                bs_globals.bedops_path[strlen(candidate)] = '\0';
            }
            /* bedmap */
            else if (strcmp(fn, bs_qd_bedmap) == 0) {
                bs_globals.bedmap_path = malloc(strlen(candidate) + 1);
                if (!bs_globals.bedmap_path) {
                    fprintf(stderr, "Error: Could not allocate space for storing bedmap path global\n");
                    exit(ENOMEM); /* Not enough space (POSIX.1) */
                }
                memcpy(bs_globals.bedmap_path, candidate, strlen(candidate));
                bs_globals.bedmap_path[strlen(candidate)] = '\0';
            }
            /* sort-bed */
            else if (strcmp(fn, bs_qd_sortbed) == 0) {
                bs_globals.sortbed_path = malloc(strlen(candidate) + 1);
                if (!bs_globals.sortbed_path) {
                    fprintf(stderr, "Error: Could not allocate space for storing sort-bed path global\n");
                    exit(ENOMEM); /* Not enough space (POSIX.1) */
                }
                memcpy(bs_globals.sortbed_path, candidate, strlen(candidate));
                bs_globals.sortbed_path[strlen(candidate)] = '\0';
            }
            break;
        }
    }

    return found;
}

static char*
bs_qd_strsep(char** stringp, const char* delim)
{
    char* s;
    const char* spanp;
    int c, sc;
    char* tok;

    if ((s = *stringp) == NULL)
        return NULL;

    for (tok = s;;) {
        c = *s++;
        spanp = delim;
        do {
            if ((sc = *spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return tok;
            }
        } while (sc != 0);
    }

    return NULL;
}

static boolean_t
bs_qd_is_there(char* candidate)
{
    struct stat fin;
    boolean_t found = kFalse;

    if (access(candidate, X_OK) == 0 
        && stat(candidate, &fin) == 0 
        && S_ISREG(fin.st_mode) 
        && (getuid() != 0 || (fin.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0)) {
        found = kTrue;
    }

    return found;
}

/**
 * @brief      bs_encode_score_to_byte(d)
 *
 * @details    Encodes double-like score value between -1 and +1 to 
 *             byte_t byte "bin".
 *
 *             (-1.01, -1.00] (-1.00, -0.99] (-0.99, -0.98] ...
 *                        ... (-0.01, -0.00] [+0.00, +0.01) ... 
 *                        ... [+0.98, +0.99) [+0.99, +1.00) [+1.00, +1.01)
 *
 *                                  ---to---
 *
 *             { 0x00, 0x01, 0x02, ... , 0x64, 0x65, ... , 0xc8, 0xc9 }
 *
 * @param      d      (score_t) value to be encoded
 *
 * @return     (byte_t) encoded score byte value
 *
 * @example    bs_encode_score_to_byte(-0.010) = 0x63
 *             bs_encode_score_to_byte(-0.009) = 0x64
 *             bs_encode_score_to_byte(-0.000) = 0x64
 *             bs_encode_score_to_byte(+0.000) = 0x65
 *             bs_encode_score_to_byte(+0.139) = 0x72
 *             bs_encode_score_to_byte(+0.140) = 0x73
 *             bs_encode_score_to_byte(+0.142) = 0x73
 */

static inline byte_t
bs_encode_score_to_byte(score_t d) 
{
    if (isnan(d)) {
        return kNANEncodedByte;
    }
    d += (d < 0) ? -kEpsilon : kEpsilon; /* jitter is used to deal with interval edges */
    int encode_d = (int)(100*d) + 100 + bs_signbit(-d);
    //fprintf(stderr, "%f\t%d\t%d\t%d\t%02x\n", d, (int)(100*d), bs_signbit(-d), encode_d, (byte_t) encode_d);
    return (byte_t) encode_d;
}

/**
 * @brief      bs_encode_score_to_byte_mqz(d)
 *
 * @details    Encodes double-like score value between -1 
 *             and +1 to byte-type byte "bin". If value is 
 *             between (-0.25, +0.25) then encoding returns 
 *             +0.00.
 *
 * @param      d      (score_t) value to be encoded
 *
 * @return     (byte_t) encoded score byte value
 */

static inline byte_t
bs_encode_score_to_byte_mqz(score_t d) 
{
    if (isnan(d)) {
        return kNANEncodedByte;
    }
    d += (d < 0) ? -kEpsilon : kEpsilon; /* jitter is used to deal with interval edges */
    int encode_d = (int)(100*d) + 100 + bs_signbit(-d);
    if ((encode_d > 76) && (encode_d < 127))
        encode_d = 100;
    return (byte_t) encode_d;
}

/**
 * @brief      bs_encode_score_to_byte_custom(d, min, max)
 *
 * @details    Encodes double-like score value between -1 
 *             and +1 to byte-type byte "bin". If value is 
 *             between (min, max) then encoding returns +0.00.
 *
 * @param      d      (score_t) value to be encoded
 *             min    (score_t) minimum cutoff value for zero-encoding
 *             max    (score_t) maximum cutoff value for zero-encoding
 *
 * @return     (byte_t) encoded score byte value
 */

static byte_t
bs_encode_score_to_byte_custom(score_t d, score_t min, score_t max) 
{
    if (isnan(d)) {
        return kNANEncodedByte;
    }
    d += (d < 0) ? -kEpsilon : kEpsilon; /* jitter is used to deal with interval edges */
    int encode_d = (int)(100*d) + 100 + bs_signbit(-d);
    int encode_min = (int)(100*min) + 100 + bs_signbit(-min);
    int encode_max = (int)(100*min) + 100 + bs_signbit(-max);
    if ((encode_d > encode_min) && (encode_d < encode_max))
        encode_d = 100;
    return (byte_t) encode_d;
}

/**
 * @brief      bs_signbit(d)
 *
 * @details    Calculates compiler-independent signbit() shift value. To represent
 *             true and false results, clang signbit() returns either 0 or 1, while 
 *             GNU gcc appears to return either 0 or 128.
 *
 * @param      d      (score_t) value to have its sign evaluated
 *
 * @return     (boolean_t) true, if sign is negative; false, if sign is positive
 */

static inline boolean_t
bs_signbit(score_t d)
{
    return (signbit(d) > 0) ? kTrue : kFalse;
}

/**
 * @brief      bs_decode_byte_to_score(uc)
 *
 * @details    Decodes byte-type byte bin to
 *             equivalent score bin.
 *
 * @param      uc     (byte) value to be decoded
 *
 * @return     (score_t) decoded score bin start value
 *
 * @example    bs_decode_byte_to_score(0x64) = -0.00 -- or bin (-0.01, -0.00]
 *             bs_decode_byte_to_score(0x65) = +0.00 -- or bin [+0,00, +0.01)
 *             bs_decode_byte_to_score(0x73) = +0.14 -- or bin [+0,14, +0.15)
 */

static inline score_t
bs_decode_byte_to_score(byte_t uc)
{
    return bs_encode_byte_to_score_table[uc];
}

/**
 * @brief      bs_decode_byte_to_score_mqz(uc)
 *
 * @details    Decodes byte-type byte bin to
 *             equivalent mid-quarter-zero score bin.
 *
 * @param      uc     (byte_t) value to be decoded
 *
 * @return     (score_t) decoded score bin start value
 */

static inline score_t
bs_decode_byte_to_score_mqz(byte_t uc)
{
    return bs_encode_byte_to_score_mqz_table[uc];
}

/**
 * @brief      bs_decode_byte_to_score_custom(uc)
 *
 * @details    Decodes byte-type byte bin to
 *             equivalent custom zero-ranged score bin.
 *
 * @param      uc     (byte_t) value to be decoded
 *             min    (score_t) minimum value to be decoded to zero byte
 *             max    (score_t) maximum value to be decoded to zero byte
 *
 * @return     (score_t) decoded score bin start value
 */

static inline score_t
bs_decode_byte_to_score_custom(byte_t uc, score_t min, score_t max)
{
    score_t test = bs_encode_byte_to_score_table[uc];
    return ((min < test) && (test < max)) ? +0.00f : test;        
}

/**
 * @brief      bs_parse_query_range_str(l, rs, start, end)
 *
 * @details    Parses range-query string, performs some validation, and
 *             scans lookup table for start and end indices for matching
 *             rows.
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             rs     (char*) range string
 *             start  (size_t*) start index to be populated
 *             end    (size_t*) end index to be populated
 *
 * @return     (boolean_t) whether rows were found in provided range
 */

boolean_t
bs_parse_query_range_str(lookup_t* l, char* rs, size_t* start, size_t* end)
{
    if (!rs) {
        fprintf(stderr, "Error: Range string is empty and cannot be parsed into components!\n");
        exit(EXIT_FAILURE);
    }

    /* 
       range string format:
       --------------------------------------
       chrA:coordA-chrB:coordB
    */

    ssize_t rs_len = strlen(rs);
    char *rs_substr_start = rs;
    char buf[QUERY_MAX_LEN] = {0};    
    char chrA[QUERY_MAX_LEN] = {0};
    char chrB[QUERY_MAX_LEN] = {0};
    uint64_t coordA = 0;
    uint64_t coordB = 0;
    char* rs_delim = NULL;
    ssize_t substr_len;
    ssize_t cumulative_len = 0;

    /* chrA */
    rs_delim = strchr(rs_substr_start, kQueryRangeWithinDelim);
    substr_len = rs_delim - rs_substr_start;
    if ((!rs_delim) || (substr_len < 0) || (substr_len >= QUERY_MAX_LEN)) {
        fprintf(stderr, "Error: Query range string not formatted correctly? (A) (chrA:coordA-chrB:coordB)\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    memcpy(buf, rs_substr_start, substr_len);
    buf[substr_len] = '\0';
    cumulative_len += substr_len;
    strcpy(chrA, buf);
    rs_substr_start = rs_delim + 1;

    /* coordA */
    rs_delim = strchr(rs_substr_start, kQueryRangeBetweenDelim);
    substr_len = rs_delim - rs_substr_start;
    if ((!rs_delim) || (substr_len < 0) || (substr_len >= QUERY_MAX_LEN)) {
        fprintf(stderr, "Error: Query range string not formatted correctly? (B) (chrA:coordA-chrB:coordB)\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    memcpy(buf, rs_substr_start, substr_len);
    buf[substr_len] = '\0';
    cumulative_len += substr_len;    
    sscanf(buf, "%" SCNu64, &coordA);
    rs_substr_start = rs_delim + 1;

    /* chrB */
    rs_delim = strchr(rs_substr_start, kQueryRangeWithinDelim);
    substr_len = rs_delim - rs_substr_start;
    if ((!rs_delim) || (substr_len < 0) || (substr_len >= QUERY_MAX_LEN)) {
        fprintf(stderr, "Error: Query range string not formatted correctly? (C) (chrA:coordA-chrB:coordB)\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    memcpy(buf, rs_substr_start, substr_len);
    buf[substr_len] = '\0';
    cumulative_len += substr_len;    
    strcpy(chrB, buf);
    rs_substr_start = rs_delim + 1;

    /* coordB */
    memcpy(buf, rs_substr_start, rs_len - cumulative_len);
    buf[rs_len - cumulative_len] = '\0';    
    sscanf(buf, "%" SCNu64, &coordB);

    if ((strcmp(chrA, chrB) > 0) || (strcmp(chrA, chrB) && (coordA > coordB))) {
        fprintf(stderr, "Error: Query bounds incorrect!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* iterate through lookup table's elements to find start index */
    element_t* elem = NULL;
    size_t start_elem_idx;
    boolean_t start_set_flag = kFalse;
    for (start_elem_idx = 0; start_elem_idx < l->nelems; start_elem_idx++) {
        elem = l->elems[start_elem_idx];
        if (strcmp(elem->chr, chrA) < 0) {
            continue;
        }
        else if ((strcmp(elem->chr, chrA) == 0) && (elem->start >= coordA) && (elem->stop <= coordB)) {
            *start = start_elem_idx;
            start_set_flag = kTrue;
            break;
        }
        else if (strcmp(elem->chr, chrA) > 0) {
            break;
        }        
    }
    *end = *start;
    size_t end_elem_idx;
    for (end_elem_idx = start_elem_idx + 1; end_elem_idx < l->nelems; end_elem_idx++) {
        elem = l->elems[end_elem_idx];
        if (strcmp(elem->chr, chrB) < 0) {
            *end = end_elem_idx;
        }
        else if ((strcmp(elem->chr, chrB) == 0) && (elem->stop <= coordB)) {
            *end = end_elem_idx;
        }
        else if (strcmp(elem->chr, chrB) > 0) {
            continue;
        }        
    }

    return start_set_flag;
}

/**
 * @brief      bs_parse_query_index_str(l)
 *
 * @details    Parses index-query string and performs some validation
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *
 * @return     (boolean_t) if rows are found or not
 */

boolean_t
bs_parse_query_index_str(lookup_t* l)
{
    /* parse query string for index values */
    bs_parse_query_str_to_indices(bs_globals.store_query_str, 
                                  &bs_globals.store_query_idx_start, 
                                  &bs_globals.store_query_idx_end);
    
    return (((bs_globals.store_query_idx_start + 1) > l->nelems) || ((bs_globals.store_query_idx_end + 1) > l->nelems)) ? kFalse : kTrue;        
}

/**
 * @brief      bs_parse_query_multiple_index_str(l)
 *
 * @details    Parses multiple-index-query string and performs some validation
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             qs     (char*) string to parse into indices
 *
 * @return     (boolean_t) if rows are found or not
 */

boolean_t
bs_parse_query_multiple_index_str(lookup_t* l, char* qs)
{
    boolean_t index_not_found_flag = kFalse;
    boolean_t indices_found_flag = kTrue;
    
    /* populate store_query_indices copy from qs */    
    char* start = qs;
    char* end = qs;
    char entry_buf[ENTRY_MAX_LEN];
    uint32_t entry_idx = 0;
    size_t* entries = NULL;
    boolean_t finished = kFalse;
    entries = malloc(sizeof(*entries) * MULT_IDX_MAX_NUM);
    if (!entries) {
        fprintf(stderr, "Error: Could not allocate space for temporary multiple indices!\n");
        exit(EXIT_FAILURE);
    }
    do {
        end = strchr(start, kQueryMultipleIndexDelim);
        if (!end) {
            end = qs + strlen(qs);
            finished = kTrue;
        }
        memcpy(entry_buf, start, end - start);
        entry_buf[end - start] = '\0';
        sscanf(entry_buf, "%lu", &entries[entry_idx++]);
        start = end + 1;
    } while (!finished);
    
    if (entry_idx == 0) {
        free(entries), entries = NULL;
        return index_not_found_flag;
    }
    
    /* check that all indices are between 0 and l->nelems-1 */
    size_t current_idx = 0;
    size_t max_entry = INT32_MIN;
    do {
        if (entries[current_idx] > max_entry) {
            max_entry = entries[current_idx];
        }
        current_idx++;
    } while (current_idx < entry_idx);
    if (max_entry > (l->nelems - 1)) {
        fprintf(stderr, "Error: Entry in multiple indices is greater than the number of elements in the input index file!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* copy entries array to bs_globals.store_query_indices */
    bs_globals.store_query_indices = NULL;
    bs_globals.store_query_indices = malloc(sizeof(*bs_globals.store_query_indices) * entry_idx);
    if (!bs_globals.store_query_indices) {
        fprintf(stderr, "Error: Could not allocate space for multiple indices!\n");
        exit(EXIT_FAILURE);
    }
    current_idx = 0;
    do {
        bs_globals.store_query_indices[current_idx] = entries[current_idx];
    } while (++current_idx < entry_idx);
    bs_globals.store_query_indices_num = entry_idx;
    bs_globals.store_query_indices_capacity = entry_idx;

    /* clean up entries */
    free(entries), entries = NULL;
    
    /* sort entries array copy */
    qsort(bs_globals.store_query_indices, 
          bs_globals.store_query_indices_num, 
          sizeof(*bs_globals.store_query_indices), 
          bs_parse_query_multiple_index_str_comparator);

    /* copy sorted entries to _starts and _ends arrays */
    bs_globals.store_query_indices_starts = NULL;
    bs_globals.store_query_indices_starts = malloc(sizeof(*bs_globals.store_query_indices_starts) * entry_idx);
    if (!bs_globals.store_query_indices_starts) {
        fprintf(stderr, "Error: Could not allocate space for multiple indices (starts)!\n");
        exit(EXIT_FAILURE);
    }
    bs_globals.store_query_indices_ends = NULL;
    bs_globals.store_query_indices_ends = malloc(sizeof(*bs_globals.store_query_indices_ends) * entry_idx);
    if (!bs_globals.store_query_indices_ends) {
        fprintf(stderr, "Error: Could not allocate space for multiple indices (ends)!\n");
        exit(EXIT_FAILURE);
    }
    current_idx = 0;
    do {
        bs_globals.store_query_indices_starts[current_idx] = bs_globals.store_query_indices[current_idx];
        bs_globals.store_query_indices_ends[current_idx] = bs_globals.store_query_indices[current_idx];
    } while (++current_idx < entry_idx);

    return indices_found_flag;
}

/**
 * @brief      bs_parse_query_multiple_index_file()
 *
 * @details    Parses multiple-index values from file and performs some validation
 *
 * @param      l      (lookup_t*) pointer to lookup table
 * @param      qf     (char*) query file to parse
 *
 * @return     (boolean_t) if file and rows are found or not; check formatting
 */

boolean_t
bs_parse_query_multiple_index_file(lookup_t* l, char* qf)
{
    FILE* fptr = fopen(qf, "r");
    if (!fptr)
        return kFalse;

    bs_globals.store_query_indices_starts = NULL;
    bs_globals.store_query_indices_starts = calloc(MULT_IDX_MAX_NUM, sizeof(*bs_globals.store_query_indices_starts));
    if (!bs_globals.store_query_indices_starts) {
        fprintf(stderr, "Error: Could not allocate space for multiple indices starts!\n");
        exit(EXIT_FAILURE);
    }
    bs_globals.store_query_indices_ends = NULL;
    bs_globals.store_query_indices_ends = calloc(MULT_IDX_MAX_NUM, sizeof(*bs_globals.store_query_indices_ends));
    if (!bs_globals.store_query_indices_ends) {
        fprintf(stderr, "Error: Could not allocate space for multiple indices ends!\n");
        exit(EXIT_FAILURE);
    }
    bs_globals.store_query_indices_capacity = MULT_IDX_MAX_NUM;

    size_t first = 1;
    size_t last = 0;
    int count = 0;
    size_t current_idx = 0;

    while (!feof(fptr)) {
        /* resize array of indices, if necessary */
        if ((current_idx - 2) == bs_globals.store_query_indices_capacity) {
            uint32_t resized_indices_arr_capacity = bs_globals.store_query_indices_capacity * 2;
            size_t* resized_indices_starts_arr = NULL;
            resized_indices_starts_arr = malloc(resized_indices_arr_capacity * sizeof(*bs_globals.store_query_indices_starts));
            if (!resized_indices_starts_arr) {
                fprintf(stderr, "Error: Could not allocate space for resized multiple indices starts!\n");
                exit(EXIT_FAILURE);
            }
            for (uint32_t idx = 0; idx < current_idx; idx++) {
                resized_indices_starts_arr[idx] = bs_globals.store_query_indices_starts[idx];
            }
            free(bs_globals.store_query_indices_starts);
            bs_globals.store_query_indices_starts = resized_indices_starts_arr;
            size_t* resized_indices_ends_arr = NULL;
            resized_indices_ends_arr = malloc(resized_indices_arr_capacity * sizeof(*bs_globals.store_query_indices_ends));
            if (!resized_indices_ends_arr) {
                fprintf(stderr, "Error: Could not allocate space for resized multiple indices ends!\n");
                exit(EXIT_FAILURE);
            }
            for (uint32_t idx = 0; idx < current_idx; idx++) {
                resized_indices_ends_arr[idx] = bs_globals.store_query_indices_ends[idx];
            }
            free(bs_globals.store_query_indices_ends);
            bs_globals.store_query_indices_ends = resized_indices_ends_arr;
            bs_globals.store_query_indices_capacity = resized_indices_arr_capacity;
        }
        count = fscanf(fptr, "%lu-%lu\n", &first, &last);
        if (count != 2) {
            fprintf(stderr, "found something not a range of A-B in the input index file!\n");
            bs_globals.store_query_indices_num = 0;
            return kFalse;
        } else if ( first > last ) {
            fprintf(stderr, "end-range ID less than start-range ID in the input index file!\n");
            bs_globals.store_query_indices_num = 0;
            return kFalse;
        } else if ( last >= l->nelems ) {
            fprintf(stderr, "Error: Entry in multiple indices is greater than the number of elements in the input index file!\n");
            bs_globals.store_query_indices_num = 0;
            return kFalse;
        }
        /* mark start and end (inclusive) */
        bs_globals.store_query_indices_starts[current_idx] = first;
        bs_globals.store_query_indices_ends[current_idx] = last;
        bs_globals.store_query_indices_num++;
        current_idx++;
    }
    return kTrue;
}

int32_t 
bs_parse_query_multiple_index_str_comparator(const void* a, const void* b) 
{ 
    const int32_t* a_ptr = (const int32_t*) a; // casting pointer types 
    const int32_t* b_ptr = (const int32_t*) b;
    return *a_ptr  - *b_ptr;
} 

/**
 * @brief      bs_parse_query_str_to_indices(qs, start, end)
 *
 * @details    Parses index-query string into indices.
 *
 * @param      qs     (char*) string to parse into indices
 *             start  (uint32_t*) pointer to start index value to be populated
 *             end    (uint32_t*) pointer to end index value to be populated
 */

void
bs_parse_query_str_to_indices(char* qs, size_t* start, size_t* end)
{
    ssize_t qs_len = strlen(qs);
    char start_str[QUERY_MAX_LEN] = {0};
    char end_str[QUERY_MAX_LEN] = {0};
    char* qs_delim = strchr(qs, kQueryIndexDelim);
    ssize_t start_len = qs_delim - qs;
    ssize_t end_len = qs_len - start_len;

    if ((!qs_delim) || 
        (start_len < 0) || 
        (start_len >= QUERY_MAX_LEN) || 
        (end_len < 0) || 
        (end_len >= QUERY_MAX_LEN)) 
        {
            fprintf(stderr, "Error: Query index string not formatted correctly?\n");
            bs_print_usage(stderr);
            exit(EXIT_FAILURE);
        }

    memcpy(start_str, qs, start_len);
    memcpy(end_str, qs_delim + 1, end_len);
    
    *start = (size_t) strtol(start_str, NULL, 10);
    *end = (size_t) strtol(end_str, NULL, 10);
}

/**
 * @brief      bs_init_bed(chr, start, end)
 *
 * @details    Read BED components into a bed_t pointer, allocating
 *             memory as required.
 *
 * @param      chr    (char*) nul-terminated chromosome string
 *             start  (uint64_t) start coordinate of BED element
 *             end    (uint64_t) end coordinate of BED element
 *
 * @return     (bed_t*) BED element pointer
 */

bed_t*
bs_init_bed(const char* chr, uint64_t start, uint64_t end)
{
    bed_t* bp = NULL;
    bp = malloc(sizeof(bed_t));
    if (!bp) {
        fprintf(stderr, "Error: Could not allocate space for bed_t pointer!\n");
        exit(EXIT_FAILURE);
    }
    bp->chromosome = NULL;
    bp->chromosome = malloc(strlen(chr) + 1);
    if (!bp->chromosome) {
        fprintf(stderr, "Error: Could not allocate space for bed_t pointer chromosome member!\n");
        exit(EXIT_FAILURE);
    }
    memcpy(bp->chromosome, chr, strlen(chr) + 1);
    bp->start = start;
    bp->end = end;
    return bp;
}

/**
 * @brief      bs_delete_bed(b)
 *
 * @details    Release memory used by bed_t* pointer.
 *
 * @param      b      (bed_t**) pointer to BED element pointer
 */

void
bs_delete_bed(bed_t** b)
{
    free((*b)->chromosome), (*b)->chromosome = NULL;
    free(*b), *b = NULL;
}

/**
 * @brief      bs_init_lookup(fn, pi, ss, ir)
 *
 * @details    Read BED-formatted coordinates into a "lookup table" pointer.
 *             Function allocates memory to lookup table pointer, as needed.
 *
 * @param      fn     (char*) filename string
 *             pi     (boolean_t) flag to decide whether to parse ID string
 *             ss     (boolean_t) flag to decide whether to calculate summary statistics
 *             ir     (boolean_t) flag to decide whether to store a rank
 *
 * @return     (lookup_t*) lookup table pointer referencing element data
 */

lookup_t*
bs_init_lookup(char* fn, boolean_t pi, boolean_t ss, boolean_t ir)
{
    lookup_t* l = NULL;
    FILE* lf = NULL;
    char* buf = NULL;
    size_t buf_len = 0;
    ssize_t buf_read = 0;
    char chr_str[CHR_MAX_LEN] = {0};
    char start_str[COORD_MAX_LEN] = {0};
    char stop_str[COORD_MAX_LEN] = {0};
    char id_str[ID_MAX_LEN] = {0};
    uint64_t start_val = 0;
    uint64_t stop_val = 0;
    
    l = malloc(sizeof(lookup_t));
    if (!l) {
        fprintf(stderr, "Error: Could not allocate space for lookup table!\n");
        exit(EXIT_FAILURE);
    }
    l->capacity = 0;
    l->nelems = 0;
    l->elems = NULL;

    lf = fopen(fn, "r");
    if (ferror(lf)) {
        fprintf(stderr, "Error: Could not open or read from [%s]\n", fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* parse BED element into element_t* and push to lookup table */
    while ((buf_read = getline(&buf, &buf_len, lf)) != -1) {
        sscanf(buf, "%s\t%s\t%s\t%s\n", chr_str, start_str, stop_str, id_str);
        sscanf(start_str, "%" SCNu64, &start_val);
        sscanf(stop_str, "%" SCNu64, &stop_val);
        element_t* e = NULL;
        signal_t* sp = NULL;
        bs_init_element(chr_str, start_val, stop_val, id_str, pi, ss, ir, sp, &e);
        bs_push_elem_to_lookup(e, &l, pi, ss, ir);
    }

    free(buf);
    fclose(lf);

    return l;
}

/**
 * @brief      bs_permute_lookup(l, os)
 *
 * @details    Permute contents of lookup_t* and print frequency table to output stream.
 *
 * @param      l      (lookup_t*) lookup table pointer
 *             os     (FILE*) output stream pointer
 */

void
bs_permute_lookup(lookup_t *l, FILE* os)
{
    /* 
       Via permutation tests on signal_t vectors, we can shuffle the signal
       within each vector some number of times and then calculate a sample 
       of r scores that determine a two-sided 99% confidence interval where 
       we observe no correlation.

       Once that interval is determined, we can decide what values have "significant"
       correlation. An encoding strategy can be applied that "zeroes" out scores 
       outside this region of significance for optimal compression.

       If we provide the desired precision, significance threshold, and confidence
       interval increment, we can calculate the minimum number of permutation tests:

       k >= ( 2 * l )^2 * ( alpha * (1 - alpha) / P^2 )

       l     = multiple of sigma, i.e., 1, 2, 3, etc. which is equivalent 
               to a confidence interval of ~68%, ~95%, ~99%, etc.

       alpha = desired significance threshold

       P     = precision (e.g., 0.01)

       Or, we can provide the number of tests (k) directly.
       
       cf. http://stats.stackexchange.com/a/5754/13384
           http://stats.stackexchange.com/questions/80025
    */

    /* set up minimum permutation count k, if default k is set */
    if (bs_globals.permutation_count == kPermutationTestDefaultCount) {
        bs_globals.permutation_count = ceil( 4 * (bs_globals.permutation_significance_level * bs_globals.permutation_significance_level) * (bs_globals.permutation_alpha * ( 1 - bs_globals.permutation_alpha ) / ( bs_globals.permutation_precision * bs_globals.permutation_precision ) ) );
    }
    
    int32_t sl_perc =
        (bs_globals.permutation_significance_level == 1) ? 68 :
        (bs_globals.permutation_significance_level == 2) ? 95 :
        (bs_globals.permutation_significance_level == 3) ? 99 : 99;
    fprintf(os, "Permutation precision            --   P     = %7.8f\n",         bs_globals.permutation_precision);        
    fprintf(os, "Permutation significance level   --   l     = %u (~%d%%)\n",    bs_globals.permutation_significance_level, sl_perc);        
    fprintf(os, "Permutation alpha                --   alpha = %7.8f\n",         bs_globals.permutation_alpha);    
    fprintf(os, "Permutation count                --   k     = %u\n",            bs_globals.permutation_count);
    
    /* seed RNG */
    if (bs_globals.rng_seed_flag)
        mt19937_seed_rng(bs_globals.rng_seed_value);
    else
        mt19937_seed_rng(time(NULL));

    uint64_t* freq_table = NULL;
    freq_table = calloc(256, sizeof(uint64_t));
    if (!freq_table) {
        fprintf(stderr, "Error: Could not allocate space for permutation frequency table!\n");
        exit(EXIT_FAILURE);
    }
    for (uint32_t permute_idx = 0; permute_idx < bs_globals.permutation_count; permute_idx++) {
        /* shuffle all l->nelem vectors */
        for (uint32_t elem_idx = 0; elem_idx < l->nelems; elem_idx++) {
            bs_shuffle_signal_data(l->elems[elem_idx]->signal->data, l->elems[elem_idx]->signal->n);
        }
        switch (bs_globals.store_type) {
            case kStorePearsonRSUT:
            case kStorePearsonRSquareMatrix:
            case kStorePearsonRSquareMatrixSplit:
            case kStorePearsonRSquareMatrixSplitSingleChunk:
            case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
            case kStorePearsonRSquareMatrixBzip2:
            case kStorePearsonRSquareMatrixBzip2Split:
                bs_increment_lookup_frequency(freq_table, l, &bs_pearson_r_signal);
                break;
            case kStoreSpearmanRhoSquareMatrix:
            case kStoreSpearmanRhoSquareMatrixSplit:
            case kStoreSpearmanRhoSquareMatrixSplitSingleChunk:
            case kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadata:
            case kStoreSpearmanRhoSquareMatrixBzip2:
            case kStoreSpearmanRhoSquareMatrixBzip2Split:
                bs_increment_lookup_frequency(freq_table, l, &bs_spearman_rho_signal_v2);
                break;
            case kStoreRandomSquareMatrix:
            case kStoreRandomBufferedSquareMatrix:
            case kStoreRandomSUT:
            case kStoreUndefined:
            default:
                break;
        }
    }

    uint64_t non_diagonal_n = bs_globals.permutation_count * (((uint64_t) l->nelems * l->nelems) - l->nelems);
    uint64_t test_ndn = ceil((1.0 - bs_globals.permutation_alpha) * non_diagonal_n);
    uint64_t non_signif_ndn = non_diagonal_n - test_ndn;
    uint64_t signif_ndn_width = floor(non_signif_ndn / 2);
    uint64_t start_bin_bound = signif_ndn_width;
    uint64_t end_bin_bound = non_diagonal_n - signif_ndn_width;

    /*
       fprintf(os, "non_diagonal_n: [%" PRIu64 "]\n", non_diagonal_n);
       fprintf(os, "test_ndn: [%" PRIu64 "]\n", test_ndn);
       fprintf(os, "non_signif_ndn: [%" PRIu64 "]\n", non_signif_ndn);
       fprintf(os, "signif_ndn_width: [%" PRIu64 "]\n", signif_ndn_width);        
       fprintf(os, "start_bin_bound: [%" PRIu64 "]\n", start_bin_bound);
       fprintf(os, "end_bin_bound: [%" PRIu64 "]\n", end_bin_bound);    
    */
    
    uint64_t cumulative_freq_count = 0;
    uint32_t start_bin_idx = 0;
    uint32_t end_bin_idx = 0;
    for (uint32_t bin_idx = 0; bin_idx <= 201; bin_idx++) {
        cumulative_freq_count += freq_table[bin_idx];
        if (cumulative_freq_count < start_bin_bound) {
            start_bin_idx = bin_idx;
        }
        if (cumulative_freq_count > end_bin_bound) {
            end_bin_idx = bin_idx;
            break;
        }
    }
    fprintf(os,
            "Non-significant interval         --   [%3.2f, %3.2f]\n",
            bs_encode_byte_to_score_table[start_bin_idx],
            bs_encode_byte_to_score_table[end_bin_idx]);

    /*
       uint64_t n = (uint64_t) l->nelems * l->nelems * bs_globals.permutation_count;
       bs_print_frequency_buffer(freq_table, n, os);
    */
    
    /* clean up */
    free(freq_table), freq_table = NULL;    
}

/**
 * @brief      bs_shuffle_signal_data(d, n)
 *
 * @details    Apply unbiased Fisher-Yates shuffle on score_t array entries.
 *
 * @param      d      (score_t*) data pointer
 *             n      (size_n) number of elements in data pointer
 */

void
bs_shuffle_signal_data(score_t* d, size_t n)
{
    /* cf. http://blog.codinghorror.com/the-danger-of-naivete/ */
    if (n > 1) {
        for (size_t i = 0; i < n; i++) {   
            size_t s = i + (mt19937_generate_random_ulong() % (n - i));
            score_t t = d[s];
            d[s] = d[i];
            d[i] = t;
        }
    }
}

/**
 * @brief      bs_print_lookup(l, os)
 *
 * @details    Print contents of lookup_t* to output stream for debugging.
 *
 * @param      l      (lookup_t*) lookup table pointer
 *             os     (FILE*) output stream pointer
 */

void
bs_print_lookup(lookup_t* l, FILE* os)
{
    fprintf(os, "----------------------\n");
    fprintf(os, "Lookup\n");
    fprintf(os, "----------------------\n");
    for (uint32_t idx = 0; idx < l->nelems; idx++) {
        fprintf(os, 
                "Element [%09d] [%s | %" PRIu64 " | %" PRIu64 " | %s]\n", 
                idx, 
                l->elems[idx]->chr,
                l->elems[idx]->start,
                l->elems[idx]->stop,
                l->elems[idx]->id);
        bs_print_signal(l->elems[idx]->signal, os);
    }
    fprintf(os, "----------------------\n");
}

/**
 * @brief      bs_print_lookup_frequency(l, os)
 *
 * @details    Print bin frequency of lookup_t* to output stream for debugging.
 *
 * @param      l      (lookup_t*) lookup table pointer
 *             os     (FILE*) output stream pointer
 */

void
bs_print_lookup_frequency(lookup_t* l, FILE* os)
{
    uint64_t* freq_table = NULL;
    freq_table = calloc(256, sizeof(uint64_t));
    if (!freq_table) {
        fprintf(stderr, "Error: Could not allocate space for permutation frequency table!\n");
        exit(EXIT_FAILURE);
    }
    switch (bs_globals.store_type) {
        case kStorePearsonRSUT:
        case kStorePearsonRSquareMatrix:
        case kStorePearsonRSquareMatrixSplit:
        case kStorePearsonRSquareMatrixSplitSingleChunk:
        case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
        case kStorePearsonRSquareMatrixBzip2:
        case kStorePearsonRSquareMatrixBzip2Split:
            bs_increment_lookup_frequency(freq_table, l, &bs_pearson_r_signal);
            break;
        case kStoreSpearmanRhoSquareMatrix:
        case kStoreSpearmanRhoSquareMatrixSplit:
        case kStoreSpearmanRhoSquareMatrixSplitSingleChunk:
        case kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadata:
        case kStoreSpearmanRhoSquareMatrixBzip2:
        case kStoreSpearmanRhoSquareMatrixBzip2Split:
            bs_increment_lookup_frequency(freq_table, l, &bs_spearman_rho_signal_v2);
            break;
        case kStoreRandomSquareMatrix:
        case kStoreRandomBufferedSquareMatrix:
        case kStoreRandomSUT:
        case kStoreUndefined:
        default:
            break;
    }
    uint64_t n_bytes = (uint64_t) l->nelems * l->nelems;
    bs_print_frequency_buffer(freq_table, n_bytes, os);
    free(freq_table), freq_table = NULL;
}

/**
 * @brief      bs_increment_lookup_frequency(t, l, *sf)
 *
 * @details    Update score frequency table from lookup struct.
 *
 * @param      t      (uint64_t*) frequency table
 *             l      (lookup_t*) pointer to lookup struct
 *             *sf    (score_t) score function pointer
 */

void
bs_increment_lookup_frequency(uint64_t* t, lookup_t* l, score_t (*sf)(signal_t*, signal_t*, uint32_t))
{
    byte_t self_correlation_score =
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_score_to_byte(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_score_to_byte_mqz(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_score_to_byte_custom(kSelfCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_score_to_byte(kSelfCorrelationScore);

    for (uint32_t row_idx = 0; row_idx < l->nelems; row_idx++) {
        signal_t* row_signal = l->elems[row_idx]->signal;
        t[self_correlation_score]++;
        for (uint32_t col_idx = row_idx + 1; col_idx < l->nelems; col_idx++) {
            signal_t* col_signal = l->elems[col_idx]->signal;
            if (row_signal->n != col_signal->n) {
                fprintf(stderr, "Error: Vectors being correlated are of unequal length!\n");
                bs_print_signal(row_signal, stderr);
                bs_print_signal(col_signal, stderr);
                exit(EXIT_FAILURE);
            }
            score_t pairwise_score = NAN;
            if ((row_signal->data_contains_nan == kFalse) && (col_signal->data_contains_nan == kFalse)) {
                if (((sf == bs_pearson_r_signal) || (sf == bs_spearman_rho_signal_v2)) && (row_signal->sd != 0.0f) && (col_signal->sd != 0.0f)) {
                    pairwise_score = (*sf)(row_signal, col_signal, row_signal->n);
                }
                else {
                    pairwise_score = (*sf)(row_signal, col_signal, row_signal->n);
                }
            }
            byte_t corr_uc =
                (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_score_to_byte(pairwise_score) : 
                (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_score_to_byte_mqz(pairwise_score) : 
                bs_encode_score_to_byte_custom(pairwise_score, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
            t[corr_uc] += 2; /* we add 2 to account for the mirrored element across the diagonal */
        }
    }
}

/**
 * @brief      bs_delete_lookup(l)
 *
 * @details    Release memory used by lookup_t* pointer.
 *
 * @param      l      (lookup_t**) pointer to lookup table pointer
 */

void 
bs_delete_lookup(lookup_t** l)
{
    for (uint32_t idx = 0; idx < (*l)->nelems; idx++) {
        bs_delete_element(&(*l)->elems[idx]);
    }
    free((*l)->elems), (*l)->elems = NULL;
    free(*l), *l = NULL;
}

/**
 * @brief      bs_copy_signal(src, dest, ss, ir)
 *
 * @details    Copy a signal_t pointer.
 *
 * @param      src    (signal_t*)     pointer to signal_t (source)
 *             dest   (signal_t**)    pointer to signal_t pointer (destination)
 *             ss     (boolean_t)     copy summary statistics (mean and SD)
 *             ir     (boolean_t)     copy ranks and related statistics from src
 */

void
bs_copy_signal(signal_t* src, signal_t** dest, boolean_t ss, boolean_t ir)
{
    signal_t* s = NULL;
    s = malloc(sizeof(signal_t));
    if (!s) {
        fprintf(stderr, "Error: Could not allocate space for signal_t pointer copy!\n");
        exit(EXIT_FAILURE);
    }
    s->n = src->n;
    s->data = NULL;
    int data_alloc_result = posix_memalign((void **) &(s->data), kSignalByteAlignment, s->n * sizeof(*s->data));
    if ((data_alloc_result != 0) || (!s->data)) {
        fprintf(stderr, "Error: Could not allocate space for signal data pointer!\n");
        exit(EXIT_FAILURE);
    }
    for (uint32_t idx = 0; idx < s->n; idx++) {
        s->data[idx] = src->data[idx];
    }
    s->data_contains_nan = src->data_contains_nan;
    s->mean = NAN;
    s->sd = NAN;

    /* if we need to copy src->mean, src->sd */
    if (ss) {
        s->mean = src->mean;
        s->sd = src->sd;
    }

    s->ranks = NULL;
    s->mean_ranks = NAN;
    s->sd_ranks = NAN;

    /* if we need to copy src->ranks, src->mean_ranks, src->sd_ranks */
    if (ir) {
        int ranks_alloc_result = posix_memalign((void **) &(s->ranks), kSignalByteAlignment, s->n * sizeof(*s->ranks));
        if ((ranks_alloc_result != 0) || (!s->ranks)) {
            fprintf(stderr, "Error: Could not allocate space for signal ranks pointer!\n");
            exit(EXIT_FAILURE);
        }
        for (uint32_t idx = 0; idx < s->n; idx++) {
            s->ranks[idx] = src->ranks[idx];
        }
        s->mean_ranks = src->mean_ranks;
        s->sd_ranks = src->sd_ranks;
    }

    *dest = s;
}

/**
 * @brief      bs_init_signal(cds, dest, ss, ir)
 *
 * @details    Initialize a signal_t pointer with a vector of score_t's,
 *             along with mean and sample standard deviation of the
 *             vector.
 *
 * @param      cds    (char*)      pointer to comma-delimited string of numerical values
 *             dest   (signal_t**) pointer to signal struct populated with signal data
 *             ss     (boolean_t)  calculate summary statistics (mean and SD) from cds
 *             ir     (boolean_t)  calculate ranks and related statistics from cds
 */

void
bs_init_signal(char* cds, signal_t** dest, boolean_t ss, boolean_t ir)
{
    rank_t rank_idx = 0;
    rank_t* ranks_temp = NULL;
    signal_t* s = NULL;
    s = malloc(sizeof(signal_t));
    if (!s) {
        fprintf(stderr, "Error: Could not allocate space for signal pointer!\n");
        exit(EXIT_FAILURE);
    }
    s->n = 1;
    s->data = NULL;
    s->data_contains_nan = kTrue;
    s->ranks = NULL;
    s->mean = NAN;
    s->sd = NAN;
    s->mean_ranks = NAN;
    s->sd_ranks = NAN;
    for (uint32_t idx = 0; idx < strlen(cds); idx++) {
        if (cds[idx] == kSignalDelim) {
            s->n++;
        }
    }
    int data_alloc_result = posix_memalign((void **) &(s->data), kSignalByteAlignment, s->n * sizeof(*s->data));
    if ((data_alloc_result != 0) || (!s->data)) {
        fprintf(stderr, "Error: Could not allocate space for signal data pointer!\n");
        exit(EXIT_FAILURE);
    }
    if (ir) {
        /* 
            Until the ranks are readjusted to fractional values for duplicate data values, 
            the ranks pointer contains values that can be cast as integers for the purpose 
            of treating them as array indices, allow reordering of data and true rank arrays
            without extra sorting, which we set a maximum number here of INT32_MAX.
        */
        if (s->n > INT32_MAX) {
            fprintf(stderr, "Error: There are more signal data points than the ranks pointer can store!\n");
            exit(EXIT_FAILURE);
        }
        int ranks_temp_alloc_result = posix_memalign((void **) &ranks_temp, kSignalByteAlignment, s->n * sizeof(*s->ranks));
        if ((ranks_temp_alloc_result != 0) || (!ranks_temp)) {
            fprintf(stderr, "Error: Could not allocate space for temporary signal ranks pointer!\n");
            exit(EXIT_FAILURE);
        }
        int ranks_alloc_result = posix_memalign((void **) &(s->ranks), kSignalByteAlignment, s->n * sizeof(*s->ranks));
        if ((ranks_alloc_result != 0) || (!s->ranks)) {
            fprintf(stderr, "Error: Could not allocate space for signal ranks pointer!\n");
            exit(EXIT_FAILURE);
        }
        bs_globals.score_ptr = s->data;
    }
    char* start = cds;
    char* end = cds;
    char entry_buf[ENTRY_MAX_LEN];
    uint32_t entry_idx = 0;
    boolean_t finished_parsing = kFalse;
    boolean_t data_contains_nan = kFalse;
    do {
        end = strchr(start, kSignalDelim);
        if (!end) {
            end = cds + strlen(cds);
            finished_parsing = kTrue;
        }
        memcpy(entry_buf, start, end - start);
        entry_buf[end - start] = '\0';
        if (ir) {
            ranks_temp[entry_idx] = rank_idx;
            s->ranks[entry_idx] = rank_idx++;
        }
        sscanf(entry_buf, "%f", &s->data[entry_idx++]);
        if (isnan(s->data[entry_idx - 1])) {
            data_contains_nan = kTrue;
        }
        start = end + 1;
    } while (!finished_parsing);
    
    if (!data_contains_nan) {
        s->data_contains_nan = kFalse;
    }

    if (ss && !data_contains_nan) {
        s->mean = bs_mean_signal(s->data, s->n);
        if (s->n >= 2) {
            s->sd = bs_sample_sd_signal(s->data, s->n, s->mean);
        }
        else {
            fprintf(stderr, "Warning: Vector has one value and therefore cannot have a standard deviation!\n");
        }
    }

    if (ss && ir && !data_contains_nan) {
        /* sort temporary ranks by special comparator that allows reordering of true ranks */
        qsort(ranks_temp, s->n, sizeof(rank_t), bs_rank_comparator);
        /* reorder true ranks from temporary ranks, and add 1 for later Spearman's rho calculation */
        for (uint32_t idx = 0; idx < s->n; ++idx) {
            s->ranks[(int32_t)ranks_temp[idx]] = idx + 1;
        }
        /* 
            We adjust ranks where there are ties between two or more 
            data points. To do this, we walk through s->data by the 
            ordering specified by the temporary ranks_temp array. 
            We don't need to re-sort, and, further, we can look for 
            contiguous ranges of equal data values, adjusting final 
            ranks as ranges of duplicate data values are discovered. 
        */
        boolean_t ties_in_midstream = kFalse;
        int32_t ties_ranks_temp_start_idx = 0;
        int32_t ties_ranks_temp_stop_idx = 0;
        float ties_avg = 0.0f;
        //fprintf(stderr, "----\n");
        for (uint32_t idx = 0; idx < s->n; ++idx) {
            if (idx > 0) {
                if (s->data[(int32_t)ranks_temp[idx]] == s->data[(int32_t)ranks_temp[idx - 1]]) {
                    if (!ties_in_midstream) {
                        ties_ranks_temp_start_idx = idx - 1;
                    }
                    ties_in_midstream = kTrue;
                }
                else if (ties_in_midstream) {
                    ties_ranks_temp_stop_idx = idx - 1;
                    //fprintf(stderr, "Warning: Ranks tied at indices [%d : %d]\n", ties_ranks_temp_start_idx, ties_ranks_temp_stop_idx);
                    ties_avg = 0.0f;
                    for (int32_t tie_idx = ties_ranks_temp_start_idx; tie_idx <= ties_ranks_temp_stop_idx; ++tie_idx) {
                        ties_avg += tie_idx;
                    }
                    ties_avg /= (ties_ranks_temp_stop_idx - ties_ranks_temp_start_idx + 1);
                    /* 
                        Because array indices are 0-based, and ranks have a 1-based index, 
                        we add 1 to the average result for later Spearman's rho calculation 
                    */
                    ties_avg += 1;
                    //fprintf(stderr, "Warning: Would replace with tie avg [%.2f]\n", ties_avg);
                    for (int32_t tie_idx = ties_ranks_temp_start_idx; tie_idx <= ties_ranks_temp_stop_idx; ++tie_idx) {
                        //fprintf(stderr, "Warning: For data [%f] replacing [%.2f] with [%.2f]\n", s->data[(int32_t)ranks_temp[tie_idx]], s->ranks[(int32_t)ranks_temp[tie_idx]], ties_avg);
                        s->ranks[(int32_t)ranks_temp[tie_idx]] = ties_avg;
                    }
                    ties_in_midstream = kFalse;
                }
            }
            //fprintf(stderr, "[%f | %u | %.2f | %f | %.2f ]\n", s->data[(int32_t)ranks_temp[idx]], idx, ranks_temp[idx], s->data[idx], s->ranks[idx]);
        }
        /* 
            If we get to the end of the data array and there are still 
            duplicates, we finalize the ties adjustment 
        */
        if (ties_in_midstream) {
            ties_ranks_temp_stop_idx = s->n - 1;
            //fprintf(stderr, "Warning: Ranks tied at indices [%d : %d]\n", ties_ranks_temp_start_idx, ties_ranks_temp_stop_idx);
            ties_avg = 0.0f;
            for (int32_t tie_idx = ties_ranks_temp_start_idx; tie_idx <= ties_ranks_temp_stop_idx; ++tie_idx) {
                ties_avg += tie_idx;
            }
            ties_avg /= (ties_ranks_temp_stop_idx - ties_ranks_temp_start_idx + 1);
            /* 
                Because array indices are 0-based, and ranks have a 1-based index, 
                we add 1 to the average result for later Spearman's rho calculation 
            */
            ties_avg += 1;
            //fprintf(stderr, "Warning: Would replace with tie avg [%.2f]\n", ties_avg);
            for (int32_t tie_idx = ties_ranks_temp_start_idx; tie_idx <= ties_ranks_temp_stop_idx; ++tie_idx) {
                //fprintf(stderr, "Warning: For data [%f] replacing [%.2f] with [%.2f]\n", s->data[(int32_t)ranks_temp[tie_idx]], s->ranks[(int32_t)ranks_temp[tie_idx]], ties_avg);
                s->ranks[(int32_t)ranks_temp[tie_idx]] = ties_avg;
            }
            ties_in_midstream = kFalse;
        }
        //fprintf(stderr, "----\n");
        //for (uint32_t idx = 0; idx < s->n; ++idx) {
        //    fprintf(stderr, "[%f | %u | %.2f ]\n", s->data[idx], idx + 1, s->ranks[idx]);
        //}
        s->mean_ranks = bs_mean_ranks(s->ranks, s->n);
        s->sd_ranks = bs_sample_sd_ranks(s->ranks, s->n, s->mean_ranks);
        /* 
            We no longer need temporary rank data
        */
        free(ranks_temp);
        ranks_temp = NULL;
        /*
            If we replace data and data summary statistics with ranks and rank summary statistics, we should be able to release the data values. For now we'll keep data while this is reviewed.
        */
        //free(s->data);
        //s->data = NULL;
    }

    *dest = s;
}

/**
 * @brief      bs_rank_comparator(a, b)
 *
 * @details    This custom rank comparator resorts an array of index
 *             values by the numerical order of the *score data*, which
 *             we access through a global reference to whatever is the
 *             current score pointer.
 *
 * @param      a      (void*) pointer to ranks pointer
 *             b      (void*) pointer to ranks pointer
 *
 * @return     (int) comparator result
 */

static int
bs_rank_comparator(const void *a, const void *b)
{
    /* we must cast fractional ranks to integers to use as array indices */
    int32_t ar = (int32_t) *((rank_t *) a);
    int32_t br = (int32_t) *((rank_t *) b);

    /* for the custom rank comparator, we compare the scores to resort rank indices */
    if (bs_globals.score_ptr[ar] < bs_globals.score_ptr[br]) {
        return -1;
    }
    else if (bs_globals.score_ptr[ar] > bs_globals.score_ptr[br]) {
        return 1;
    }
    return 0; /* scores are equal, so we do nothing to rank indices */
}

/**
 * @brief      bs_print_signal(s, os)
 *
 * @details    Print a signal struct's details to output stream
 *
 * @param      s      (signal_t*) pointer to signal struct to be printed
 *             os     (FILE*) pointer to output stream
 */

void
bs_print_signal(signal_t* s, FILE* os)
{
    fprintf(os, "vector -> [");
    for (uint32_t idx = 0; idx < s->n; idx++) {
        fprintf(os, "%3.6f", s->data[idx]);
        if (idx != (s->n - 1)) {
            fprintf(os, ", ");
        }
    }
    fprintf(os, "]\n");
    fprintf(os, "n      -> [%u]\n", s->n);
    fprintf(os, "mean   -> [%3.6f]\n", s->mean);
    fprintf(os, "sd     -> [%3.6f]\n", s->sd);
}

/**
 * @brief      bs_mean_signal(d, len)
 *
 * @details    Calculates the arithmetic mean of the 
 *             provided array of score_t's of given length
 *
 * @param      d      (score_t*) pointer to score_t's
 *             len    (uint32_t) length of score_t array
 *
 * @return     (score_t) mean value of score_t array
 */

static inline score_t
bs_mean_signal(score_t* d, uint32_t len)
{
    score_t s = 0.0f;
    for (uint32_t idx = 0; idx < len; idx++) {
        s += d[idx];
    }
    return s / len;
}

/**
 * @brief      bs_mean_ranks(d, len)
 *
 * @details    Calculates the arithmetic mean of the 
 *             provided array of rank_t's of given length
 *
 * @param      d      (rank_t*) pointer to rank_t's
 *             len    (uint32_t) length of rank_t array
 *
 * @return     (score_t) mean value of rank_t array
 */

static inline score_t
bs_mean_ranks(rank_t* d, uint32_t len)
{
    score_t s = 0.0f;
    for (uint32_t idx = 0; idx < len; idx++) {
        s += (score_t) d[idx];
    }
    return s / len;
}

/**
 * @brief      bs_sample_sd_signal(d, len, m)
 *
 * @details    Calculates the sample standard deviation of the 
 *             provided array of score_t's of given length and mean
 *
 * @param      d      (score_t*) pointer to score_t's
 *             len    (uint32_t) length of score_t array
 *             m      (score_t) arithmetic mean of score_t array
 *
 * @return     (score_t) sample standard deviation value of score_t array
 */

static inline score_t
bs_sample_sd_signal(score_t* d, uint32_t len, score_t m)
{
    score_t s = 0.0f;
    for (uint32_t idx = 0; idx < len; idx++) {
        s += (d[idx] - m) * (d[idx] - m);
    }
    return sqrt(s / (len - 1));
}

/**
 * @brief      bs_sample_sd_ranks(d, len, m)
 *
 * @details    Calculates the sample standard deviation of the 
 *             provided array of rank_t's of given length and mean
 *
 * @param      d      (rank_t*) pointer to rank_t's
 *             len    (uint32_t) length of rank_t array
 *             m      (score_t) arithmetic mean of rank_t array
 *
 * @return     (score_t) sample standard deviation value of score_t array
 */

score_t
bs_sample_sd_ranks(rank_t* d, uint32_t len, score_t m)
{
    score_t s = 0.0f;
    for (uint32_t idx = 0; idx < len; idx++) {
        s += ((score_t) d[idx] - m) * ((score_t) d[idx] - m);
    }
    return sqrt(s / (len - 1));
}

/**
 * @brief      bs_pearson_r_signal(x, y, len)
 *
 * @details    Calculates the Pearson's r correlation of two
 *             signal vectors
 *
 * @param      x      (signal_t*) pointer to first signal struct
 *             y      (signal_t*) pointer to second signal struct
 *             len    (uint32_t) length of signal vectors
 *
 * @return     (score_t) Pearson's r between [-1,1]
 */

static inline score_t
bs_pearson_r_signal(signal_t* x, signal_t* y, uint32_t len)
{
    score_t s = 0.0f;
    for (uint32_t idx = 0; idx < len; idx++)
        s += (x->data[idx] - x->mean) * (y->data[idx] - y->mean);
    return s / ((len - 1.0f) * x->sd * y->sd);
}

/**
 * @brief      bs_spearman_rho_signal_v1(x, y, len)
 *
 * @details    Calculates the Spearman's rho correlation of two
 *             signal vectors using approximation function that 
 *             will be inaccurate if there are duplicate data
 *             points.
 *
 * @param      x      (signal_t*) pointer to first signal struct
 *             y      (signal_t*) pointer to second signal struct
 *             len    (uint32_t) length of signal vectors
 *
 * @return     (score_t) Spearman's rho between [-1,1]
 */

static inline score_t
bs_spearman_rho_signal_v1(signal_t* x, signal_t* y, uint32_t len)
{
    int64_t sum_of_squared_differences = 0;
    uint32_t idx = 0;
    for (idx = 0; idx < len; ++idx) {
        sum_of_squared_differences += ((x->ranks[idx] - y->ranks[idx]) * (x->ranks[idx] - y->ranks[idx]));
    }
    /* possible overflow in denominator? */
    return 1.0f - (( 6.0f * (score_t) sum_of_squared_differences ) / ((len * len * len) - len));
}

/**
 * @brief      bs_spearman_rho_signal_v2(x, y, len)
 *
 * @details    Calculates the Spearman's rho correlation of two
 *             signal vectors using exact function.
 *
 * @param      x      (signal_t*) pointer to first signal struct
 *             y      (signal_t*) pointer to second signal struct
 *             len    (uint32_t) length of signal vectors
 *
 * @return     (score_t) Spearman's rho between [-1,1]
 */

static inline score_t
bs_spearman_rho_signal_v2(signal_t* x, signal_t* y, uint32_t len)
{
    uint32_t idx = 0;
    score_t covariance_ranks = 0.0f;
    for (idx = 0; idx < len; ++idx) {
        covariance_ranks += ((x->ranks[idx] - x->mean_ranks) * (y->ranks[idx] - y->mean_ranks));
    }
    return covariance_ranks / ((len - 1.0f) * x->sd_ranks * y->sd_ranks);
}

/**
 * @brief      bs_jaccard_index_signal(x, y, len)
 *
 * @details    Calculates the Jaccard index of two signal
 *             vectors; mostly meant for binary matrices
 *
 * @param      x      (signal_t*) pointer to first signal struct
 *             y      (signal_t*) pointer to second signal struct
 *             len    (uint32_t) length of signal vectors
 *
 * @return     (score_t) Jaccard index between [0,1]
 */

static inline score_t
bs_jaccard_index_signal(signal_t* x, signal_t* y, uint32_t len)
{
    /* 
        We use a-b-c-d 2x2 contingency notation:
        
                 y
               1   0
              -------
          1  | a | b |
        x     -------
          0  | c | d |
              -------
    */
    score_t a = 0.0f;
    score_t abc = 0.0f;
    for (uint32_t idx = 0; idx < len; idx++) {
        abc += (x->data[idx] || y->data[idx]);
        a   += (x->data[idx] && y->data[idx]);
    }
    /*
        The formula for the Jaccard index is undefined when 
        both of the vectors being compared are all zeros. If 
        both are all zeros, we define the measure as NaN. If 
        only one of the two vectors is all zeros, the measure 
        is defined to be zero.
    */
    return (abc > 0) ? a / abc : NAN;
}

/**
 * @brief      bs_ochiai_similarity_signal(x, y, len)
 *
 * @details    Calculates the Ochiai similarity measure of 
 *             two signal vectors; mostly meant for binary 
 *             matrices
 *
 * @param      x      (signal_t*) pointer to first signal struct
 *             y      (signal_t*) pointer to second signal struct
 *             len    (uint32_t) length of signal vectors
 *
 * @return     (score_t) Ochiai similarity measure between [0,1]
 */

static inline score_t
bs_ochiai_similarity_signal(signal_t* x, signal_t* y, uint32_t len)
{
    /* 
        We use a-b-c-d 2x2 contingency notation:
        
                 y
               1   0
              -------
          1  | a | b |
        x     -------
          0  | c | d |
              -------
    */
    score_t a = 0.0f;
    score_t b = 0.0f;
    score_t c = 0.0f;
    for (uint32_t idx = 0; idx < len; idx++) {
        a += ( x->data[idx] &&  y->data[idx]);
        b += ( x->data[idx] && !y->data[idx]);
        c += (!x->data[idx] &&  y->data[idx]);
    }
    /*
        The formula for the Ochiai measure is undefined when one 
        or both of the vectors being compared are all zeros. If 
        both are all zeros, we define the measure as one, and 
        if only one of the two vectors is all zeros, the measure 
        is defined to be zero.
    */
    score_t s = a + b + c;
    return (s > 0) ? (((a > 0) || ((b > 0) && (c > 0))) ? a * sqrt(1.0/((a + b)*(a + c))) : 0.0f) : 1.0f;
}

/**
 * @brief      bs_pearson_phi_similarity_signal(x, y, len)
 *
 * @details    Calculates the Pearson product-moment correlation 
 *             coefficient (Pearson phi) of two signal vectors; mostly 
 *             meant for binary matrices
 *
 * @param      x      (signal_t*) pointer to first signal struct
 *             y      (signal_t*) pointer to second signal struct
 *             len    (uint32_t) length of signal vectors
 *
 * @return     (score_t) Pearson phi similarity measure between [-1,1]
 */

static inline score_t
bs_pearson_phi_similarity_signal(signal_t* x, signal_t* y, uint32_t len)
{
    /* 
        We use a-b-c-d 2x2 contingency notation:
        
                 y
               1   0
              -------
          1  | a | b |
        x     -------
          0  | c | d |
              -------
    */
    score_t a = 0.0f;
    score_t b = 0.0f;
    score_t c = 0.0f;
    score_t d = 0.0f;
    for (uint32_t idx = 0; idx < len; idx++) {
        a += ( x->data[idx] &&  y->data[idx]);
        b += ( x->data[idx] && !y->data[idx]);
        c += (!x->data[idx] &&  y->data[idx]);
        d += (!x->data[idx] && !y->data[idx]);
    }
    score_t apb = a + b;
    score_t apc = a + c;
    score_t bpd = b + d;
    score_t cpd = c + d;
    score_t amd = a * d;
    score_t bmc = b * c;
    /*
        If both of the vectors being compared are all zeros or all
        ones, we define the similarity measure as one, and if one of 
        the two vectors is all zeros and the other all ones, the 
        measure is defined to be zero.
    */
    return ((d == len) || (a == len)) ? 1.0f : (((apb > 0) && (apc > 0) && (bpd > 0) && (cpd > 0)) ? (amd - bmc) / sqrt(apb*apc*bpd*cpd) : -1.0f);
}

/**
 * @brief      bs_rogers_and_tanimoto_similarity_signal(x, y, len)
 *
 * @details    Calculates the Rogers and Tanimoto similarity measure
 *             of two signal vectors; mostly meant for binary matrices
 *
 * @param      x      (signal_t*) pointer to first signal struct
 *             y      (signal_t*) pointer to second signal struct
 *             len    (uint32_t) length of signal vectors
 *
 * @return     (score_t) Rogers and Tanimoto similarity measure between [0,1]
 */

static inline score_t
bs_rogers_and_tanimoto_similarity_signal(signal_t* x, signal_t* y, uint32_t len)
{
    /* 
        We use a-b-c-d 2x2 contingency notation:
        
                 y
               1   0
              -------
          1  | a | b |
        x     -------
          0  | c | d |
              -------
    */
    score_t a = 0.0f;
    score_t b = 0.0f;
    score_t c = 0.0f;
    score_t d = 0.0f;
    for (uint32_t idx = 0; idx < len; idx++) {
        a += ( x->data[idx] &&  y->data[idx]);
        b += ( x->data[idx] && !y->data[idx]);
        c += (!x->data[idx] &&  y->data[idx]);
        d += (!x->data[idx] && !y->data[idx]);
    }
    score_t numerator = a + d;
    score_t denominator = numerator + 2 * (b + c);
    /*
        It is not necessary to deal with special cases here as
        the result for the denominator will always be non-zero,
        so long as the value of len is positive and non-zero.
    */
    return numerator / denominator;
}

/**
 * @brief      bs_normalized_pointwise_mutual_information_signal(x, y, len)
 *
 * @details    Calculates the normalized pointwise mutual information
 *             of two signal vectors; mostly meant for binary matrices
 *
 * @param      x      (signal_t*) pointer to first signal struct
 *             y      (signal_t*) pointer to second signal struct
 *             len    (uint32_t) length of signal vectors
 *
 * @return     (score_t) normalized PMI between [-1,1]
 */

static inline score_t
bs_normalized_pointwise_mutual_information_signal(signal_t* x, signal_t* y, uint32_t len)
{
    /* 
        We use a-b-c-d 2x2 contingency notation:
        
                 y
               1   0
              -------
          1  | a | b |
        x     -------
          0  | c | d |
              -------
    */
    score_t a = 0.0f;
    score_t b = 0.0f;
    score_t c = 0.0f;
    score_t d = 0.0f;
    for (uint32_t idx = 0; idx < len; idx++) {
        a += ( x->data[idx] &&  y->data[idx]);
        b += ( x->data[idx] && !y->data[idx]);
        c += (!x->data[idx] &&  y->data[idx]);
        d += (!x->data[idx] && !y->data[idx]);
    }
    /*
        When two vectors are in concordance, npmi(x, y) = 1
        When they are independent, npmi(x, y) = 0 as the numerator is 0
        When two vectors are in complete discordance, npmi(x, y) = -1
    */
    return ((a == len) || (d == len)) ?  1.0f :
           (((b + c + d) == len) && (b < len) && (c < len)) ? 0.0f :
           ((b == len) || (c == len)) ? -1.0f :
           log((a * len)/((a + b)*(a + c)))/-log(a * len);
}

/**
 * @brief      bs_delete_signal(s)
 *
 * @details    Reset and release memory of provided pointer to signal struct
 *
 * @param      s      (signal_t**) pointer to signal struct pointer to be deleted
 */

void
bs_delete_signal(signal_t** s)
{
    free((*s)->data), (*s)->data = NULL;
    free((*s)->ranks), (*s)->ranks = NULL;
    free(*s), *s = NULL;
}

/**
 * @brief      bs_init_element(chr, start, stop, id, ss, pi, ir, sp, e)
 *
 * @details    Allocates space for element_t* and copies chr, start, stop
 *             and id values to element.
 *
 * @param      chr    (char*) chromosome string 
 *             start  (uint64_t) start coordinate position
 *             stop   (uint64_t) stop coordinate position
 *             id     (char*) id string 
 *             pi     (boolean_t) parse ID string to data pointer
 *             ss     (boolean_t) parse data pointer into or copy summary statistics
 *             ir     (boolean_t) parse data pointer into or copy ranks and related statistics
 *             sp     (signal_t*) signal_t pointer
 *             e      (element_t**) pointer to element_t pointer
 */

void
bs_init_element(char* chr, uint64_t start, uint64_t stop, char* id, boolean_t pi, boolean_t ss, boolean_t ir, signal_t* sp, element_t** e)
{
    element_t *elem = NULL;

    elem = malloc(sizeof(element_t));
    if (!elem) {
        fprintf(stderr, "Error: Could not allocate space for element!\n");
        exit(EXIT_FAILURE);
    }
    elem->chr = NULL;
    if (strlen(chr) > 0) {
        elem->chr = malloc(sizeof(*chr) * strlen(chr) + 1);
        if (!elem->chr) {
            fprintf(stderr, "Error: Could not allocate space for element chromosome!\n");
            exit(EXIT_FAILURE);
        }
        memcpy(elem->chr, chr, strlen(chr) + 1);
    }
    elem->start = start;
    elem->stop = stop;
    elem->id = NULL;
    if (id && (strlen(id) > 0) && pi) {
        elem->id = malloc(sizeof(*id) * strlen(id) + 1);
        if (!elem->id) {
            fprintf(stderr,"Error: Could not allocate space for element id!\n");
            exit(EXIT_FAILURE);
        }
        memcpy(elem->id, id, strlen(id) + 1);
    }
    elem->signal = NULL;
    if (pi) {
        if (!sp) {
            bs_init_signal(elem->id, &(elem->signal), ss, ir);
        }
        else {
            bs_copy_signal(sp, &(elem->signal), ss, ir);
        }
    }
    
    *e = elem;
}

/**
 * @brief      bs_delete_element(e)
 *
 * @details    Releases space for element_t* and components.
 *
 * @param      e      (element_t**) pointer to element_t pointer
 */

void
bs_delete_element(element_t** e)
{
    free((*e)->chr), (*e)->chr = NULL;
    free((*e)->id), (*e)->id = NULL;
    if ((*e)->signal) {
        bs_delete_signal(&((*e)->signal));
        (*e)->signal = NULL;
    }
    free(*e), *e = NULL;
}

/**
 * @brief      bs_push_elem_to_lookup(e, l, pi, ss, ir)
 *
 * @details    Pushes element_t pointer to lookup table.
 *
 * @param      e      (element_t*) element pointer
 *             l      (lookup_t**) pointer to lookup table pointer
 *             pi     (boolean_t) parse ID string
 *             ss     (boolean_t) parse summary statistics from IDs
 *             ir     (boolean_t) parse ranks from IDs
 */

void
bs_push_elem_to_lookup(element_t* e, lookup_t** l, boolean_t pi, boolean_t ss, boolean_t ir)
{
    if ((*l)->capacity == 0) {
        (*l)->capacity++;
        (*l)->elems = malloc(sizeof(element_t *));
    }
    else if ((*l)->nelems >= (*l)->capacity) {
        (*l)->capacity *= 2;
        element_t** new_elems = malloc(sizeof(element_t *) * (*l)->capacity);
        for (uint32_t idx = 0; idx < (*l)->nelems; idx++) {
            bs_init_element((*l)->elems[idx]->chr,
                            (*l)->elems[idx]->start,
                            (*l)->elems[idx]->stop,
                            (*l)->elems[idx]->id,
                            pi,
                            ss,
                            ir,
                            (*l)->elems[idx]->signal,
                            &(new_elems[idx]));
            bs_delete_element(&((*l)->elems[idx]));
        }   
        (*l)->elems = new_elems;     
    }
    uint32_t n = (*l)->nelems;
    (*l)->elems[n] = e;
    (*l)->nelems++;
}

/**
 * @brief      bs_test_normalized_pointwise_mutual_information()
 *
 * @details    Tests calculation and encoding of normalized pointwise
 *             mutual information measure from test vectors
 */
 
void
bs_test_normalized_pointwise_mutual_information()
{
    signal_t* m1 = NULL;
    bs_init_signal((char*) kTestVectorM1, &m1, kFalse, kFalse);
    if (!m1) {
        fprintf(stderr, "Error: Could not allocate space for test (M1) normalized PMI vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* m2 = NULL;
    bs_init_signal((char*) kTestVectorM2, &m2, kFalse, kFalse);
    if (!m2) {
        fprintf(stderr, "Error: Could not allocate space for test (M2) normalized PMI vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* m3 = NULL;
    bs_init_signal((char*) kTestVectorM3, &m3, kFalse, kFalse);
    if (!m3) {
        fprintf(stderr, "Error: Could not allocate space for test (M3) normalized PMI vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* mz = NULL;
    bs_init_signal((char*) kTestVectorMz, &mz, kFalse, kFalse);
    if (!mz) {
        fprintf(stderr, "Error: Could not allocate space for test (Mz) normalized PMI vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* mu = NULL;
    bs_init_signal((char*) kTestVectorMu, &mu, kFalse, kFalse);
    if (!mu) {
        fprintf(stderr, "Error: Could not allocate space for test (Mu) normalized PMI vector!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Comparing M1 vs M2\n---\nM1 -> %s\nM2 -> %s\n---\n", kTestVectorM1, kTestVectorM2);
    if (m1->n != m2->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(m1, stderr);
        bs_print_signal(m2, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_m1m2_score = NAN;
    if ((m1->data_contains_nan == kFalse) && (m2->data_contains_nan == kFalse)) {
        unencoded_observed_m1m2_score = bs_normalized_pointwise_mutual_information_signal(m1, m2, m1->n);
    }
    fprintf(stderr, "Expected - unencoded M1-vs-M2 Normalized PMI measure: %3.6f\n", kNormalizedPointwiseMutualInformationTestM1M2Unencoded);
    fprintf(stderr, "Observed - unencoded M1-vs-M2 Normalized PMI measure: %3.6f\n", unencoded_observed_m1m2_score);
    score_t absolute_diff_unencoded_m1m2_scores = fabs(kNormalizedPointwiseMutualInformationTestM1M2Unencoded - unencoded_observed_m1m2_score);
    assert(absolute_diff_unencoded_m1m2_scores + kEpsilon > 0 && absolute_diff_unencoded_m1m2_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed M1-vs-M2 scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_m1m2_score_byte = bs_encode_score_to_byte(kNormalizedPointwiseMutualInformationTestM1M2Unencoded);
    byte_t encoded_observed_m1m2_score_byte = bs_encode_score_to_byte(unencoded_observed_m1m2_score);
    fprintf(stderr, "Expected - encoded, precomputed M1-vs-M2 Normalized PMI measure: 0x%02x\n", kNormalizedPointwiseMutualInformationTestM1M2EncodedByte);
    fprintf(stderr, "Expected - encoded, computed M1-vs-M2 Normalized PMI measure: 0x%02x\n", encoded_expected_m1m2_score_byte);
    fprintf(stderr, "Observed - encoded, computed M1-vs-M2 Normalized PMI measure: 0x%02x\n", encoded_observed_m1m2_score_byte);
    assert(kNormalizedPointwiseMutualInformationTestM1M2EncodedByte == encoded_expected_m1m2_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed M1-vs-M2 Normalized PMI measures do not differ\n");
    assert(kNormalizedPointwiseMutualInformationTestM1M2EncodedByte == encoded_observed_m1m2_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed M1-vs-M2 Normalized PMI measures do not differ\n");
    assert(encoded_expected_m1m2_score_byte == encoded_observed_m1m2_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed M1-vs-M2 Normalized PMI measures do not differ\n");

    fprintf(stderr, "Comparing M1 vs M3\n---\nM1 -> %s\nM3 -> %s\n---\n", kTestVectorM1, kTestVectorM3);
    if (m1->n != m3->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(m1, stderr);
        bs_print_signal(m3, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_m1m3_score = NAN;
    if ((m1->data_contains_nan == kFalse) && (m3->data_contains_nan == kFalse)) {
        unencoded_observed_m1m3_score = bs_normalized_pointwise_mutual_information_signal(m1, m3, m1->n);
    }
    fprintf(stderr, "Expected - unencoded M1-vs-M3 Normalized PMI measure: %3.6f\n", kNormalizedPointwiseMutualInformationTestM1M3Unencoded);
    fprintf(stderr, "Observed - unencoded M1-vs-M3 Normalized PMI measure: %3.6f\n", unencoded_observed_m1m3_score);
    score_t absolute_diff_unencoded_m1m3_scores = fabs(kNormalizedPointwiseMutualInformationTestM1M3Unencoded - unencoded_observed_m1m3_score);
    assert(absolute_diff_unencoded_m1m3_scores + kEpsilon > 0 && absolute_diff_unencoded_m1m3_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed M1-vs-M3 scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_m1m3_score_byte = bs_encode_score_to_byte(kNormalizedPointwiseMutualInformationTestM1M3Unencoded);
    byte_t encoded_observed_m1m3_score_byte = bs_encode_score_to_byte(unencoded_observed_m1m3_score);
    fprintf(stderr, "Expected - encoded, precomputed M1-vs-M3 Normalized PMI measure: 0x%02x\n", kNormalizedPointwiseMutualInformationTestM1M3EncodedByte);
    fprintf(stderr, "Expected - encoded, computed M1-vs-M3 Normalized PMI measure: 0x%02x\n", encoded_expected_m1m3_score_byte);
    fprintf(stderr, "Observed - encoded, computed M1-vs-M3 Normalized PMI measure: 0x%02x\n", encoded_observed_m1m3_score_byte);
    assert(kNormalizedPointwiseMutualInformationTestM1M3EncodedByte == encoded_expected_m1m3_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed M1-vs-M3 Normalized PMI measures do not differ\n");
    assert(kNormalizedPointwiseMutualInformationTestM1M3EncodedByte == encoded_observed_m1m3_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed M1-vs-M3 Normalized PMI measures do not differ\n");
    assert(encoded_expected_m1m3_score_byte == encoded_observed_m1m3_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed M1-vs-M3 Normalized PMI measures do not differ\n");

    fprintf(stderr, "Comparing Mz vs Mz\n---\nMz -> %s\nMz -> %s\n---\n", kTestVectorMz, kTestVectorMz);
    if (mz->n != mz->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(mz, stderr);
        bs_print_signal(mz, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_mzmz_score = NAN;
    if ((mz->data_contains_nan == kFalse) && (mz->data_contains_nan == kFalse)) {
        unencoded_observed_mzmz_score = bs_normalized_pointwise_mutual_information_signal(mz, mz, mz->n);
    }
    fprintf(stderr, "Expected - unencoded Mz-vs-Mz Normalized PMI measure: %3.6f\n", kNormalizedPointwiseMutualInformationTestMzMzUnencoded);
    fprintf(stderr, "Observed - unencoded Mz-vs-Mz Normalized PMI measure: %3.6f\n", unencoded_observed_mzmz_score);
    score_t absolute_diff_unencoded_mzmz_scores = fabs(kNormalizedPointwiseMutualInformationTestMzMzUnencoded - unencoded_observed_mzmz_score);
    assert(absolute_diff_unencoded_mzmz_scores + kEpsilon > 0 && absolute_diff_unencoded_mzmz_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed Mz-vs-Mz scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_mzmz_score_byte = bs_encode_score_to_byte(kNormalizedPointwiseMutualInformationTestMzMzUnencoded);
    byte_t encoded_observed_mzmz_score_byte = bs_encode_score_to_byte(unencoded_observed_mzmz_score);
    fprintf(stderr, "Expected - encoded, precomputed Mz-vs-Mz Normalized PMI measure: 0x%02x\n", kNormalizedPointwiseMutualInformationTestMzMzEncodedByte);
    fprintf(stderr, "Expected - encoded, computed Mz-vs-Mz Normalized PMI measure: 0x%02x\n", encoded_expected_mzmz_score_byte);
    fprintf(stderr, "Observed - encoded, computed Mz-vs-Mz Normalized PMI measure: 0x%02x\n", encoded_observed_mzmz_score_byte);
    assert(kNormalizedPointwiseMutualInformationTestMzMzEncodedByte == encoded_expected_mzmz_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed Mz-vs-Mz Normalized PMI measures do not differ\n");
    assert(kNormalizedPointwiseMutualInformationTestMzMzEncodedByte == encoded_observed_mzmz_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed Mz-vs-Mz Normalized PMI measures do not differ\n");
    assert(encoded_expected_mzmz_score_byte == encoded_observed_mzmz_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed Mz-vs-Mz Normalized PMI measures do not differ\n");

    fprintf(stderr, "Comparing Mz vs Mu\n---\nMz -> %s\nMu -> %s\n---\n", kTestVectorMz, kTestVectorMu);
    if (mz->n != mu->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(mz, stderr);
        bs_print_signal(mu, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_mzmu_score = NAN;
    if ((mz->data_contains_nan == kFalse) && (mu->data_contains_nan == kFalse)) {
        unencoded_observed_mzmu_score = bs_normalized_pointwise_mutual_information_signal(mz, mu, mz->n);
    }
    fprintf(stderr, "Expected - unencoded Mz-vs-Mu Normalized PMI measure: %3.6f\n", kNormalizedPointwiseMutualInformationTestMzMuUnencoded);
    fprintf(stderr, "Observed - unencoded Mz-vs-Mu Normalized PMI measure: %3.6f\n", unencoded_observed_mzmu_score);
    score_t absolute_diff_unencoded_mzmu_scores = fabs(kNormalizedPointwiseMutualInformationTestMzMuUnencoded - unencoded_observed_mzmu_score);
    assert(absolute_diff_unencoded_mzmu_scores + kEpsilon > 0 && absolute_diff_unencoded_mzmu_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed Mz-vs-Mu scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_mzmu_score_byte = bs_encode_score_to_byte(kNormalizedPointwiseMutualInformationTestMzMuEncoded);
    byte_t encoded_observed_mzmu_score_byte = bs_encode_score_to_byte(unencoded_observed_mzmu_score);
    fprintf(stderr, "Expected - encoded, precomputed Mz-vs-Mu Normalized PMI measure: 0x%02x\n", kNormalizedPointwiseMutualInformationTestMzMuEncodedByte);
    fprintf(stderr, "Expected - encoded, computed Mz-vs-Mu Normalized PMI measure: 0x%02x\n", encoded_expected_mzmu_score_byte);
    fprintf(stderr, "Observed - encoded, computed Mz-vs-Mu Normalized PMI measure: 0x%02x\n", encoded_observed_mzmu_score_byte);
    assert(kNormalizedPointwiseMutualInformationTestMzMuEncodedByte == encoded_expected_mzmu_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed Mz-vs-Mu Normalized PMI measures do not differ\n");
    assert(kNormalizedPointwiseMutualInformationTestMzMuEncodedByte == encoded_observed_mzmu_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed Mz-vs-Mu Normalized PMI measures do not differ\n");
    assert(encoded_expected_mzmu_score_byte == encoded_observed_mzmu_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed Mz-vs-Mu Normalized PMI measures do not differ\n");

    fprintf(stderr, "Comparing Mu vs Mu\n---\nMu -> %s\nMu -> %s\n---\n", kTestVectorMu, kTestVectorMu);
    if (mu->n != mu->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(mu, stderr);
        bs_print_signal(mu, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_mumu_score = NAN;
    if ((mu->data_contains_nan == kFalse) && (mu->data_contains_nan == kFalse)) {
        unencoded_observed_mumu_score = bs_normalized_pointwise_mutual_information_signal(mu, mu, mu->n);
    }
    fprintf(stderr, "Expected - unencoded Mu-vs-Mu Normalized PMI measure: %3.6f\n", kNormalizedPointwiseMutualInformationTestMuMuUnencoded);
    fprintf(stderr, "Observed - unencoded Mu-vs-Mu Normalized PMI measure: %3.6f\n", unencoded_observed_mumu_score);
    score_t absolute_diff_unencoded_mumu_scores = fabs(kNormalizedPointwiseMutualInformationTestMuMuUnencoded - unencoded_observed_mumu_score);
    assert(absolute_diff_unencoded_mumu_scores + kEpsilon > 0 && absolute_diff_unencoded_mumu_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed Mu-vs-Mu scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_mumu_score_byte = bs_encode_score_to_byte(kNormalizedPointwiseMutualInformationTestMuMuEncoded);
    byte_t encoded_observed_mumu_score_byte = bs_encode_score_to_byte(unencoded_observed_mumu_score);
    fprintf(stderr, "Expected - encoded, precomputed Mu-vs-Mu Normalized PMI measure: 0x%02x\n", kNormalizedPointwiseMutualInformationTestMuMuEncodedByte);
    fprintf(stderr, "Expected - encoded, computed Mu-vs-Mu Normalized PMI measure: 0x%02x\n", encoded_expected_mumu_score_byte);
    fprintf(stderr, "Observed - encoded, computed Mu-vs-Mu Normalized PMI measure: 0x%02x\n", encoded_observed_mumu_score_byte);
    assert(kNormalizedPointwiseMutualInformationTestMuMuEncodedByte == encoded_expected_mumu_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed Mu-vs-Mu Normalized PMI measures do not differ\n");
    assert(kNormalizedPointwiseMutualInformationTestMuMuEncodedByte == encoded_observed_mumu_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed Mu-vs-Mu Normalized PMI measures do not differ\n");
    assert(encoded_expected_mumu_score_byte == encoded_observed_mumu_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed Mu-vs-Mu Normalized PMI measures do not differ\n");

    bs_delete_signal(&m1);
    bs_delete_signal(&m2);
    bs_delete_signal(&m3);
    bs_delete_signal(&mz);
    bs_delete_signal(&mu);
}

/**
 * @brief      bs_test_rogers_and_tanimoto_similarity()
 *
 * @details    Tests calculation and encoding of Rogers and
 *             Tanimoto similarity measure from test vectors
 */
 
void
bs_test_rogers_and_tanimoto_similarity()
{
    signal_t* t1 = NULL;
    bs_init_signal((char*) kTestVectorT1, &t1, kFalse, kFalse);
    if (!t1) {
        fprintf(stderr, "Error: Could not allocate space for test (T1) Rogers and Tanimoto similarity vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* t2 = NULL;
    bs_init_signal((char*) kTestVectorT2, &t2, kFalse, kFalse);
    if (!t2) {
        fprintf(stderr, "Error: Could not allocate space for test (T2) Rogers and Tanimoto similarity vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* t3 = NULL;
    bs_init_signal((char*) kTestVectorT3, &t3, kFalse, kFalse);
    if (!t3) {
        fprintf(stderr, "Error: Could not allocate space for test (T3) Rogers and Tanimoto similarity vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* tz = NULL;
    bs_init_signal((char*) kTestVectorTz, &tz, kFalse, kFalse);
    if (!tz) {
        fprintf(stderr, "Error: Could not allocate space for test (Tz) Rogers and Tanimoto similarity vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* tu = NULL;
    bs_init_signal((char*) kTestVectorTu, &tu, kFalse, kFalse);
    if (!tu) {
        fprintf(stderr, "Error: Could not allocate space for test (Tu) Rogers and Tanimoto similarity vector!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Comparing T1 vs T2\n---\nT1 -> %s\nT2 -> %s\n---\n", kTestVectorT1, kTestVectorT2);
    if (t1->n != t2->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(t1, stderr);
        bs_print_signal(t2, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_t1t2_score = NAN;
    if ((t1->data_contains_nan == kFalse) && (t2->data_contains_nan == kFalse)) {
        unencoded_observed_t1t2_score = bs_rogers_and_tanimoto_similarity_signal(t1, t2, t1->n);
    }
    fprintf(stderr, "Expected - unencoded T1-vs-T2 Rogers and Tanimoto index: %3.6f\n", kRogersAndTanimotoSimilarityTestT1T2Unencoded);
    fprintf(stderr, "Observed - unencoded T1-vs-T2 Rogers and Tanimoto index: %3.6f\n", unencoded_observed_t1t2_score);
    score_t absolute_diff_unencoded_t1t2_scores = fabs(kRogersAndTanimotoSimilarityTestT1T2Unencoded - unencoded_observed_t1t2_score);
    assert(absolute_diff_unencoded_t1t2_scores + kEpsilon > 0 && absolute_diff_unencoded_t1t2_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed T1-vs-T2 scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_t1t2_score_byte = bs_encode_score_to_byte(kRogersAndTanimotoSimilarityTestT1T2Unencoded);
    byte_t encoded_observed_t1t2_score_byte = bs_encode_score_to_byte(unencoded_observed_t1t2_score);
    fprintf(stderr, "Expected - encoded, precomputed T1-vs-T2 Rogers and Tanimoto index: 0x%02x\n", kRogersAndTanimotoSimilarityTestT1T2EncodedByte);
    fprintf(stderr, "Expected - encoded, computed T1-vs-T2 Rogers and Tanimoto index: 0x%02x\n", encoded_expected_t1t2_score_byte);
    fprintf(stderr, "Observed - encoded, computed T1-vs-T2 Rogers and Tanimoto index: 0x%02x\n", encoded_observed_t1t2_score_byte);
    assert(kRogersAndTanimotoSimilarityTestT1T2EncodedByte == encoded_expected_t1t2_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed T1-vs-T2 Rogers and Tanimoto indices do not differ\n");
    assert(kRogersAndTanimotoSimilarityTestT1T2EncodedByte == encoded_observed_t1t2_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed T1-vs-T2 Rogers and Tanimoto indices do not differ\n");
    assert(encoded_expected_t1t2_score_byte == encoded_observed_t1t2_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed T1-vs-T2 Rogers and Tanimoto indices do not differ\n");

    fprintf(stderr, "Comparing T1 vs T3\n---\nT1 -> %s\nT3 -> %s\n---\n", kTestVectorT1, kTestVectorT3);
    if (t1->n != t3->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(t1, stderr);
        bs_print_signal(t3, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_t1t3_score = NAN;
    if ((t1->data_contains_nan == kFalse) && (t3->data_contains_nan == kFalse)) {
        unencoded_observed_t1t3_score = bs_rogers_and_tanimoto_similarity_signal(t1, t3, t1->n);
    }
    fprintf(stderr, "Expected - unencoded T1-vs-T3 Rogers and Tanimoto index: %3.6f\n", kRogersAndTanimotoSimilarityTestT1T3Unencoded);
    fprintf(stderr, "Observed - unencoded T1-vs-T3 Rogers and Tanimoto index: %3.6f\n", unencoded_observed_t1t3_score);
    score_t absolute_diff_unencoded_t1t3_scores = fabs(kRogersAndTanimotoSimilarityTestT1T3Unencoded - unencoded_observed_t1t3_score);
    assert(absolute_diff_unencoded_t1t3_scores + kEpsilon > 0 && absolute_diff_unencoded_t1t3_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed T1-vs-T3 scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_t1t3_score_byte = bs_encode_score_to_byte(kRogersAndTanimotoSimilarityTestT1T3Unencoded);
    byte_t encoded_observed_t1t3_score_byte = bs_encode_score_to_byte(unencoded_observed_t1t3_score);
    fprintf(stderr, "Expected - encoded, precomputed T1-vs-T3 Rogers and Tanimoto index: 0x%02x\n", kRogersAndTanimotoSimilarityTestT1T3EncodedByte);
    fprintf(stderr, "Expected - encoded, computed T1-vs-T3 Rogers and Tanimoto index: 0x%02x\n", encoded_expected_t1t3_score_byte);
    fprintf(stderr, "Observed - encoded, computed T1-vs-T3 Rogers and Tanimoto index: 0x%02x\n", encoded_observed_t1t3_score_byte);
    assert(kRogersAndTanimotoSimilarityTestT1T3EncodedByte == encoded_expected_t1t3_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed T1-vs-T3 Rogers and Tanimoto indices do not differ\n");
    assert(kRogersAndTanimotoSimilarityTestT1T3EncodedByte == encoded_observed_t1t3_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed T1-vs-T3 Rogers and Tanimoto indices do not differ\n");
    assert(encoded_expected_t1t3_score_byte == encoded_observed_t1t3_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed T1-vs-T3 Rogers and Tanimoto indices do not differ\n");

    fprintf(stderr, "Comparing Tz vs Tz\n---\nTz -> %s\nTz -> %s\n---\n", kTestVectorTz, kTestVectorTz);
    if (tz->n != tz->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(tz, stderr);
        bs_print_signal(tz, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_tztz_score = NAN;
    if ((tz->data_contains_nan == kFalse) && (tz->data_contains_nan == kFalse)) {
        unencoded_observed_tztz_score = bs_rogers_and_tanimoto_similarity_signal(tz, tz, tz->n);
    }
    fprintf(stderr, "Expected - unencoded Tz-vs-Tz Rogers and Tanimoto index: %3.6f\n", kRogersAndTanimotoSimilarityTestTzTzUnencoded);
    fprintf(stderr, "Observed - unencoded Tz-vs-Tz Rogers and Tanimoto index: %3.6f\n", unencoded_observed_tztz_score);
    score_t absolute_diff_unencoded_tztz_scores = fabs(kRogersAndTanimotoSimilarityTestTzTzUnencoded - unencoded_observed_tztz_score);
    assert(absolute_diff_unencoded_tztz_scores + kEpsilon > 0 && absolute_diff_unencoded_tztz_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed Tz-vs-Tz scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_tztz_score_byte = bs_encode_score_to_byte(kRogersAndTanimotoSimilarityTestTzTzUnencoded);
    byte_t encoded_observed_tztz_score_byte = bs_encode_score_to_byte(unencoded_observed_tztz_score);
    fprintf(stderr, "Expected - encoded, precomputed Tz-vs-Tz Rogers and Tanimoto index: 0x%02x\n", kRogersAndTanimotoSimilarityTestTzTzEncodedByte);
    fprintf(stderr, "Expected - encoded, computed Tz-vs-Tz Rogers and Tanimoto index: 0x%02x\n", encoded_expected_tztz_score_byte);
    fprintf(stderr, "Observed - encoded, computed Tz-vs-Tz Rogers and Tanimoto index: 0x%02x\n", encoded_observed_tztz_score_byte);
    assert(kRogersAndTanimotoSimilarityTestTzTzEncodedByte == encoded_expected_tztz_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed Tz-vs-Tz Rogers and Tanimoto indices do not differ\n");
    assert(kRogersAndTanimotoSimilarityTestTzTzEncodedByte == encoded_observed_tztz_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed Tz-vs-Tz Rogers and Tanimoto indices do not differ\n");
    assert(encoded_expected_tztz_score_byte == encoded_observed_tztz_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed Tz-vs-Tz Rogers and Tanimoto indices do not differ\n");

    fprintf(stderr, "Comparing Tz vs Tu\n---\nTz -> %s\nTu -> %s\n---\n", kTestVectorTz, kTestVectorTu);
    if (tz->n != tu->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(tz, stderr);
        bs_print_signal(tu, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_tztu_score = NAN;
    if ((tz->data_contains_nan == kFalse) && (tu->data_contains_nan == kFalse)) {
        unencoded_observed_tztu_score = bs_rogers_and_tanimoto_similarity_signal(tz, tu, tz->n);
    }
    fprintf(stderr, "Expected - unencoded Tz-vs-Tu Rogers and Tanimoto index: %3.6f\n", kRogersAndTanimotoSimilarityTestTzTuUnencoded);
    fprintf(stderr, "Observed - unencoded Tz-vs-Tu Rogers and Tanimoto index: %3.6f\n", unencoded_observed_tztu_score);
    score_t absolute_diff_unencoded_tztu_scores = fabs(kRogersAndTanimotoSimilarityTestTzTuUnencoded - unencoded_observed_tztu_score);
    assert(absolute_diff_unencoded_tztu_scores + kEpsilon > 0 && absolute_diff_unencoded_tztu_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed Tz-vs-Tu scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_tztu_score_byte = bs_encode_score_to_byte(kRogersAndTanimotoSimilarityTestTzTuEncoded);
    byte_t encoded_observed_tztu_score_byte = bs_encode_score_to_byte(unencoded_observed_tztu_score);
    fprintf(stderr, "Expected - encoded, precomputed Tz-vs-Tu Rogers and Tanimoto index: 0x%02x\n", kRogersAndTanimotoSimilarityTestTzTuEncodedByte);
    fprintf(stderr, "Expected - encoded, computed Tz-vs-Tu Rogers and Tanimoto index: 0x%02x\n", encoded_expected_tztu_score_byte);
    fprintf(stderr, "Observed - encoded, computed Tz-vs-Tu Rogers and Tanimoto index: 0x%02x\n", encoded_observed_tztu_score_byte);
    assert(kRogersAndTanimotoSimilarityTestTzTuEncodedByte == encoded_expected_tztu_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed Tz-vs-Tu Rogers and Tanimoto indices do not differ\n");
    assert(kRogersAndTanimotoSimilarityTestTzTuEncodedByte == encoded_observed_tztu_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed Tz-vs-Tu Rogers and Tanimoto indices do not differ\n");
    assert(encoded_expected_tztu_score_byte == encoded_observed_tztu_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed Tz-vs-Tu Rogers and Tanimoto indices do not differ\n");

    fprintf(stderr, "Comparing Tu vs Tu\n---\nTu -> %s\nTu -> %s\n---\n", kTestVectorTu, kTestVectorTu);
    if (tu->n != tu->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(tu, stderr);
        bs_print_signal(tu, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_tutu_score = NAN;
    if ((tu->data_contains_nan == kFalse) && (tu->data_contains_nan == kFalse)) {
        unencoded_observed_tutu_score = bs_rogers_and_tanimoto_similarity_signal(tu, tu, tu->n);
    }
    fprintf(stderr, "Expected - unencoded Tu-vs-Tu Rogers and Tanimoto index: %3.6f\n", kRogersAndTanimotoSimilarityTestTuTuUnencoded);
    fprintf(stderr, "Observed - unencoded Tu-vs-Tu Rogers and Tanimoto index: %3.6f\n", unencoded_observed_tutu_score);
    score_t absolute_diff_unencoded_tutu_scores = fabs(kRogersAndTanimotoSimilarityTestTuTuUnencoded - unencoded_observed_tutu_score);
    assert(absolute_diff_unencoded_tutu_scores + kEpsilon > 0 && absolute_diff_unencoded_tutu_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed Tu-vs-Tu scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_tutu_score_byte = bs_encode_score_to_byte(kRogersAndTanimotoSimilarityTestTuTuEncoded);
    byte_t encoded_observed_tutu_score_byte = bs_encode_score_to_byte(unencoded_observed_tutu_score);
    fprintf(stderr, "Expected - encoded, precomputed Tu-vs-Tu Rogers and Tanimoto index: 0x%02x\n", kRogersAndTanimotoSimilarityTestTuTuEncodedByte);
    fprintf(stderr, "Expected - encoded, computed Tu-vs-Tu Rogers and Tanimoto index: 0x%02x\n", encoded_expected_tutu_score_byte);
    fprintf(stderr, "Observed - encoded, computed Tu-vs-Tu Rogers and Tanimoto index: 0x%02x\n", encoded_observed_tutu_score_byte);
    assert(kRogersAndTanimotoSimilarityTestTuTuEncodedByte == encoded_expected_tutu_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed Tu-vs-Tu Rogers and Tanimoto indices do not differ\n");
    assert(kRogersAndTanimotoSimilarityTestTuTuEncodedByte == encoded_observed_tutu_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed Tu-vs-Tu Rogers and Tanimoto indices do not differ\n");
    assert(encoded_expected_tutu_score_byte == encoded_observed_tutu_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed Tu-vs-Tu Rogers and Tanimoto indices do not differ\n");

    bs_delete_signal(&t1);
    bs_delete_signal(&t2);
    bs_delete_signal(&t3);
    bs_delete_signal(&tz);
    bs_delete_signal(&tu);
}

/**
 * @brief      bs_test_pearson_phi_similarity()
 *
 * @details    Tests calculation and encoding of Pearson
 *             phi (product-moment correlation) similarity 
 *             from test vectors
 */
 
void
bs_test_pearson_phi_similarity()
{
    signal_t* h1 = NULL;
    bs_init_signal((char*) kTestVectorH1, &h1, kFalse, kFalse);
    if (!h1) {
        fprintf(stderr, "Error: Could not allocate space for test (H1) Pearson phi similarity vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* h2 = NULL;
    bs_init_signal((char*) kTestVectorH2, &h2, kFalse, kFalse);
    if (!h2) {
        fprintf(stderr, "Error: Could not allocate space for test (H2) Pearson phi similarity vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* h3 = NULL;
    bs_init_signal((char*) kTestVectorH3, &h3, kFalse, kFalse);
    if (!h3) {
        fprintf(stderr, "Error: Could not allocate space for test (H3) Pearson phi similarity vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* hz = NULL;
    bs_init_signal((char*) kTestVectorHz, &hz, kFalse, kFalse);
    if (!hz) {
        fprintf(stderr, "Error: Could not allocate space for test (Hz) Pearson phi similarity vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* hu = NULL;
    bs_init_signal((char*) kTestVectorHu, &hu, kFalse, kFalse);
    if (!hu) {
        fprintf(stderr, "Error: Could not allocate space for test (Hu) Pearson phi similarity vector!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Comparing H1 vs H2\n---\nH1 -> %s\nH2 -> %s\n---\n", kTestVectorH1, kTestVectorH2);
    if (h1->n != h2->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(h1, stderr);
        bs_print_signal(h2, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_h1h2_score = NAN;
    if ((h1->data_contains_nan == kFalse) && (h2->data_contains_nan == kFalse)) {
        unencoded_observed_h1h2_score = bs_pearson_phi_similarity_signal(h1, h2, h1->n);
    }
    fprintf(stderr, "Expected - unencoded H1-vs-H2 Pearson phi index: %3.6f\n", kPearsonPhiSimilarityTestH1H2Unencoded);
    fprintf(stderr, "Observed - unencoded H1-vs-H2 Pearson phi index: %3.6f\n", unencoded_observed_h1h2_score);
    score_t absolute_diff_unencoded_h1h2_scores = fabs(kPearsonPhiSimilarityTestH1H2Unencoded - unencoded_observed_h1h2_score);
    assert(absolute_diff_unencoded_h1h2_scores + kEpsilon > 0 && absolute_diff_unencoded_h1h2_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed H1-vs-H2 scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_h1h2_score_byte = bs_encode_score_to_byte(kPearsonPhiSimilarityTestH1H2Unencoded);
    byte_t encoded_observed_h1h2_score_byte = bs_encode_score_to_byte(unencoded_observed_h1h2_score);
    fprintf(stderr, "Expected - encoded, precomputed H1-vs-H2 Pearson phi index: 0x%02x\n", kPearsonPhiSimilarityTestH1H2EncodedByte);
    fprintf(stderr, "Expected - encoded, computed H1-vs-H2 Pearson phi index: 0x%02x\n", encoded_expected_h1h2_score_byte);
    fprintf(stderr, "Observed - encoded, computed H1-vs-H2 Pearson phi index: 0x%02x\n", encoded_observed_h1h2_score_byte);
    assert(kPearsonPhiSimilarityTestH1H2EncodedByte == encoded_expected_h1h2_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed H1-vs-H2 Pearson phi indices do not differ\n");
    assert(kPearsonPhiSimilarityTestH1H2EncodedByte == encoded_observed_h1h2_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed H1-vs-H2 Pearson phi indices do not differ\n");
    assert(encoded_expected_h1h2_score_byte == encoded_observed_h1h2_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed H1-vs-H2 Pearson phi indices do not differ\n");

    fprintf(stderr, "Comparing H1 vs H3\n---\nH1 -> %s\nH3 -> %s\n---\n", kTestVectorH1, kTestVectorH3);
    if (h1->n != h3->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(h1, stderr);
        bs_print_signal(h3, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_h1h3_score = NAN;
    if ((h1->data_contains_nan == kFalse) && (h3->data_contains_nan == kFalse)) {
        unencoded_observed_h1h3_score = bs_pearson_phi_similarity_signal(h1, h3, h1->n);
    }
    fprintf(stderr, "Expected - unencoded H1-vs-H3 Pearson phi index: %3.6f\n", kPearsonPhiSimilarityTestH1H3Unencoded);
    fprintf(stderr, "Observed - unencoded H1-vs-H3 Pearson phi index: %3.6f\n", unencoded_observed_h1h3_score);
    score_t absolute_diff_unencoded_h1h3_scores = fabs(kPearsonPhiSimilarityTestH1H3Unencoded - unencoded_observed_h1h3_score);
    assert(absolute_diff_unencoded_h1h3_scores + kEpsilon > 0 && absolute_diff_unencoded_h1h3_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed H1-vs-H3 scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_h1h3_score_byte = bs_encode_score_to_byte(kPearsonPhiSimilarityTestH1H3Unencoded);
    byte_t encoded_observed_h1h3_score_byte = bs_encode_score_to_byte(unencoded_observed_h1h3_score);
    fprintf(stderr, "Expected - encoded, precomputed H1-vs-H3 Pearson phi index: 0x%02x\n", kPearsonPhiSimilarityTestH1H3EncodedByte);
    fprintf(stderr, "Expected - encoded, computed H1-vs-H3 Pearson phi index: 0x%02x\n", encoded_expected_h1h3_score_byte);
    fprintf(stderr, "Observed - encoded, computed H1-vs-H3 Pearson phi index: 0x%02x\n", encoded_observed_h1h3_score_byte);
    assert(kPearsonPhiSimilarityTestH1H3EncodedByte == encoded_expected_h1h3_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed H1-vs-H3 Pearson phi indices do not differ\n");
    assert(kPearsonPhiSimilarityTestH1H3EncodedByte == encoded_observed_h1h3_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed H1-vs-H3 Pearson phi indices do not differ\n");
    assert(encoded_expected_h1h3_score_byte == encoded_observed_h1h3_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed H1-vs-H3 Pearson phi indices do not differ\n");

    fprintf(stderr, "Comparing Hz vs Hz\n---\nHz -> %s\nHz -> %s\n---\n", kTestVectorHz, kTestVectorHz);
    if (hz->n != hz->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(hz, stderr);
        bs_print_signal(hz, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_hzhz_score = NAN;
    if ((hz->data_contains_nan == kFalse) && (hz->data_contains_nan == kFalse)) {
        unencoded_observed_hzhz_score = bs_pearson_phi_similarity_signal(hz, hz, hz->n);
    }
    fprintf(stderr, "Expected - unencoded Hz-vs-Hz Pearson phi index: %3.6f\n", kPearsonPhiSimilarityTestHzHzUnencoded);
    fprintf(stderr, "Observed - unencoded Hz-vs-Hz Pearson phi index: %3.6f\n", unencoded_observed_hzhz_score);
    score_t absolute_diff_unencoded_hzhz_scores = fabs(kPearsonPhiSimilarityTestHzHzUnencoded - unencoded_observed_hzhz_score);
    assert(absolute_diff_unencoded_hzhz_scores + kEpsilon > 0 && absolute_diff_unencoded_hzhz_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed Hz-vs-Hz scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_hzhz_score_byte = bs_encode_score_to_byte(kPearsonPhiSimilarityTestHzHzUnencoded);
    byte_t encoded_observed_hzhz_score_byte = bs_encode_score_to_byte(unencoded_observed_hzhz_score);
    fprintf(stderr, "Expected - encoded, precomputed Hz-vs-Hz Pearson phi index: 0x%02x\n", kPearsonPhiSimilarityTestHzHzEncodedByte);
    fprintf(stderr, "Expected - encoded, computed Hz-vs-Hz Pearson phi index: 0x%02x\n", encoded_expected_hzhz_score_byte);
    fprintf(stderr, "Observed - encoded, computed Hz-vs-Hz Pearson phi index: 0x%02x\n", encoded_observed_hzhz_score_byte);
    assert(kPearsonPhiSimilarityTestHzHzEncodedByte == encoded_expected_hzhz_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed Hz-vs-Hz Pearson phi indices do not differ\n");
    assert(kPearsonPhiSimilarityTestHzHzEncodedByte == encoded_observed_hzhz_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed Hz-vs-Hz Pearson phi indices do not differ\n");
    assert(encoded_expected_hzhz_score_byte == encoded_observed_hzhz_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed Hz-vs-Hz Pearson phi indices do not differ\n");

    fprintf(stderr, "Comparing Hz vs Hu\n---\nHz -> %s\nHu -> %s\n---\n", kTestVectorHz, kTestVectorHu);
    if (hz->n != hu->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(hz, stderr);
        bs_print_signal(hu, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_hzhu_score = NAN;
    if ((hz->data_contains_nan == kFalse) && (hu->data_contains_nan == kFalse)) {
        unencoded_observed_hzhu_score = bs_pearson_phi_similarity_signal(hz, hu, hz->n);
    }
    fprintf(stderr, "Expected - unencoded Hz-vs-Hu Pearson phi index: %3.6f\n", kPearsonPhiSimilarityTestHzHuUnencoded);
    fprintf(stderr, "Observed - unencoded Hz-vs-Hu Pearson phi index: %3.6f\n", unencoded_observed_hzhu_score);
    score_t absolute_diff_unencoded_hzhu_scores = fabs(kPearsonPhiSimilarityTestHzHuUnencoded - unencoded_observed_hzhu_score);
    assert(absolute_diff_unencoded_hzhu_scores + kEpsilon > 0 && absolute_diff_unencoded_hzhu_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed Hz-vs-Hu scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_hzhu_score_byte = bs_encode_score_to_byte(kPearsonPhiSimilarityTestHzHuEncoded);
    byte_t encoded_observed_hzhu_score_byte = bs_encode_score_to_byte(unencoded_observed_hzhu_score);
    fprintf(stderr, "Expected - encoded, precomputed Hz-vs-Hu Pearson phi index: 0x%02x\n", kPearsonPhiSimilarityTestHzHuEncodedByte);
    fprintf(stderr, "Expected - encoded, computed Hz-vs-Hu Pearson phi index: 0x%02x\n", encoded_expected_hzhu_score_byte);
    fprintf(stderr, "Observed - encoded, computed Hz-vs-Hu Pearson phi index: 0x%02x\n", encoded_observed_hzhu_score_byte);
    assert(kPearsonPhiSimilarityTestHzHuEncodedByte == encoded_expected_hzhu_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed Hz-vs-Hu Pearson phi indices do not differ\n");
    assert(kPearsonPhiSimilarityTestHzHuEncodedByte == encoded_observed_hzhu_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed Hz-vs-Hu Pearson phi indices do not differ\n");
    assert(encoded_expected_hzhu_score_byte == encoded_observed_hzhu_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed Hz-vs-Hu Pearson phi indices do not differ\n");

    fprintf(stderr, "Comparing Hu vs Hu\n---\nHu -> %s\nHu -> %s\n---\n", kTestVectorHu, kTestVectorHu);
    if (hu->n != hu->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(hu, stderr);
        bs_print_signal(hu, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_huhu_score = NAN;
    if ((hu->data_contains_nan == kFalse) && (hu->data_contains_nan == kFalse)) {
        unencoded_observed_huhu_score = bs_pearson_phi_similarity_signal(hu, hu, hu->n);
    }
    fprintf(stderr, "Expected - unencoded Hu-vs-Hu Pearson phi index: %3.6f\n", kPearsonPhiSimilarityTestHuHuUnencoded);
    fprintf(stderr, "Observed - unencoded Hu-vs-Hu Pearson phi index: %3.6f\n", unencoded_observed_huhu_score);
    score_t absolute_diff_unencoded_huhu_scores = fabs(kPearsonPhiSimilarityTestHuHuUnencoded - unencoded_observed_huhu_score);
    assert(absolute_diff_unencoded_huhu_scores + kEpsilon > 0 && absolute_diff_unencoded_huhu_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed Hu-vs-Hu scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_huhu_score_byte = bs_encode_score_to_byte(kPearsonPhiSimilarityTestHuHuEncoded);
    byte_t encoded_observed_huhu_score_byte = bs_encode_score_to_byte(unencoded_observed_huhu_score);
    fprintf(stderr, "Expected - encoded, precomputed Hu-vs-Hu Pearson phi index: 0x%02x\n", kPearsonPhiSimilarityTestHuHuEncodedByte);
    fprintf(stderr, "Expected - encoded, computed Hu-vs-Hu Pearson phi index: 0x%02x\n", encoded_expected_huhu_score_byte);
    fprintf(stderr, "Observed - encoded, computed Hu-vs-Hu Pearson phi index: 0x%02x\n", encoded_observed_huhu_score_byte);
    assert(kPearsonPhiSimilarityTestHuHuEncodedByte == encoded_expected_huhu_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed Hu-vs-Hu Pearson phi indices do not differ\n");
    assert(kPearsonPhiSimilarityTestHuHuEncodedByte == encoded_observed_huhu_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed Hu-vs-Hu Pearson phi indices do not differ\n");
    assert(encoded_expected_huhu_score_byte == encoded_observed_huhu_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed Hu-vs-Hu Pearson phi indices do not differ\n");

    bs_delete_signal(&h1);
    bs_delete_signal(&h2);
    bs_delete_signal(&h3);
    bs_delete_signal(&hz);
    bs_delete_signal(&hu);
}

/**
 * @brief      bs_test_ochiai_similarity()
 *
 * @details    Tests calculation and encoding of Ochiai
 *             similarity from test vectors
 */
 
void
bs_test_ochiai_similarity()
{
    signal_t* o1 = NULL;
    bs_init_signal((char*) kTestVectorO1, &o1, kFalse, kFalse);
    if (!o1) {
        fprintf(stderr, "Error: Could not allocate space for test (O1) Ochiai similarity vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* o2 = NULL;
    bs_init_signal((char*) kTestVectorO2, &o2, kFalse, kFalse);
    if (!o2) {
        fprintf(stderr, "Error: Could not allocate space for test (O2) Ochiai similarity vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* o3 = NULL;
    bs_init_signal((char*) kTestVectorO3, &o3, kFalse, kFalse);
    if (!o3) {
        fprintf(stderr, "Error: Could not allocate space for test (O3) Ochiai similarity vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* oz = NULL;
    bs_init_signal((char*) kTestVectorOz, &oz, kFalse, kFalse);
    if (!oz) {
        fprintf(stderr, "Error: Could not allocate space for test (Oz) Ochiai similarity vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* ou = NULL;
    bs_init_signal((char*) kTestVectorOu, &ou, kFalse, kFalse);
    if (!ou) {
        fprintf(stderr, "Error: Could not allocate space for test (Ou) Ochiai similarity vector!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Comparing O1 vs O2\n---\nO1 -> %s\nO2 -> %s\n---\n", kTestVectorO1, kTestVectorO2);
    if (o1->n != o2->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(o1, stderr);
        bs_print_signal(o2, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_o1o2_score = NAN;
    if ((o1->data_contains_nan == kFalse) && (o2->data_contains_nan == kFalse)) {
        unencoded_observed_o1o2_score = bs_ochiai_similarity_signal(o1, o2, o1->n);
    }
    fprintf(stderr, "Expected - unencoded O1-vs-O2 Ochiai index: %3.6f\n", kOchiaiSimilarityTestO1O2Unencoded);
    fprintf(stderr, "Observed - unencoded O1-vs-O2 Ochiai index: %3.6f\n", unencoded_observed_o1o2_score);
    score_t absolute_diff_unencoded_o1o2_scores = fabs(kOchiaiSimilarityTestO1O2Unencoded - unencoded_observed_o1o2_score);
    assert(absolute_diff_unencoded_o1o2_scores + kEpsilon > 0 && absolute_diff_unencoded_o1o2_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed O1-vs-O2 scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_o1o2_score_byte = bs_encode_score_to_byte(kOchiaiSimilarityTestO1O2Unencoded);
    byte_t encoded_observed_o1o2_score_byte = bs_encode_score_to_byte(unencoded_observed_o1o2_score);
    fprintf(stderr, "Expected - encoded, precomputed O1-vs-O2 Ochiai index: 0x%02x\n", kOchiaiSimilarityTestO1O2EncodedByte);
    fprintf(stderr, "Expected - encoded, computed O1-vs-O2 Ochiai index: 0x%02x\n", encoded_expected_o1o2_score_byte);
    fprintf(stderr, "Observed - encoded, computed O1-vs-O2 Ochiai index: 0x%02x\n", encoded_observed_o1o2_score_byte);
    assert(kOchiaiSimilarityTestO1O2EncodedByte == encoded_expected_o1o2_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed O1-vs-O2 Ochiai indices do not differ\n");
    assert(kOchiaiSimilarityTestO1O2EncodedByte == encoded_observed_o1o2_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed O1-vs-O2 Ochiai indices do not differ\n");
    assert(encoded_expected_o1o2_score_byte == encoded_observed_o1o2_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed O1-vs-O2 Ochiai indices do not differ\n");

    fprintf(stderr, "Comparing O1 vs O3\n---\nO1 -> %s\nO3 -> %s\n---\n", kTestVectorO1, kTestVectorO3);
    if (o1->n != o3->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(o1, stderr);
        bs_print_signal(o3, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_o1o3_score = NAN;
    if ((o1->data_contains_nan == kFalse) && (o3->data_contains_nan == kFalse)) {
        unencoded_observed_o1o3_score = bs_ochiai_similarity_signal(o1, o3, o1->n);
    }
    fprintf(stderr, "Expected - unencoded O1-vs-O3 Ochiai index: %3.6f\n", kOchiaiSimilarityTestO1O3Unencoded);
    fprintf(stderr, "Observed - unencoded O1-vs-O3 Ochiai index: %3.6f\n", unencoded_observed_o1o3_score);
    score_t absolute_diff_unencoded_o1o3_scores = fabs(kOchiaiSimilarityTestO1O3Unencoded - unencoded_observed_o1o3_score);
    assert(absolute_diff_unencoded_o1o3_scores + kEpsilon > 0 && absolute_diff_unencoded_o1o3_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed O1-vs-O3 scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_o1o3_score_byte = bs_encode_score_to_byte(kOchiaiSimilarityTestO1O3Unencoded);
    byte_t encoded_observed_o1o3_score_byte = bs_encode_score_to_byte(unencoded_observed_o1o3_score);
    fprintf(stderr, "Expected - encoded, precomputed O1-vs-O3 Ochiai index: 0x%02x\n", kOchiaiSimilarityTestO1O3EncodedByte);
    fprintf(stderr, "Expected - encoded, computed O1-vs-O3 Ochiai index: 0x%02x\n", encoded_expected_o1o3_score_byte);
    fprintf(stderr, "Observed - encoded, computed O1-vs-O3 Ochiai index: 0x%02x\n", encoded_observed_o1o3_score_byte);
    assert(kOchiaiSimilarityTestO1O3EncodedByte == encoded_expected_o1o3_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed O1-vs-O3 Ochiai indices do not differ\n");
    assert(kOchiaiSimilarityTestO1O3EncodedByte == encoded_observed_o1o3_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed O1-vs-O3 Ochiai indices do not differ\n");
    assert(encoded_expected_o1o3_score_byte == encoded_observed_o1o3_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed O1-vs-O3 Ochiai indices do not differ\n");

    fprintf(stderr, "Comparing Oz vs Oz\n---\nOz -> %s\nOz -> %s\n---\n", kTestVectorOz, kTestVectorOz);
    if (oz->n != oz->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(oz, stderr);
        bs_print_signal(oz, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_ozoz_score = NAN;
    if ((oz->data_contains_nan == kFalse) && (oz->data_contains_nan == kFalse)) {
        unencoded_observed_ozoz_score = bs_ochiai_similarity_signal(oz, oz, oz->n);
    }
    fprintf(stderr, "Expected - unencoded Oz-vs-Oz Ochiai index: %3.6f\n", kOchiaiSimilarityTestOzOzUnencoded);
    fprintf(stderr, "Observed - unencoded Oz-vs-Oz Ochiai index: %3.6f\n", unencoded_observed_ozoz_score);
    score_t absolute_diff_unencoded_ozoz_scores = fabs(kOchiaiSimilarityTestOzOzUnencoded - unencoded_observed_ozoz_score);
    assert(absolute_diff_unencoded_ozoz_scores + kEpsilon > 0 && absolute_diff_unencoded_ozoz_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed Oz-vs-Oz scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_ozoz_score_byte = bs_encode_score_to_byte(kOchiaiSimilarityTestOzOzUnencoded);
    byte_t encoded_observed_ozoz_score_byte = bs_encode_score_to_byte(unencoded_observed_ozoz_score);
    fprintf(stderr, "Expected - encoded, precomputed Oz-vs-Oz Ochiai index: 0x%02x\n", kOchiaiSimilarityTestOzOzEncodedByte);
    fprintf(stderr, "Expected - encoded, computed Oz-vs-Oz Ochiai index: 0x%02x\n", encoded_expected_ozoz_score_byte);
    fprintf(stderr, "Observed - encoded, computed Oz-vs-Oz Ochiai index: 0x%02x\n", encoded_observed_ozoz_score_byte);
    assert(kOchiaiSimilarityTestOzOzEncodedByte == encoded_expected_ozoz_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed Oz-vs-Oz Ochiai indices do not differ\n");
    assert(kOchiaiSimilarityTestOzOzEncodedByte == encoded_observed_ozoz_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed Oz-vs-Oz Ochiai indices do not differ\n");
    assert(encoded_expected_ozoz_score_byte == encoded_observed_ozoz_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed Oz-vs-Oz Ochiai indices do not differ\n");

    fprintf(stderr, "Comparing Oz vs Ou\n---\nOz -> %s\nOu -> %s\n---\n", kTestVectorOz, kTestVectorOu);
    if (oz->n != ou->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(oz, stderr);
        bs_print_signal(ou, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_ozou_score = NAN;
    if ((oz->data_contains_nan == kFalse) && (ou->data_contains_nan == kFalse)) {
        unencoded_observed_ozou_score = bs_ochiai_similarity_signal(oz, ou, oz->n);
    }
    fprintf(stderr, "Expected - unencoded Oz-vs-Ou Ochiai index: %3.6f\n", kOchiaiSimilarityTestOzOuUnencoded);
    fprintf(stderr, "Observed - unencoded Oz-vs-Ou Ochiai index: %3.6f\n", unencoded_observed_ozou_score);
    score_t absolute_diff_unencoded_ozou_scores = fabs(kOchiaiSimilarityTestOzOuUnencoded - unencoded_observed_ozou_score);
    assert(absolute_diff_unencoded_ozou_scores + kEpsilon > 0 && absolute_diff_unencoded_ozou_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed Oz-vs-Ou scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_ozou_score_byte = bs_encode_score_to_byte(kOchiaiSimilarityTestOzOuUnencoded);
    byte_t encoded_observed_ozou_score_byte = bs_encode_score_to_byte(unencoded_observed_ozou_score);
    fprintf(stderr, "Expected - encoded, precomputed Oz-vs-Ou Ochiai index: 0x%02x\n", kOchiaiSimilarityTestOzOuEncodedByte);
    fprintf(stderr, "Expected - encoded, computed Oz-vs-Ou Ochiai index: 0x%02x\n", encoded_expected_ozou_score_byte);
    fprintf(stderr, "Observed - encoded, computed Oz-vs-Ou Ochiai index: 0x%02x\n", encoded_observed_ozou_score_byte);
    assert(kOchiaiSimilarityTestOzOuEncodedByte == encoded_expected_ozou_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed Oz-vs-Ou Ochiai indices do not differ\n");
    assert(kOchiaiSimilarityTestOzOuEncodedByte == encoded_observed_ozou_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed Oz-vs-Ou Ochiai indices do not differ\n");
    assert(encoded_expected_ozou_score_byte == encoded_observed_ozou_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed Oz-vs-Ou Ochiai indices do not differ\n");

    fprintf(stderr, "Comparing Ou vs Ou\n---\nOu -> %s\nOu -> %s\n---\n", kTestVectorOu, kTestVectorOu);
    if (ou->n != ou->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(ou, stderr);
        bs_print_signal(ou, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_ouou_score = NAN;
    if ((ou->data_contains_nan == kFalse) && (ou->data_contains_nan == kFalse)) {
        unencoded_observed_ouou_score = bs_ochiai_similarity_signal(ou, ou, ou->n);
    }
    fprintf(stderr, "Expected - unencoded Ou-vs-Ou Ochiai index: %3.6f\n", kOchiaiSimilarityTestOuOuUnencoded);
    fprintf(stderr, "Observed - unencoded Ou-vs-Ou Ochiai index: %3.6f\n", unencoded_observed_ouou_score);
    score_t absolute_diff_unencoded_ouou_scores = fabs(kOchiaiSimilarityTestOuOuUnencoded - unencoded_observed_ouou_score);
    assert(absolute_diff_unencoded_ouou_scores + kEpsilon > 0 && absolute_diff_unencoded_ouou_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed Ou-vs-Ou scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_ouou_score_byte = bs_encode_score_to_byte(kOchiaiSimilarityTestOuOuUnencoded);
    byte_t encoded_observed_ouou_score_byte = bs_encode_score_to_byte(unencoded_observed_ouou_score);
    fprintf(stderr, "Expected - encoded, precomputed Ou-vs-Ou Ochiai index: 0x%02x\n", kOchiaiSimilarityTestOuOuEncodedByte);
    fprintf(stderr, "Expected - encoded, computed Ou-vs-Ou Ochiai index: 0x%02x\n", encoded_expected_ouou_score_byte);
    fprintf(stderr, "Observed - encoded, computed Ou-vs-Ou Ochiai index: 0x%02x\n", encoded_observed_ouou_score_byte);
    assert(kOchiaiSimilarityTestOuOuEncodedByte == encoded_expected_ouou_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed Ou-vs-Ou Ochiai indices do not differ\n");
    assert(kOchiaiSimilarityTestOuOuEncodedByte == encoded_observed_ouou_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed Ou-vs-Ou Ochiai indices do not differ\n");
    assert(encoded_expected_ouou_score_byte == encoded_observed_ouou_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed Ou-vs-Ou Ochiai indices do not differ\n");

    bs_delete_signal(&o1);
    bs_delete_signal(&o2);
    bs_delete_signal(&o3);
    bs_delete_signal(&oz);
    bs_delete_signal(&ou);
}

/**
 * @brief      bs_test_jaccard_index()
 *
 * @details    Tests calculation and encoding of Jaccard 
 *             index from test vectors
 */

void
bs_test_jaccard_index()
{
    signal_t* j1 = NULL;
    bs_init_signal((char*) kTestVectorJ1, &j1, kFalse, kFalse);
    if (!j1) {
        fprintf(stderr, "Error: Could not allocate space for test (J1) Jaccard index vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* j2 = NULL;
    bs_init_signal((char*) kTestVectorJ2, &j2, kFalse, kFalse);
    if (!j2) {
        fprintf(stderr, "Error: Could not allocate space for test (J2) Jaccard index vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* j3 = NULL;
    bs_init_signal((char*) kTestVectorJ3, &j3, kFalse, kFalse);
    if (!j3) {
        fprintf(stderr, "Error: Could not allocate space for test (J3) Jaccard index vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* jz = NULL;
    bs_init_signal((char*) kTestVectorJz, &jz, kFalse, kFalse);
    if (!jz) {
        fprintf(stderr, "Error: Could not allocate space for test (Jz) Jaccard index vector!\n");
        exit(EXIT_FAILURE);
    }

    signal_t* ju = NULL;
    bs_init_signal((char*) kTestVectorJu, &ju, kFalse, kFalse);
    if (!ju) {
        fprintf(stderr, "Error: Could not allocate space for test (Ju) Jaccard index vector!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Comparing J1 vs J2\n---\nJ1 -> %s\nJ2 -> %s\n---\n", kTestVectorJ1, kTestVectorJ2);
    if (j1->n != j2->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(j1, stderr);
        bs_print_signal(j2, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_j1j2_score = NAN;
    if ((j1->data_contains_nan == kFalse) && (j2->data_contains_nan == kFalse)) {
        unencoded_observed_j1j2_score = bs_jaccard_index_signal(j1, j2, j1->n);
    }
    fprintf(stderr, "Expected - unencoded J1-vs-J2 Jaccard index: %3.6f\n", kJaccardIndexTestJ1J2Unencoded);
    fprintf(stderr, "Observed - unencoded J1-vs-J2 Jaccard index: %3.6f\n", unencoded_observed_j1j2_score);
    score_t absolute_diff_unencoded_j1j2_scores = fabs(kJaccardIndexTestJ1J2Unencoded - unencoded_observed_j1j2_score);
    assert(absolute_diff_unencoded_j1j2_scores + kEpsilon > 0 && absolute_diff_unencoded_j1j2_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed J1-vs-J2 scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_j1j2_score_byte = bs_encode_score_to_byte(kJaccardIndexTestJ1J2Unencoded);
    byte_t encoded_observed_j1j2_score_byte = bs_encode_score_to_byte(unencoded_observed_j1j2_score);
    fprintf(stderr, "Expected - encoded, precomputed J1-vs-J2 Jaccard index: 0x%02x\n", kJaccardIndexTestJ1J2EncodedByte);
    fprintf(stderr, "Expected - encoded, computed J1-vs-J2 Jaccard index: 0x%02x\n", encoded_expected_j1j2_score_byte);
    fprintf(stderr, "Observed - encoded, computed J1-vs-J2 Jaccard index: 0x%02x\n", encoded_observed_j1j2_score_byte);
    assert(kJaccardIndexTestJ1J2EncodedByte == encoded_expected_j1j2_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed J1-vs-J2 Jaccard indices do not differ\n");
    assert(kJaccardIndexTestJ1J2EncodedByte == encoded_observed_j1j2_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed J1-vs-J2 Jaccard indices do not differ\n");
    assert(encoded_expected_j1j2_score_byte == encoded_observed_j1j2_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed J1-vs-J2 Jaccard indices do not differ\n");

    fprintf(stderr, "Comparing J1 vs J3\n---\nJ1 -> %s\nJ3 -> %s\n---\n", kTestVectorJ1, kTestVectorJ3);
    if (j1->n != j3->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(j1, stderr);
        bs_print_signal(j3, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_j1j3_score = NAN;
    if ((j1->data_contains_nan == kFalse) && (j3->data_contains_nan == kFalse)) {
        unencoded_observed_j1j3_score = bs_jaccard_index_signal(j1, j3, j1->n);
    }
    fprintf(stderr, "Expected - unencoded J1-vs-J3 Jaccard index: %3.6f\n", kJaccardIndexTestJ1J3Unencoded);
    fprintf(stderr, "Observed - unencoded J1-vs-J3 Jaccard index: %3.6f\n", unencoded_observed_j1j3_score);
    score_t absolute_diff_unencoded_j1j3_scores = fabs(kJaccardIndexTestJ1J3Unencoded - unencoded_observed_j1j3_score);
    assert(absolute_diff_unencoded_j1j3_scores + kEpsilon > 0 && absolute_diff_unencoded_j1j3_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed J1-vs-J3 scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_j1j3_score_byte = bs_encode_score_to_byte(kJaccardIndexTestJ1J3Unencoded);
    byte_t encoded_observed_j1j3_score_byte = bs_encode_score_to_byte(unencoded_observed_j1j3_score);
    fprintf(stderr, "Expected - encoded, precomputed J1-vs-J3 Jaccard index: 0x%02x\n", kJaccardIndexTestJ1J3EncodedByte);
    fprintf(stderr, "Expected - encoded, computed J1-vs-J3 Jaccard index: 0x%02x\n", encoded_expected_j1j3_score_byte);
    fprintf(stderr, "Observed - encoded, computed J1-vs-J3 Jaccard index: 0x%02x\n", encoded_observed_j1j3_score_byte);
    assert(kJaccardIndexTestJ1J3EncodedByte == encoded_expected_j1j3_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed J1-vs-J3 Jaccard indices do not differ\n");
    assert(kJaccardIndexTestJ1J3EncodedByte == encoded_observed_j1j3_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed J1-vs-J3 Jaccard indices do not differ\n");
    assert(encoded_expected_j1j3_score_byte == encoded_observed_j1j3_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed J1-vs-J3 Jaccard indices do not differ\n");

    fprintf(stderr, "Comparing Jz vs Jz\n---\nJz -> %s\nJz -> %s\n---\n", kTestVectorJz, kTestVectorJz);
    if (jz->n != jz->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(jz, stderr);
        bs_print_signal(jz, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_jzjz_score = NAN;
    if ((jz->data_contains_nan == kFalse) && (jz->data_contains_nan == kFalse)) {
        unencoded_observed_jzjz_score = bs_jaccard_index_signal(jz, jz, jz->n);
    }
    fprintf(stderr, "Expected - unencoded Jz-vs-Jz Jaccard index: %3.6f\n", kJaccardIndexTestJzJzUnencoded);
    fprintf(stderr, "Observed - unencoded Jz-vs-Jz Jaccard index: %3.6f\n", unencoded_observed_jzjz_score);
    score_t absolute_diff_unencoded_jzjz_scores = fabs(kJaccardIndexTestJzJzUnencoded - unencoded_observed_jzjz_score);
    assert(absolute_diff_unencoded_jzjz_scores + kEpsilon > 0 && absolute_diff_unencoded_jzjz_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed Jz-vs-Jz scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_jzjz_score_byte = bs_encode_score_to_byte(kJaccardIndexTestJzJzUnencoded);
    byte_t encoded_observed_jzjz_score_byte = bs_encode_score_to_byte(unencoded_observed_jzjz_score);
    fprintf(stderr, "Expected - encoded, precomputed Jz-vs-Jz Jaccard index: 0x%02x\n", kJaccardIndexTestJzJzEncodedByte);
    fprintf(stderr, "Expected - encoded, computed Jz-vs-Jz Jaccard index: 0x%02x\n", encoded_expected_jzjz_score_byte);
    fprintf(stderr, "Observed - encoded, computed Jz-vs-Jz Jaccard index: 0x%02x\n", encoded_observed_jzjz_score_byte);
    assert(kJaccardIndexTestJzJzEncodedByte == encoded_expected_jzjz_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed Jz-vs-Jz Jaccard indices do not differ\n");
    assert(kJaccardIndexTestJzJzEncodedByte == encoded_observed_jzjz_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed Jz-vs-Jz Jaccard indices do not differ\n");
    assert(encoded_expected_jzjz_score_byte == encoded_observed_jzjz_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed Jz-vs-Jz Jaccard indices do not differ\n");

    fprintf(stderr, "Comparing Jz vs Ju\n---\nJz -> %s\nJu -> %s\n---\n", kTestVectorJz, kTestVectorJu);
    if (jz->n != ju->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(jz, stderr);
        bs_print_signal(ju, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_jzju_score = NAN;
    if ((jz->data_contains_nan == kFalse) && (ju->data_contains_nan == kFalse)) {
        unencoded_observed_jzju_score = bs_jaccard_index_signal(jz, ju, jz->n);
    }
    fprintf(stderr, "Expected - unencoded Jz-vs-Ju Jaccard index: %3.6f\n", kJaccardIndexTestJzJuUnencoded);
    fprintf(stderr, "Observed - unencoded Jz-vs-Ju Jaccard index: %3.6f\n", unencoded_observed_jzju_score);
    score_t absolute_diff_unencoded_jzju_scores = fabs(kJaccardIndexTestJzJuUnencoded - unencoded_observed_jzju_score);
    assert(absolute_diff_unencoded_jzju_scores + kEpsilon > 0 && absolute_diff_unencoded_jzju_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed Jz-vs-Ju scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_jzju_score_byte = bs_encode_score_to_byte(kJaccardIndexTestJzJuUnencoded);
    byte_t encoded_observed_jzju_score_byte = bs_encode_score_to_byte(unencoded_observed_jzju_score);
    fprintf(stderr, "Expected - encoded, precomputed Jz-vs-Ju Jaccard index: 0x%02x\n", kJaccardIndexTestJzJuEncodedByte);
    fprintf(stderr, "Expected - encoded, computed Jz-vs-Ju Jaccard index: 0x%02x\n", encoded_expected_jzju_score_byte);
    fprintf(stderr, "Observed - encoded, computed Jz-vs-Ju Jaccard index: 0x%02x\n", encoded_observed_jzju_score_byte);
    assert(kJaccardIndexTestJzJuEncodedByte == encoded_expected_jzju_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed Jz-vs-Ju Jaccard indices do not differ\n");
    assert(kJaccardIndexTestJzJuEncodedByte == encoded_observed_jzju_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed Jz-vs-Ju Jaccard indices do not differ\n");
    assert(encoded_expected_jzju_score_byte == encoded_observed_jzju_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed Jz-vs-Ju Jaccard indices do not differ\n");

    fprintf(stderr, "Comparing Ju vs Ju\n---\nJu -> %s\nJu -> %s\n---\n", kTestVectorJu, kTestVectorJu);
    if (ju->n != ju->n) {
        fprintf(stderr, "Error: Vectors being compared are of unequal length!\n");
        bs_print_signal(ju, stderr);
        bs_print_signal(ju, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_juju_score = NAN;
    if ((ju->data_contains_nan == kFalse) && (ju->data_contains_nan == kFalse)) {
        unencoded_observed_juju_score = bs_jaccard_index_signal(ju, ju, ju->n);
    }
    fprintf(stderr, "Expected - unencoded Ju-vs-Ju Jaccard index: %3.6f\n", kJaccardIndexTestJuJuUnencoded);
    fprintf(stderr, "Observed - unencoded Ju-vs-Ju Jaccard index: %3.6f\n", unencoded_observed_juju_score);
    score_t absolute_diff_unencoded_juju_scores = fabs(kJaccardIndexTestJuJuUnencoded - unencoded_observed_juju_score);
    assert(absolute_diff_unencoded_juju_scores + kEpsilon > 0 && absolute_diff_unencoded_juju_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed Ju-vs-Ju scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_juju_score_byte = bs_encode_score_to_byte(kJaccardIndexTestJuJuUnencoded);
    byte_t encoded_observed_juju_score_byte = bs_encode_score_to_byte(unencoded_observed_juju_score);
    fprintf(stderr, "Expected - encoded, precomputed Ju-vs-Ju Jaccard index: 0x%02x\n", kJaccardIndexTestJuJuEncodedByte);
    fprintf(stderr, "Expected - encoded, computed Ju-vs-Ju Jaccard index: 0x%02x\n", encoded_expected_juju_score_byte);
    fprintf(stderr, "Observed - encoded, computed Ju-vs-Ju Jaccard index: 0x%02x\n", encoded_observed_juju_score_byte);
    assert(kJaccardIndexTestJuJuEncodedByte == encoded_expected_juju_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed Ju-vs-Ju Jaccard indices do not differ\n");
    assert(kJaccardIndexTestJuJuEncodedByte == encoded_observed_juju_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed Ju-vs-Ju Jaccard indices do not differ\n");
    assert(encoded_expected_juju_score_byte == encoded_observed_juju_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed Ju-vs-Ju Jaccard indices do not differ\n");

    bs_delete_signal(&j1);
    bs_delete_signal(&j2);
    bs_delete_signal(&j3);
    bs_delete_signal(&jz);
    bs_delete_signal(&ju);
}

/**
 * @brief      bs_test_spearman_rho()
 *
 * @details    Tests calculation and encoding of Spearman's 
 *             rho score from test vectors and their ranks
 */

void
bs_test_spearman_rho()
{
    signal_t* x = NULL;
    bs_init_signal((char*) kTestVectorX, &x, kTrue, kTrue);
    if (!x) {
        fprintf(stderr, "Error: Could not allocate space for test (X) Spearman's rho vector!\n");
        exit(EXIT_FAILURE);
    }
    signal_t* y = NULL;
    bs_init_signal((char*) kTestVectorY, &y, kTrue, kTrue);
    if (!y) {
        fprintf(stderr, "Error: Could not allocate space for test (Y) Spearman's rho vector!\n");
        exit(EXIT_FAILURE);
    }
    bs_print_signal(x, stdout);
    bs_print_signal(y, stdout);

    fprintf(stderr, "Comparing XY\n---\nX -> %s\nY -> %s\n---\n", kTestVectorX, kTestVectorY);
    if (x->n != y->n) {
        fprintf(stderr, "Error: Vectors being correlated are of unequal length!\n");
        bs_print_signal(x, stderr);
        bs_print_signal(y, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_xy_score = NAN;
    if ((x->sd_ranks != 0.0f) && (y->sd_ranks != 0.0f)) {
        unencoded_observed_xy_score = bs_spearman_rho_signal_v1(x, y, x->n);
    }
    fprintf(stderr, "Expected - unencoded XY Spearman's rho score: %3.6f\n", kSpearmanRhoTestXYCorrelationUnencoded);
    fprintf(stderr, "Observed - unencoded XY Spearman's rho score: %3.6f\n", unencoded_observed_xy_score);
    score_t absolute_diff_unencoded_xy_scores = fabs(kSpearmanRhoTestXYCorrelationUnencoded - unencoded_observed_xy_score);
    assert(absolute_diff_unencoded_xy_scores + kEpsilon > 0 && absolute_diff_unencoded_xy_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed XY scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_xy_score_byte = bs_encode_score_to_byte(kSpearmanRhoTestXYCorrelationUnencoded);
    byte_t encoded_observed_xy_score_byte = bs_encode_score_to_byte(unencoded_observed_xy_score);
    fprintf(stderr, "Expected - encoded, precomputed XY Spearman's rho score: 0x%02x\n", kSpearmanRhoTestXYCorrelationEncodedByte);
    fprintf(stderr, "Expected - encoded, computed XY Spearman's rho score: 0x%02x\n", encoded_expected_xy_score_byte);
    fprintf(stderr, "Observed - encoded, computed XY Spearman's rho score: 0x%02x\n", encoded_observed_xy_score_byte);
    assert(kSpearmanRhoTestXYCorrelationEncodedByte == encoded_expected_xy_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed XY scores do not differ\n");
    assert(kSpearmanRhoTestXYCorrelationEncodedByte == encoded_observed_xy_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed XY scores do not differ\n");
    assert(encoded_expected_xy_score_byte == encoded_observed_xy_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed XY scores do not differ\n");

    signal_t* z = NULL;
    bs_init_signal((char*) kTestVectorZ, &z, kTrue, kTrue);
    if (!z) {
        fprintf(stderr, "Error: Could not allocate space for test (Z) Spearman's rho vector!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Comparing XZ\n---\nX -> %s\nZ -> %s\n---\n", kTestVectorX, kTestVectorZ);
    if (x->n != z->n) {
        fprintf(stderr, "Error: Vectors being correlated are of unequal length!\n");
        bs_print_signal(x, stderr);
        bs_print_signal(z, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_xz_score = NAN;
    if ((x->sd_ranks != 0.0f) && (z->sd_ranks != 0.0f)) {
        unencoded_observed_xz_score = bs_spearman_rho_signal_v1(x, z, x->n);
    } 
    fprintf(stderr, "Expected - unencoded XZ Spearman's rho score: %3.6f\n", kSpearmanRhoTestXZCorrelationUnencoded);
    fprintf(stderr, "Observed - unencoded XZ Spearman's rho score: %3.6f\n", unencoded_observed_xz_score);
    byte_t encoded_expected_xz_score_byte = bs_encode_score_to_byte(kSpearmanRhoTestXZCorrelationUnencoded);
    byte_t encoded_observed_xz_score_byte = bs_encode_score_to_byte(unencoded_observed_xz_score);
    fprintf(stderr, "Expected - encoded, precomputed XZ Spearman's rho score: 0x%02x\n", kSpearmanRhoTestXZCorrelationEncodedByte);
    fprintf(stderr, "Expected - encoded, computed XZ Spearman's rho score: 0x%02x\n", encoded_expected_xz_score_byte);
    fprintf(stderr, "Observed - encoded, computed XZ Spearman's rho score: 0x%02x\n", encoded_observed_xz_score_byte);
    assert(kSpearmanRhoTestXZCorrelationEncodedByte == encoded_expected_xz_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed XZ scores do not differ\n");
    assert(kSpearmanRhoTestXZCorrelationEncodedByte == encoded_observed_xz_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed XZ scores do not differ\n");
    assert(encoded_expected_xz_score_byte == encoded_observed_xz_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed XZ scores do not differ\n");

    bs_delete_signal(&x);
    bs_delete_signal(&y);
    bs_delete_signal(&z);

    signal_t* a = NULL;
    bs_init_signal((char*) kTestVectorA, &a, kTrue, kTrue);
    if (!a) {
        fprintf(stderr, "Error: Could not allocate space for test (A) Spearman's rho vector!\n");
        exit(EXIT_FAILURE);
    }
    signal_t* b = NULL;
    bs_init_signal((char*) kTestVectorB, &b, kTrue, kTrue);
    if (!b) {
        fprintf(stderr, "Error: Could not allocate space for test (B) Spearman's rho vector!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "(V1) Comparing AB\n---\nA -> %s\nB -> %s\n---\n", kTestVectorA, kTestVectorB);
    if (a->n != b->n) {
        fprintf(stderr, "Error: Vectors being correlated are of unequal length!\n");
        bs_print_signal(a, stderr);
        bs_print_signal(b, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_ab_score_v1 = NAN;
    if ((a->sd_ranks != 0.0f) && (b->sd_ranks != 0.0f)) {
        unencoded_observed_ab_score_v1 = bs_spearman_rho_signal_v1(a, b, a->n);
    }
    fprintf(stderr, "Expected - unencoded AB Spearman's rho score: %3.6f\n", kSpearmanRhoTestABCorrelationUnencoded);
    fprintf(stderr, "Observed - unencoded AB Spearman's rho score (v1): %3.6f\n", unencoded_observed_ab_score_v1);
    score_t absolute_diff_unencoded_ab_scores_v1 = fabs(kSpearmanRhoTestABCorrelationUnencoded - unencoded_observed_ab_score_v1);
    assert(absolute_diff_unencoded_ab_scores_v1 + kEpsilonLessStringent > 0 && absolute_diff_unencoded_ab_scores_v1 - kEpsilonLessStringent < 0);
    fprintf(stderr, "\t-> Expected and observed (v1) AB scores do not differ within less stringent %3.7f error\n", kEpsilonLessStringent);
    byte_t encoded_expected_ab_score_byte_v1 = bs_encode_score_to_byte(kSpearmanRhoTestABCorrelationUnencoded);
    byte_t encoded_observed_ab_score_byte_v1 = bs_encode_score_to_byte(unencoded_observed_ab_score_v1);
    fprintf(stderr, "Expected - encoded, precomputed AB Spearman's rho score: 0x%02x\n", kSpearmanRhoTestABCorrelationEncodedByte);
    fprintf(stderr, "Expected - encoded, computed AB Spearman's rho score: 0x%02x\n", encoded_expected_ab_score_byte_v1);
    fprintf(stderr, "Observed - encoded, computed AB Spearman's rho score: 0x%02x\n", encoded_observed_ab_score_byte_v1);
    assert(kSpearmanRhoTestABCorrelationEncodedByte == encoded_expected_ab_score_byte_v1);
    fprintf(stderr, "\t-> Expected precomputed and computed AB scores do not differ\n");
    assert(kSpearmanRhoTestABCorrelationEncodedByte == encoded_observed_ab_score_byte_v1);
    fprintf(stderr, "\t-> Expected precomputed and observed computed AB scores do not differ\n");
    assert(encoded_expected_ab_score_byte_v1 == encoded_observed_ab_score_byte_v1);
    fprintf(stderr, "\t-> Expected computed and observed computed AB scores do not differ\n");

    fprintf(stderr, "(V2) Comparing AB\n---\nA -> %s\nB -> %s\n---\n", kTestVectorA, kTestVectorB);
    score_t unencoded_observed_ab_score_v2 = NAN;
    if ((a->sd_ranks != 0.0f) && (b->sd_ranks != 0.0f)) {
        unencoded_observed_ab_score_v2 = bs_spearman_rho_signal_v2(a, b, a->n);
    } 
    fprintf(stderr, "Expected - unencoded AB Spearman's rho score: %3.6f\n", kSpearmanRhoTestABCorrelationUnencoded);
    fprintf(stderr, "Observed - unencoded AB Spearman's rho score (v2): %3.6f\n", unencoded_observed_ab_score_v2);
    score_t absolute_diff_unencoded_ab_scores_v2 = fabs(kSpearmanRhoTestABCorrelationUnencoded - unencoded_observed_ab_score_v2);
    assert(absolute_diff_unencoded_ab_scores_v2 + kEpsilon > 0 && absolute_diff_unencoded_ab_scores_v2 - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed (v2) AB scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_ab_score_byte_v2 = bs_encode_score_to_byte(kSpearmanRhoTestABCorrelationUnencoded);
    byte_t encoded_observed_ab_score_byte_v2 = bs_encode_score_to_byte(unencoded_observed_ab_score_v2);
    fprintf(stderr, "Expected - encoded, precomputed AB Spearman's rho score: 0x%02x\n", kSpearmanRhoTestABCorrelationEncodedByte);
    fprintf(stderr, "Expected - encoded, computed AB Spearman's rho score: 0x%02x\n", encoded_expected_ab_score_byte_v2);
    fprintf(stderr, "Observed - encoded, computed AB Spearman's rho score: 0x%02x\n", encoded_observed_ab_score_byte_v2);
    assert(kSpearmanRhoTestABCorrelationEncodedByte == encoded_expected_ab_score_byte_v2);
    fprintf(stderr, "\t-> Expected precomputed and computed AB scores do not differ\n");
    assert(kSpearmanRhoTestABCorrelationEncodedByte == encoded_observed_ab_score_byte_v2);
    fprintf(stderr, "\t-> Expected precomputed and observed computed AB scores do not differ\n");
    assert(encoded_expected_ab_score_byte_v2 == encoded_observed_ab_score_byte_v2);
    fprintf(stderr, "\t-> Expected computed and observed computed AB scores do not differ\n");

    signal_t* c = NULL;
    bs_init_signal((char*) kTestVectorC, &c, kTrue, kTrue);
    if (!c) {
        fprintf(stderr, "Error: Could not allocate space for test (C) Spearman's rho vector!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "(V2) Comparing AC\n---\nA -> %s\nC -> %s\n---\n", kTestVectorA, kTestVectorC);
    if (a->n != c->n) {
        fprintf(stderr, "Error: Vectors being correlated are of unequal length!\n");
        bs_print_signal(a, stderr);
        bs_print_signal(c, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_ac_score_v2 = NAN;
    if ((a->sd_ranks != 0.0f) && (c->sd_ranks != 0.0f)) {
        unencoded_observed_ac_score_v2 = bs_spearman_rho_signal_v2(a, c, a->n);
    }
    fprintf(stderr, "Expected - unencoded AC Spearman's rho score: %3.6f\n", kSpearmanRhoTestACCorrelationUnencoded);
    fprintf(stderr, "Observed - unencoded AC Spearman's rho score (v2): %3.6f\n", unencoded_observed_ac_score_v2);
    fprintf(stderr, "\t-> Expected and observed (v2) AC scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_ac_score_byte_v2 = bs_encode_score_to_byte(kSpearmanRhoTestACCorrelationUnencoded);
    byte_t encoded_observed_ac_score_byte_v2 = bs_encode_score_to_byte(unencoded_observed_ac_score_v2);
    fprintf(stderr, "Expected - encoded, precomputed AC Spearman's rho score: 0x%02x\n", kSpearmanRhoTestACCorrelationEncodedByte);
    fprintf(stderr, "Expected - encoded, computed AC Spearman's rho score: 0x%02x\n", encoded_expected_ac_score_byte_v2);
    fprintf(stderr, "Observed - encoded, computed AC Spearman's rho score: 0x%02x\n", encoded_observed_ac_score_byte_v2);
    assert(kSpearmanRhoTestACCorrelationEncodedByte == encoded_expected_ac_score_byte_v2);
    fprintf(stderr, "\t-> Expected precomputed and computed AC scores do not differ\n");
    assert(kSpearmanRhoTestACCorrelationEncodedByte == encoded_observed_ac_score_byte_v2);
    fprintf(stderr, "\t-> Expected precomputed and observed computed AC scores do not differ\n");
    assert(encoded_expected_ac_score_byte_v2 == encoded_observed_ac_score_byte_v2);
    fprintf(stderr, "\t-> Expected computed and observed computed AC scores do not differ\n");

    bs_delete_signal(&a);
    bs_delete_signal(&b);
    bs_delete_signal(&c);
}

/**
 * @brief      bs_test_pearson_r()
 *
 * @details    Tests calculation and encoding of Pearson's 
 *             r score from test vectors
 */

void
bs_test_pearson_r()
{
    signal_t* a = NULL;
    bs_init_signal((char*) kTestVectorA, &a, kTrue, kFalse);
    if (!a) {
        fprintf(stderr, "Error: Could not allocate space for test (A) Pearson's r vector!\n");
        exit(EXIT_FAILURE);
    }
    signal_t* b = NULL;
    bs_init_signal((char*) kTestVectorB, &b, kTrue, kFalse);
    if (!b) {
        fprintf(stderr, "Error: Could not allocate space for test (B) Pearson's r vector!\n");
        exit(EXIT_FAILURE);
    }
    signal_t* c = NULL;
    bs_init_signal((char*) kTestVectorC, &c, kTrue, kFalse);
    if (!c) {
        fprintf(stderr, "Error: Could not allocate space for test (C) Pearson's r vector!\n");
        exit(EXIT_FAILURE);
    }
    
    fprintf(stderr, "Comparing AB\n---\nA -> %s\nB -> %s\n---\n", kTestVectorA, kTestVectorB);
    if (a->n != b->n) {
        fprintf(stderr, "Error: Vectors being correlated are of unequal length!\n");
        bs_print_signal(a, stderr);
        bs_print_signal(b, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_ab_score = NAN;
    if ((a->sd != 0.0f) && (b->sd != 0.0f)) {
        unencoded_observed_ab_score = bs_pearson_r_signal(a, b, a->n);
    }
    fprintf(stderr, "Expected - unencoded AB Pearson's r score: %3.6f\n", kPearsonRTestABCorrelationUnencoded);
    fprintf(stderr, "Observed - unencoded AB Pearson's r score: %3.6f\n", unencoded_observed_ab_score);
    score_t absolute_diff_unencoded_ab_scores = fabs(kPearsonRTestABCorrelationUnencoded - unencoded_observed_ab_score);
    assert(absolute_diff_unencoded_ab_scores + kEpsilon > 0 && absolute_diff_unencoded_ab_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed AB scores do not differ within %3.7f error\n", kEpsilon);
    byte_t encoded_expected_ab_score_byte = bs_encode_score_to_byte(kPearsonRTestABCorrelationUnencoded);
    byte_t encoded_observed_ab_score_byte = bs_encode_score_to_byte(unencoded_observed_ab_score);
    fprintf(stderr, "Expected - encoded, precomputed AB Pearson's r score: 0x%02x\n", kPearsonRTestABCorrelationEncodedByte);
    fprintf(stderr, "Expected - encoded, computed AB Pearson's r score: 0x%02x\n", encoded_expected_ab_score_byte);
    fprintf(stderr, "Observed - encoded, computed AB Pearson's r score: 0x%02x\n", encoded_observed_ab_score_byte);
    assert(kPearsonRTestABCorrelationEncodedByte == encoded_expected_ab_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed AB scores do not differ\n");
    assert(kPearsonRTestABCorrelationEncodedByte == encoded_observed_ab_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed AB scores do not differ\n");
    assert(encoded_expected_ab_score_byte == encoded_observed_ab_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed AB scores do not differ\n");

    fprintf(stderr, "Comparing AC\n---\nA -> %s\nC -> %s\n---\n", kTestVectorA, kTestVectorC);
    if (a->n != c->n) {
        fprintf(stderr, "Error: Vectors being correlated are of unequal length!\n");
        bs_print_signal(a, stderr);
        bs_print_signal(c, stderr);
        exit(EXIT_FAILURE);
    }
    score_t unencoded_observed_ac_score = NAN;
    if ((a->sd != 0.0f) && (c->sd != 0.0f)) {
        unencoded_observed_ac_score = bs_pearson_r_signal(a, c, a->n);
    }
    fprintf(stderr, "Expected - unencoded AC Pearson's r score: %3.6f\n", kPearsonRTestACCorrelationUnencoded);
    fprintf(stderr, "Observed - unencoded AC Pearson's r score: %3.6f\n", unencoded_observed_ac_score);
    byte_t encoded_expected_ac_score_byte = bs_encode_score_to_byte(kPearsonRTestACCorrelationUnencoded);
    byte_t encoded_observed_ac_score_byte = bs_encode_score_to_byte(unencoded_observed_ac_score);
    fprintf(stderr, "Expected - encoded, precomputed AC Pearson's r score: 0x%02x\n", kPearsonRTestACCorrelationEncodedByte);
    fprintf(stderr, "Expected - encoded, computed AC Pearson's r score: 0x%02x\n", encoded_expected_ac_score_byte);
    fprintf(stderr, "Observed - encoded, computed AC Pearson's r score: 0x%02x\n", encoded_observed_ac_score_byte);
    assert(kPearsonRTestACCorrelationEncodedByte == encoded_expected_ac_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed AC scores do not differ\n");
    assert(kPearsonRTestACCorrelationEncodedByte == encoded_observed_ac_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed AC scores do not differ\n");
    assert(encoded_expected_ac_score_byte == encoded_observed_ac_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed AC scores do not differ\n");    

    bs_delete_signal(&a);
    bs_delete_signal(&b);
    bs_delete_signal(&c);
}

/**
 * @brief      bs_test_score_encoding()
 *
 * @details    Tests encoding of scores in the interval 
 *             [-1.0, +1.0] at kEpsilon increments. 
 *
 *             Encoded scores are byte_t byte values 
 *             equivalent to the provided score value. Decoded 
 *             scores are the double-like "bin" with which the 
 *             original score associates.
 */

void
bs_test_score_encoding()
{
    score_t d, decode_d;
    int count;
    byte_t encode_d;
    
    fprintf(stderr, "Testing score encoding for strategy...\n");

    if (bs_globals.encoding_strategy == kEncodingStrategyFull) {
        for (d = -1.0f, count = 0; d <= 1.0f; d += kEpsilon, ++count) {
            encode_d = bs_encode_score_to_byte(d);
            decode_d = bs_decode_byte_to_score(encode_d);
            fprintf(stderr, 
                    "Test [%07d] [ %3.7f ]\t-> [ 0x%02x ]\t-> [ %3.7f ]\n",
                    count,
                    d, 
                    encode_d,
                    decode_d);
        }
    }
    else if (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) {
        for (d = -1.0f, count = 0; d <= 1.0f; d += kEpsilon, ++count) {
            encode_d = bs_encode_score_to_byte_mqz(d);
            decode_d = bs_decode_byte_to_score_mqz(encode_d);
            fprintf(stderr, 
                    "Test [%07d] [ %3.7f ]\t-> [ 0x%02x ]\t-> [ %3.7f ]\n",
                    count,
                    d, 
                    encode_d,
                    decode_d);
        }
    }
    else if (bs_globals.encoding_strategy == kEncodingStrategyCustom) {
        for (d = -1.0f, count = 0; d <= 1.0f; d += kEpsilon, ++count) {
            encode_d = bs_encode_score_to_byte_custom(d, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
            decode_d = bs_decode_byte_to_score_custom(encode_d, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
            fprintf(stderr, 
                    "Test [%07d] [ %3.7f ]\t-> [ 0x%02x ]\t-> [ %3.7f ]\n",
                    count,
                    d, 
                    encode_d,
                    decode_d);
        }
    }
}

/**
 * @brief      bs_init_globals()
 *
 * @details    Initialize application global variables.
 */

void
bs_init_globals()
{
    bs_globals.store_create_flag = kFalse;
    bs_globals.store_query_flag = kFalse;
    bs_globals.store_query_kind = kQueryKindDefaultKind;
    bs_globals.store_query_daemon_flag = kFalse;
    bs_globals.store_query_daemon_port = -1;
    bs_globals.store_query_daemon_hostname = bs_qd_get_host_fqdn();
    bs_globals.store_query_str[0] = '\0';
    bs_globals.store_query_idx_start = kQueryIndexDefaultStart;
    bs_globals.store_query_idx_end = kQueryIndexDefaultEnd;
    bs_globals.store_query_indices = NULL;
    bs_globals.store_query_indices_num = 0;
    bs_globals.store_query_indices_capacity = 0;
    bs_globals.store_query_indices_starts = NULL;
    bs_globals.store_query_indices_ends = NULL;
    bs_globals.store_query_range_start = bs_init_bed(kQueryRangeDefaultChromosome, kQueryRangeDefaultStart, kQueryRangeDefaultEnd);
    bs_globals.store_query_range_end = bs_init_bed(kQueryRangeDefaultChromosome, kQueryRangeDefaultStart, kQueryRangeDefaultEnd);
    bs_globals.store_row_chunk_size = kRowChunkDefaultSize;
    bs_globals.store_row_chunk_offset = kRowChunkDefaultOffset;
    bs_globals.store_chunk_size_specified_flag = kFalse;
    bs_globals.store_single_chunk_flag = kFalse;
    bs_globals.store_compression_flag = kFalse;
    bs_globals.store_filter = kScoreDefaultFilter;
    bs_globals.store_filter_mutual_set = kFalse;
    bs_globals.score_filter_cutoff = 0;
    bs_globals.score_filter_cutoff_lower_bound = 0;
    bs_globals.score_filter_cutoff_upper_bound = 0;
    bs_globals.score_filter_range_set = kFalse;
    bs_globals.rng_seed_flag = kFalse;
    bs_globals.rng_seed_value = 0;
    bs_globals.lookup_fn[0] = '\0';
    bs_globals.store_fn[0] = '\0';
    bs_globals.store_type_str[0] = '\0';
    bs_globals.store_type = kStoreUndefined;
    bs_globals.encoding_strategy = kEncodingStrategyUndefined;
    bs_globals.encoding_cutoff_zero_min = kEncodingStrategyDefaultCutoff;
    bs_globals.encoding_cutoff_zero_max = kEncodingStrategyDefaultCutoff;
    bs_globals.lookup_frequency_flag = kFalse;
    bs_globals.permutation_test_flag = kFalse;
    bs_globals.permutation_count = kPermutationTestDefaultCount;
    bs_globals.permutation_precision = kPermutationTestDefaultPrecision;
    bs_globals.permutation_alpha = kPermutationTestDefaultAlpha;
    bs_globals.permutation_significance_level = kPermutationTestDefaultSignificanceLevel;
    bs_globals.zero_sd_warning_issued = kFalse;
    bs_globals.score_ptr = NULL;
    bs_globals.bedextract_path = NULL;
    bs_globals.bedops_path = NULL;
    bs_globals.bedmap_path = NULL;
    bs_globals.sortbed_path = NULL;
    bs_globals.enable_ssl = kFalse;
    bs_globals.ssl_key_pem = NULL;
    bs_globals.ssl_cert_pem = NULL;
}

/**
 * @brief      bs_delete_globals()
 *
 * @details    Release application global variables from heap.
 */

void
bs_delete_globals()
{
    free(bs_globals.ssl_key_pem), bs_globals.ssl_key_pem = NULL;
    free(bs_globals.ssl_cert_pem), bs_globals.ssl_cert_pem = NULL;
    free(bs_globals.bedextract_path), bs_globals.bedextract_path = NULL;
    free(bs_globals.bedops_path), bs_globals.bedops_path = NULL;
    free(bs_globals.bedmap_path), bs_globals.bedmap_path = NULL;
    free(bs_globals.sortbed_path), bs_globals.sortbed_path = NULL;
    free(bs_globals.store_query_indices), bs_globals.store_query_indices = NULL;
    free(bs_globals.store_query_indices_starts), bs_globals.store_query_indices_starts = NULL;
    free(bs_globals.store_query_indices_ends), bs_globals.store_query_indices_ends = NULL;
    free(bs_globals.store_query_daemon_hostname), bs_globals.store_query_daemon_hostname = NULL;
    bs_delete_bed(&bs_globals.store_query_range_start);
    bs_delete_bed(&bs_globals.store_query_range_end);
}

/**
 * @brief      bs_init_command_line_options(argc, argv)
 *
 * @details    Initialize application state from command-line options.
 *
 * @param      argc    (int) number of CLI arguments
 *             argv    (char**) pointer to character pointer of option values
 */

void 
bs_init_command_line_options(int argc, char** argv)
{
    int bs_client_long_index;
    int bs_client_opt = getopt_long(argc,
                                    argv,
                                    bs_client_opt_string,
                                    bs_client_long_options,
                                    &bs_client_long_index);

    opterr = 0;
    int bs_output_flag_counter = 0;
    int bs_permutation_flag_counter = 0;
    int bs_score_filter_counter = 0;

    while (bs_client_opt != -1) {
        switch (bs_client_opt) {
        case 't':
            if (!optarg) {
                fprintf(stderr, "Error: Store type parameter specified without type value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            memcpy(bs_globals.store_type_str, optarg, strlen(optarg) + 1);
            bs_globals.store_type =
                (strcmp(bs_globals.store_type_str, kStorePearsonRSUTStr) == 0) ? kStorePearsonRSUT :
                (strcmp(bs_globals.store_type_str, kStorePearsonRSquareMatrixStr) == 0) ? kStorePearsonRSquareMatrix :
                (strcmp(bs_globals.store_type_str, kStorePearsonRSquareMatrixSplitStr) == 0) ? kStorePearsonRSquareMatrixSplit :
                (strcmp(bs_globals.store_type_str, kStorePearsonRSquareMatrixSplitSingleChunkStr) == 0) ? kStorePearsonRSquareMatrixSplitSingleChunk :
                (strcmp(bs_globals.store_type_str, kStorePearsonRSquareMatrixSplitSingleChunkMetadataStr) == 0) ? kStorePearsonRSquareMatrixSplitSingleChunkMetadata :
                (strcmp(bs_globals.store_type_str, kStorePearsonRSquareMatrixBzip2Str) == 0) ? kStorePearsonRSquareMatrixBzip2 :
                (strcmp(bs_globals.store_type_str, kStorePearsonRSquareMatrixBzip2SplitStr) == 0) ? kStorePearsonRSquareMatrixBzip2Split :
                (strcmp(bs_globals.store_type_str, kStoreSpearmanRhoSquareMatrixStr) == 0) ? kStoreSpearmanRhoSquareMatrix :
                (strcmp(bs_globals.store_type_str, kStoreSpearmanRhoSquareMatrixSplitStr) == 0) ? kStoreSpearmanRhoSquareMatrixSplit :
                (strcmp(bs_globals.store_type_str, kStoreSpearmanRhoSquareMatrixSplitSingleChunkStr) == 0) ? kStoreSpearmanRhoSquareMatrixSplitSingleChunk :
                (strcmp(bs_globals.store_type_str, kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadataStr) == 0) ? kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadata :
                (strcmp(bs_globals.store_type_str, kStoreSpearmanRhoSquareMatrixBzip2Str) == 0) ? kStoreSpearmanRhoSquareMatrixBzip2 :
                (strcmp(bs_globals.store_type_str, kStoreSpearmanRhoSquareMatrixBzip2SplitStr) == 0) ? kStoreSpearmanRhoSquareMatrixBzip2Split :
                (strcmp(bs_globals.store_type_str, kStoreJaccardIndexSquareMatrixStr) == 0) ? kStoreJaccardIndexSquareMatrix :
                (strcmp(bs_globals.store_type_str, kStoreJaccardIndexSquareMatrixSplitStr) == 0) ? kStoreJaccardIndexSquareMatrixSplit :
                (strcmp(bs_globals.store_type_str, kStoreJaccardIndexSquareMatrixSplitSingleChunkStr) == 0) ? kStoreJaccardIndexSquareMatrixSplitSingleChunk :
                (strcmp(bs_globals.store_type_str, kStoreJaccardIndexSquareMatrixSplitSingleChunkMetadataStr) == 0) ? kStoreJaccardIndexSquareMatrixSplitSingleChunkMetadata :
                (strcmp(bs_globals.store_type_str, kStoreOchiaiSimilaritySquareMatrixStr) == 0) ? kStoreOchiaiSimilaritySquareMatrix :
                (strcmp(bs_globals.store_type_str, kStoreOchiaiSimilaritySquareMatrixSplitStr) == 0) ? kStoreOchiaiSimilaritySquareMatrixSplit :
                (strcmp(bs_globals.store_type_str, kStoreOchiaiSimilaritySquareMatrixSplitSingleChunkStr) == 0) ? kStoreOchiaiSimilaritySquareMatrixSplitSingleChunk :
                (strcmp(bs_globals.store_type_str, kStoreOchiaiSimilaritySquareMatrixSplitSingleChunkMetadataStr) == 0) ? kStoreOchiaiSimilaritySquareMatrixSplitSingleChunkMetadata :
                (strcmp(bs_globals.store_type_str, kStorePearsonPhiSimilaritySquareMatrixStr) == 0) ? kStorePearsonPhiSimilaritySquareMatrix :
                (strcmp(bs_globals.store_type_str, kStorePearsonPhiSimilaritySquareMatrixSplitStr) == 0) ? kStorePearsonPhiSimilaritySquareMatrixSplit :
                (strcmp(bs_globals.store_type_str, kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunkStr) == 0) ? kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunk :
                (strcmp(bs_globals.store_type_str, kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunkMetadataStr) == 0) ? kStorePearsonPhiSimilaritySquareMatrixSplitSingleChunkMetadata :
                (strcmp(bs_globals.store_type_str, kStoreRogersAndTanimotoSimilaritySquareMatrixStr) == 0) ? kStoreRogersAndTanimotoSimilaritySquareMatrix :
                (strcmp(bs_globals.store_type_str, kStoreRogersAndTanimotoSimilaritySquareMatrixSplitStr) == 0) ? kStoreRogersAndTanimotoSimilaritySquareMatrixSplit :
                (strcmp(bs_globals.store_type_str, kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunkStr) == 0) ? kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunk :
                (strcmp(bs_globals.store_type_str, kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunkMetadataStr) == 0) ? kStoreRogersAndTanimotoSimilaritySquareMatrixSplitSingleChunkMetadata :
                (strcmp(bs_globals.store_type_str, kStoreNormalizedPointwiseMutualInformationSquareMatrixStr) == 0) ? kStoreNormalizedPointwiseMutualInformationSquareMatrix :
                (strcmp(bs_globals.store_type_str, kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitStr) == 0) ? kStoreNormalizedPointwiseMutualInformationSquareMatrixSplit :
                (strcmp(bs_globals.store_type_str, kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunkStr) == 0) ? kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunk :
                (strcmp(bs_globals.store_type_str, kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunkMetadataStr) == 0) ? kStoreNormalizedPointwiseMutualInformationSquareMatrixSplitSingleChunkMetadata :
                (strcmp(bs_globals.store_type_str, kStoreRandomSUTStr) == 0) ? kStoreRandomSUT :
                (strcmp(bs_globals.store_type_str, kStoreRandomSquareMatrixStr) == 0) ? kStoreRandomSquareMatrix :
                (strcmp(bs_globals.store_type_str, kStoreRandomBufferedSquareMatrixStr) == 0) ? kStoreRandomBufferedSquareMatrix :
                kStoreUndefined;
            if ((bs_globals.store_type == kStorePearsonRSquareMatrixBzip2) || (bs_globals.store_type == kStorePearsonRSquareMatrixBzip2Split))
                bs_globals.store_compression_flag = kTrue;
            break;
        case 'c':
            bs_globals.store_create_flag = kTrue;
            bs_output_flag_counter++;
            break;
        case 'q':
            bs_globals.store_query_flag = kTrue;
            bs_globals.store_query_daemon_flag = kFalse;
            bs_output_flag_counter++;
            break;
        case 'Q':
            bs_globals.store_query_daemon_flag = kTrue;
            bs_globals.store_query_flag = kTrue;
            bs_output_flag_counter++;
            if (!optarg) {
                fprintf(stderr, "Error: Store query daemon option selected without daemon port value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%d", &bs_globals.store_query_daemon_port);
            break;
        case 'u':
            bs_globals.lookup_frequency_flag = kTrue;
            bs_output_flag_counter++;
            break;
        case 'r':
            bs_globals.store_chunk_size_specified_flag = kTrue;
            if (!optarg) {
                fprintf(stderr, "Error: Store chunk size parameter specified without chunk size value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%u", &bs_globals.store_row_chunk_size);
            break;
        case 'k':
            bs_globals.store_single_chunk_flag = kTrue;
            if (!optarg) {
                fprintf(stderr, "Error: Store chunk offset parameter specified without chunk offset value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%u", &bs_globals.store_row_chunk_offset);
            break;
        case '2':
            bs_globals.store_filter = kScoreFilterGtEq;
            if (!optarg) {
                fprintf(stderr, "Error: Store filter operation parameter specified without filter cutoff value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%f", &bs_globals.score_filter_cutoff);
            bs_score_filter_counter++;
            break;
        case '3':
            bs_globals.store_filter = kScoreFilterGt;
            if (!optarg) {
                fprintf(stderr, "Error: Store filter operation parameter specified without filter cutoff value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%f", &bs_globals.score_filter_cutoff);
            bs_score_filter_counter++;
            break;
        case '4':
            bs_globals.store_filter = kScoreFilterEq;
            if (!optarg) {
                fprintf(stderr, "Error: Store filter operation parameter specified without filter cutoff value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%f", &bs_globals.score_filter_cutoff);
            bs_score_filter_counter++;
            break;
        case '5':
            bs_globals.store_filter = kScoreFilterLtEq;
            if (!optarg) {
                fprintf(stderr, "Error: Store filter operation parameter specified without filter cutoff value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%f", &bs_globals.score_filter_cutoff);
            bs_score_filter_counter++;
            break;
        case '6':
            bs_globals.store_filter = kScoreFilterLt;
            if (!optarg) {
                fprintf(stderr, "Error: Store filter operation parameter specified without filter cutoff value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%f", &bs_globals.score_filter_cutoff);
            bs_score_filter_counter++;
            break;
        case '7':
            bs_globals.store_filter = kScoreFilterRangedWithinExclusive;
            if (!optarg) {
                fprintf(stderr, "Error: Store filter operation parameter specified without filter cutoff range value (i:j)!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%f:%f", &bs_globals.score_filter_cutoff_lower_bound, &bs_globals.score_filter_cutoff_upper_bound);
            bs_globals.score_filter_range_set = kTrue;
            bs_score_filter_counter++;
            break;
        case '8':
            bs_globals.store_filter = kScoreFilterRangedWithinInclusive;
            if (!optarg) {
                fprintf(stderr, "Error: Store filter operation parameter specified without filter cutoff range value (i:j)!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%f:%f", &bs_globals.score_filter_cutoff_lower_bound, &bs_globals.score_filter_cutoff_upper_bound);
            bs_globals.score_filter_range_set = kTrue;
            bs_score_filter_counter++;
            break;            
        case '9':
            bs_globals.store_filter = kScoreFilterRangedOutsideExclusive;
            if (!optarg) {
                fprintf(stderr, "Error: Store filter operation parameter specified without filter cutoff range value (i:j)!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%f:%f", &bs_globals.score_filter_cutoff_lower_bound, &bs_globals.score_filter_cutoff_upper_bound);
            bs_globals.score_filter_range_set = kTrue;
            bs_score_filter_counter++;
            break;
        case '0':
            bs_globals.store_filter = kScoreFilterRangedOutsideInclusive;
            if (!optarg) {
                fprintf(stderr, "Error: Store filter operation parameter specified without filter cutoff range value (i:j)!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%f:%f", &bs_globals.score_filter_cutoff_lower_bound, &bs_globals.score_filter_cutoff_upper_bound);
            bs_globals.score_filter_range_set = kTrue;
            bs_score_filter_counter++;
            break;
        case 'm':
            bs_globals.store_filter_mutual_set = kTrue;
            break;
        case 'f':
            bs_globals.store_frequency_flag = kTrue;
            bs_output_flag_counter++;
            break;
        case 'i':
            bs_globals.store_query_kind = kQueryKindIndex;
            if (!optarg) {
                fprintf(stderr, "Error: Index query parameter specified without index value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            memcpy(bs_globals.store_query_str, optarg, strlen(optarg) + 1);
            break;
        case 'w':
            bs_globals.store_query_kind = kQueryKindMultipleIndices;
            if (!optarg) {
                fprintf(stderr, "Error: Multiple index query parameter specified without multiple index string value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            memcpy(bs_globals.store_query_str, optarg, strlen(optarg) + 1);
            break;
        case 'z':
            bs_globals.store_query_kind = kQueryKindMultipleIndicesFromFile;
            if (!optarg) {
                fprintf(stderr, "Error: Multiple index query parameter specified without multiple index string value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            memcpy(bs_globals.store_query_str, optarg, strlen(optarg) + 1);
            break;
        case 'g':
            bs_globals.store_query_kind = kQueryKindRange;
            if (!optarg) {
                fprintf(stderr, "Error: Range query parameter specified without range value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            memcpy(bs_globals.store_query_str, optarg, strlen(optarg) + 1);            
            break;            
        case 'l':
            if (!optarg) {
                fprintf(stderr, "Error: Lookup file parameter specified without lookup filename value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            memcpy(bs_globals.lookup_fn, optarg, strlen(optarg) + 1);
            if (!bs_path_exists(bs_globals.lookup_fn)) {
                fprintf(stderr, "Error: Lookup file [%s] does not exist!\n", bs_globals.lookup_fn);
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            break;
        case 's':
            if (!optarg) {
                fprintf(stderr, "Error: Store file parameter specified without store filename value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            memcpy(bs_globals.store_fn, optarg, strlen(optarg) + 1);
            break;
        case 'e':
            if (!optarg) {
                fprintf(stderr, "Error: Encoding strategy parameter specified without strategy value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            memcpy(bs_globals.encoding_strategy_str, optarg, strlen(optarg) + 1);
            bs_globals.encoding_strategy = 
                (strcmp(bs_globals.encoding_strategy_str, kEncodingStrategyFullStr) == 0) ? kEncodingStrategyFull :
                (strcmp(bs_globals.encoding_strategy_str, kEncodingStrategyMidQuarterZeroStr) == 0) ? kEncodingStrategyMidQuarterZero :
                (strcmp(bs_globals.encoding_strategy_str, kEncodingStrategyCustomStr) == 0) ? kEncodingStrategyCustom :
                kEncodingStrategyUndefined;
            break;
        case 'n':
            if (!optarg) {
                fprintf(stderr, "Error: Encoding cutoff parameter specified without cutoff minimum value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%f", &bs_globals.encoding_cutoff_zero_min);
            break;
        case 'x':
            if (!optarg) {
                fprintf(stderr, "Error: Encoding cutoff parameter specified without cutoff maximum value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%f", &bs_globals.encoding_cutoff_zero_max);
            break;
        case 'y':
            bs_globals.permutation_test_flag = kTrue;
            break;
        case 'o':
            bs_permutation_flag_counter++;
            if (!optarg) {
                fprintf(stderr, "Error: Permutation count parameter specified without count value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%u", &bs_globals.permutation_count);
            break;
        case 'p':
            bs_permutation_flag_counter++;
            if (!optarg) {
                fprintf(stderr, "Error: Permutation precision parameter specified without precision value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%f", &bs_globals.permutation_precision);
            break;
        case 'a':
            bs_permutation_flag_counter++;
            if (!optarg) {
                fprintf(stderr, "Error: Permutation alpha parameter specified without alpha value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%f", &bs_globals.permutation_alpha);
            break;
        case 'v':
            bs_permutation_flag_counter++;
            if (!optarg) {
                fprintf(stderr, "Error: Permutation significance parameter specified without significance value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%u", &bs_globals.permutation_significance_level);
            break;
        case 'd':
            bs_globals.rng_seed_flag = kTrue;
            if (!optarg) {
                fprintf(stderr, "Error: RNG seed parameter specified without seed value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            bs_globals.rng_seed_value = (uint32_t) strtol(optarg, NULL, 10);
            break;
        case 'E':
            bs_globals.enable_ssl = kTrue;
            break;
        case 'K':
            bs_globals.ssl_key_pem = bs_qd_load_file(optarg);
            if (!bs_globals.ssl_key_pem) {
                fprintf(stderr, "Error: Could not load SSL key!\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'C':
            bs_globals.ssl_cert_pem = bs_qd_load_file(optarg);
            if (!bs_globals.ssl_cert_pem) {
                fprintf(stderr, "Error: Could not load SSL certificate!\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'P':
            bs_test_pearson_r();
            exit(EXIT_SUCCESS);
        case 'S':
            bs_test_spearman_rho();
            exit(EXIT_SUCCESS);
        case 'J':
            bs_test_jaccard_index();
            exit(EXIT_SUCCESS);
        case 'O':
            bs_test_ochiai_similarity();
            exit(EXIT_SUCCESS);
        case 'H':
            bs_test_pearson_phi_similarity();
            exit(EXIT_SUCCESS);
        case 'T':
            bs_test_rogers_and_tanimoto_similarity();
            exit(EXIT_SUCCESS);
        case 'M':
            bs_test_normalized_pointwise_mutual_information();
            exit(EXIT_SUCCESS);
        case 'N':
            bs_globals.encoding_strategy = kEncodingStrategyFull;
            bs_test_score_encoding();
            exit(EXIT_SUCCESS);
        case 'h':
        case '?':
            bs_print_usage(stdout);
            exit(EXIT_SUCCESS);
        default:
            break;
        }
        bs_client_opt = getopt_long(argc,
                                    argv,
                                    bs_client_opt_string,
                                    bs_client_long_options,
                                    &bs_client_long_index);
    }

    if (bs_output_flag_counter != 1) {
        fprintf(stderr, "Error: Must create or query a data store, count bin-frequency of data store or lookup table, or perform permutation test!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    if (bs_score_filter_counter > 1) {
        fprintf(stderr, "Error: Cannot specify more than one filter operation!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    if (bs_score_filter_counter == 0) {
        bs_globals.store_filter = kScoreFilterNone;
    }

    if ((bs_globals.score_filter_range_set == kTrue) && (bs_globals.score_filter_cutoff_lower_bound > bs_globals.score_filter_cutoff_upper_bound)) {
        swap(bs_globals.score_filter_cutoff_lower_bound, bs_globals.score_filter_cutoff_upper_bound);
    }

    if (strlen(bs_globals.lookup_fn) == 0) {
        fprintf(stderr, "Error: Must specify lookup table filename!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    if ((strlen(bs_globals.store_fn) == 0) && (!bs_globals.lookup_frequency_flag && !bs_globals.permutation_test_flag)) {
        fprintf(stderr, "Error: Must specify store filename!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    if ((bs_globals.store_type == kStoreUndefined) && (!bs_globals.lookup_frequency_flag && !bs_globals.permutation_test_flag)) {
        fprintf(stderr, "Error: Must specify store type!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    if (bs_globals.encoding_strategy == kEncodingStrategyUndefined) {
        bs_globals.encoding_strategy = kEncodingStrategyFull;
    } 
    else if ((bs_globals.encoding_strategy == kEncodingStrategyCustom) && ((bs_globals.encoding_cutoff_zero_min == kEncodingStrategyDefaultCutoff) || (bs_globals.encoding_cutoff_zero_max == kEncodingStrategyDefaultCutoff))) {
        fprintf(stderr, "Error: Must specify --encoding-cutoff-zero-min and --encoding-cutoff-zero-max with custom encoding strategy!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    if (bs_globals.store_create_flag && bs_globals.store_compression_flag && !bs_globals.store_chunk_size_specified_flag) {
        fprintf(stderr, "Error: Must specify --store-compression-row-chunk-size parameter when used with pearson-r-sqr-bzip2 or pearson-r-sqr-bzip2-split encoding type!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    if ((bs_globals.store_type == kStorePearsonRSquareMatrixSplitSingleChunk && !bs_globals.store_single_chunk_flag) || 
        (bs_globals.store_type == kStorePearsonRSquareMatrixSplitSingleChunk && bs_globals.store_single_chunk_flag && !bs_globals.store_chunk_size_specified_flag) ||
        (bs_globals.store_type == kStorePearsonRSquareMatrixSplitSingleChunkMetadata && !bs_globals.store_chunk_size_specified_flag)) {
        fprintf(stderr, "Error: Must specify both chunk size and offset when encoding a single raw chunk! Or specify chunk size when writing simulated metadata record!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    if (bs_globals.enable_ssl && (!bs_globals.ssl_key_pem || !bs_globals.ssl_cert_pem)) {
        fprintf(stderr, "Error: Must specify paths to SSL key and certificate PEM files!\n");
        bs_print_usage(stderr);
        exit(EINVAL);
    }
}

/**
 * @brief      bs_print_usage(os)
 *
 * @details    Prints application usage statement to specified output stream.
 *
 * @param      os      (FILE*) output FILE stream (e.g., "stdout" or "stderr")
 */ 

void 
bs_print_usage(FILE* os) 
{
    fprintf(os,
            "\n" \
            " Usage: \n\n" \
            "   Create data store:\n\n" \
            "     %s --store-create --store-type [ type-of-store ] --lookup=fn --store=fn --encoding-strategy [ full | mid-quarter-zero | custom ] [--encoding-cutoff-zero-min=float --encoding-cutoff-zero-max=float ] [ --store-compression-row-chunk-size=int [ --store-compression-row-chunk-offset=int ] ]\n\n" \
            "   Query data store:\n\n" \
            "     %s --store-query --store-type [ type-of-store ] --lookup=fn --store=fn [ --index-query=str | --multiple-index-query=str | --range-query=str ] [ --score-filter-gteq=float | --score-filter-gt=float | --score-filter-eq=float | --score-filter-lteq=float | --score-filter-lt=float | --score-filter-ranged-within-inclusive=float:float | --score-filter-ranged-within-exclusive=float:float | --score-filter-ranged-outside-inclusive=float:float | --score-filter-ranged-outside-exclusive=float:float ]\n\n" \
            "   Initialize query HTTPD for data store:\n\n" \
            "     %s --store-query-daemon [port] --store-type [ type-of-store ] --lookup [fn] --store [fn] \n\n" \
            "   Bin-frequency on data store:\n\n"                          \
            "     %s --store-frequency --store-type [ type-of-store ] --lookup=fn --store=fn\n\n" \
            "   Bin-frequency and permutation testing  on lookup table:\n\n" \
            "     %s --lookup-frequency --lookup=fn\n"                  \
            "     %s --lookup-frequency --permutation-test --permutation-count=int --permutation-alpha=float --lookup=fn\n" \
            "     %s --lookup-frequency --permutation-test --permutation-precision=float --permutation-alpha=float --permutation-significance-level=int --lookup=fn\n\n" \
            " Available store types:\n\n"                               \
            " - pearson-r-sut\n"                                        \
            " - pearson-r-sqr\n"                                        \
            " - pearson-r-sqr-split\n"                                  \
            " - pearson-r-sqr-split-single-chunk\n"                     \
            " - pearson-r-sqr-split-single-chunk-metadata\n"            \
            " - pearson-r-sqr-bzip2\n"                                  \
            " - pearson-r-sqr-bzip2-split\n"                            \
            " - spearman-rho-sqr\n"                                     \
            " - spearman-rho-sqr-split\n"                               \
            " - spearman-rho-sqr-split-single-chunk\n"                  \
            " - spearman-rho-sqr-split-single-chunk-metadata\n"         \
            " - spearman-rho-sqr-bzip2\n"                               \
            " - spearman-rho-sqr-bzip2-split\n"                         \
            " - random-sut\n"                                           \
            " - random-sqr\n"                                           \
            " - random-buffered-sqr\n\n"                                \
            " Notes:\n\n"                                               \
            " - Store type describes either a strictly upper triangular (SUT) or square matrix\n" \
            "   and how it is created and populated.\n\n"               \
            "   The Pearson's r population method (pearson-r-sut | -sqr) uses the row vector in the\n" \
            "   fourth column of the lookup BED4 file to generate correlation scores between pairs\n" \
            "   of elements.\n\n"                                       \
            "   The random-sut and random-sqr methods populate the data store with random values\n" \
            "   drawn from the MT19937 RNG. In the case of random-sqr, a second pass is made through\n" \
            "   the output file to populate the lower triangular section of the matrix.\n\n" \
            "   The random-buffered-sqr method works identically to the random-sqr method, but only\n" \
            "   makes one pass to generate the output store file. This can require considerably more\n" \
            "   memory than the two-pass method.\n\n"                   \
            " - Lookup file is a sorted BED4 file.\n\n"                 \
            " - The fourth column of the BED4 file is a comma-delimited string of floating-point values.\n\n" \
            " - Query string is a numeric range specifing indices of interest from lookup\n" \
            "   table (e.g. \"17-83\" represents indices 17 through 83).\n\n" \
            " - The encoding strategy determines how scores map to bytes. The full strategy maps the full\n" \
            "   range of scores to the interval [-1.00, +1.00], while the mid-quarter-zero strategy maps\n" \
            "   values between (-0.25, +0.25) to the +0.00 bin.\n\n"    \
            " - Query output is in BED7 format (BED3 + BED3 + floating-point score).\n\n" \
            " - Frequency output is a three-column text file containing the score bin, count and frequency.\n\n" \
            " - If the 'pearson-r-sqr-bzip2' storage type is specified, then the --store-compression-row-chunk-size\n" \
            "   parameter must also be set to some integer value, as the number of rows in a compression unit.\n\n" \
            " - When compressing row blocks, the 'pearson-r-sqr-bzip2-split' store type writes the compressed\n" \
            "   data store to a separate folder containing one file per block, and a 'blocks.md' file containing\n" \
            "   archive metadata.\n\n" \
            " - Specifying --lookup-frequency without --permutation-test yields the frequency distribution table\n" \
            "   without any permutation testing.\n\n",
            bs_name,
            bs_name,
            bs_name,
            bs_name,
            bs_name,
            bs_name,
            bs_name);
}

/**
 * @brief      bs_path_exists(p)
 *
 * @details    Returns kTrue or kFalse depending on whether path 
 *             refers to an existing file or directory.
 *
 * @param      p       (const char*) path to test existence
 */ 

inline boolean_t 
bs_path_exists(const char* p)
{
    struct stat st;
    return (boolean_t) (stat(p, &st) == 0);
}

/**
 * @brief      bs_file_size(fn)
 *
 * @details    Returns file size parameter
 *
 * @param      fn     (const char*) file from which to retrieve file size
 */ 

inline ssize_t
bs_file_size(const char* fn)
{
    struct stat st;
    stat(fn, &st);
    return (ssize_t) st.st_size;
}

/**
 * @brief      bs_print_pair(os, chr_a, start_a, stop_a, chr_b, start_b, stop_b, score)
 *
 * @details    Prints pair to output stream
 *
 * @param      os       (FILE*) output stream where pair will be printed
 *             chr_a    (char*) chromosome A
 *             start_a  (uint64_t) start position A
 *             stop_a   (uint64_t) stop position A
 *             chr_b    (char*) chromosome B
 *             start_b  (uint64_t) start position B
 *             stop_b   (uint64_t) stop position B
 *             score_t  (score_t) correlation score
 */ 

inline void
bs_print_pair(FILE* os, char* chr_a, uint64_t start_a, uint64_t stop_a, char* chr_b, uint64_t start_b, uint64_t stop_b, score_t score)
{
    fprintf(os, 
            "%s\t%" PRIu64 "\t%" PRIu64"\t%s\t%" PRIu64 "\t%" PRIu64 "\t%3.2f\n",
            chr_a,
            start_a,
            stop_a,
            chr_b,
            start_b,
            stop_b,
            score);
    fflush(os);
}

/**
 * @brief      bs_print_pair_to_buffer(b, bl, chr_a, start_a, stop_a, chr_b, start_b, stop_b, score)
 *
 * @details    Prints pair to output stream
 *
 * @param      b        (char*) output buffer
 *             bl       (size_t*) output buffer length
 *             chr_a    (char*) chromosome A
 *             start_a  (uint64_t) start position A
 *             stop_a   (uint64_t) stop position A
 *             chr_b    (char*) chromosome B
 *             start_b  (uint64_t) start position B
 *             stop_b   (uint64_t) stop position B
 *             score_t  (score_t) correlation score
 */ 

inline void
bs_print_pair_to_buffer(char* b, size_t* bl, char* chr_a, uint64_t start_a, uint64_t stop_a, char* chr_b, uint64_t start_b, uint64_t stop_b, score_t score)
{
    *bl += sprintf(b, 
                   "%s\t%" PRIu64 "\t%" PRIu64"\t%s\t%" PRIu64 "\t%" PRIu64 "\t%3.2f\n",
                   chr_a,
                   start_a,
                   stop_a,
                   chr_b,
                   start_b,
                   stop_b,
                   score);
}

/**
 * @brief      bs_init_sut_store(n)
 *
 * @details    Initialize sut_store_t pointer to store SUT attributes.
 *
 * @param      n      (uint32_t) order of square matrix
 *
 * @return     (sut_store_t*) pointer to SUT attribute struct
 */

sut_store_t* 
bs_init_sut_store(uint32_t n)
{
    sut_store_t* s = NULL;
    store_attr_t* a = NULL;

    s = malloc(sizeof(sut_store_t));
    if (!s) {
        fprintf(stderr, "Error: Could not allocate space for SUT store!\n");
        exit(EXIT_FAILURE);
    }
    a = malloc(sizeof(store_attr_t));
    if (!a) {
        fprintf(stderr, "Error: Could not allocate space for SUT store attributes!\n");
        exit(EXIT_FAILURE);
    }
    a->nelems = n;
    a->nbytes = n * (n - 1) / 2; /* strictly upper triangular (SUT) matrix */
    a->fn = NULL;
    a->fn = malloc(strlen(bs_globals.store_fn) + 1);
    if (!a->fn) {
        fprintf(stderr, "Error: Could not allocate space for SUT attribute filename!\n");
        exit(EXIT_FAILURE);
    }
    memcpy(a->fn, bs_globals.store_fn, strlen(bs_globals.store_fn) + 1);

    s->attr = a;

    return s;
}

/**
 * @brief      bs_populate_sut_store_with_random_scores(s)
 *
 * @details    Write randomly-generated byte_t bytes to a
 *             FILE* handle associated with the specified SUT 
 *             store filename.
 *
 * @param      s      (sut_store_t*) pointer to SUT struct
 */

void
bs_populate_sut_store_with_random_scores(sut_store_t* s)
{
    byte_t score = 0;
    FILE* os = NULL;
    
    /* seed RNG */
    if (bs_globals.rng_seed_flag)
        mt19937_seed_rng(bs_globals.rng_seed_value);
    else
        mt19937_seed_rng(time(NULL));

    os = fopen(bs_globals.store_fn, "wb");
    if (ferror(os)) {
        fprintf(stderr, "Error: Could not open handle to output SUT store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* write stream of random scores out to os ptr */
    for (uint32_t idx = 0; idx < s->attr->nbytes; idx++) {
        do {
            score = (byte_t) (mt19937_generate_random_ulong() % 256);
        } while (score > 200); /* sample until a {0, ..., 200} bin is referenced */
        if (fputc(score, os) != score) {
            fprintf(stderr, "Error: Could not write score to output SUT store!\n");
            exit(EXIT_FAILURE);
        }
    }

    fclose(os);
}

/**
 * @brief      bs_populate_sut_store(s, l, *sf)
 *
 * @details    Write scores as encoded byte_t bytes 
 *             to a FILE* handle associated with the
 *             specified SUT store filename.
 *
 * @param      s      (sut_store_t*) pointer to SUT struct
 *             l      (lookup_t*) pointer to lookup table
 *             *sf    (score_t) score function pointer
 */

void
bs_populate_sut_store(sut_store_t* s, lookup_t* l, score_t (*sf)(signal_t*, signal_t*, uint32_t))
{
    FILE* os = NULL;

    os = fopen(bs_globals.store_fn, "wb");
    if (ferror(os)) {
        fprintf(stderr, "Error: Could not open handle to output SUT store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* write out a buffer of scores to output stream */
    size_t s_buf = 0;
    size_t n_buf = BUF_MAX_LEN;
    byte_t* buf = NULL;
    buf = malloc(n_buf * sizeof(*buf));    

    /* write scores to output stream ptr */
    for (uint32_t row_idx = 0; row_idx < s->attr->nelems; row_idx++) {
        signal_t* row_signal = l->elems[row_idx]->signal;
        for (uint32_t col_idx = row_idx + 1; col_idx < s->attr->nelems; col_idx++) {
            signal_t* col_signal = l->elems[col_idx]->signal;
            if (row_signal->n != col_signal->n) {
                fprintf(stderr, "Error: Vectors being correlated are of unequal length!\n");
                bs_print_signal(row_signal, stderr);
                bs_print_signal(col_signal, stderr);
                exit(EXIT_FAILURE);
            }
            score_t pairwise_score = NAN;
            if ((row_signal->data_contains_nan == kFalse) && (col_signal->data_contains_nan == kFalse)) {
                if (((sf == bs_pearson_r_signal) || (sf == bs_spearman_rho_signal_v2)) && (row_signal->sd != 0.0f) && (col_signal->sd != 0.0f)) {
                    pairwise_score = (*sf)(row_signal, col_signal, row_signal->n);
                }
                else {
                    pairwise_score = (*sf)(row_signal, col_signal, row_signal->n);
                }
            }
            else if (!bs_globals.zero_sd_warning_issued) {
                fprintf(stderr, "Warning: One or more vectors contain NAN or have zero standard deviation!\n");
                bs_globals.zero_sd_warning_issued = kTrue;
            }
            buf[s_buf++] = 
                (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_score_to_byte(pairwise_score) : 
                (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_score_to_byte_mqz(pairwise_score) : 
                bs_encode_score_to_byte_custom(pairwise_score, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
            if (s_buf == n_buf) {
                if (fwrite(buf, sizeof(*buf), n_buf, os) != n_buf) {
                    fprintf(stderr, "Error: Could not write score buffer to output SUT store at index (%" PRIu32 ", %" PRIu32 ")!\n", row_idx, col_idx);
                    exit(EXIT_FAILURE);
                }
                s_buf = 0;
            }            
        }
    }
    if (s_buf > 0) {
        if (fwrite(buf, sizeof(*buf), s_buf, os) != s_buf) {
            fprintf(stderr, "Error: Could not write final score buffer to output SUT store!\n");
            exit(EXIT_FAILURE);
        }        
    }    

    /* clean up */
    free(buf), buf = NULL;
    fclose(os);
}

/**
 * @brief      bs_sut_byte_offset_for_element_ij(n, i, j)
 *
 * @details    Returns a zero-indexed byte offset (off_t) value from a linear
 *             representation of bytes in a strictly upper-triangular (SUT)
 *             matrix of order n, for given row i and column j.
 *
 * @param      n      (uint32_t) order of square matrix
 *             i      (uint32_t) i-th row of matrix
 *             j      (uint32_t) j-th column of matrix
 *
 * @return     (off_t) byte offset into SUT byte array
 */

off_t
bs_sut_byte_offset_for_element_ij(uint32_t n, uint32_t i, uint32_t j)
{
    /* cf. http://stackoverflow.com/a/27088560/19410 */
    return (n * (n - 1)/2) - (n - i)*((n - i) - 1)/2 + j - i - 1;
}

/**
 * @brief      bs_print_sut_store_to_bed7(l, s, os)
 *
 * @details    Queries SUT store for provided index range globals
 *             and prints BED7 (BED3 + BED3 + floating point) to 
 *             specified output stream. The two BED3 elements are 
 *             retrieved from the lookup table and represent a 
 *             score pairing.
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sut_store_t*) pointer to SUT store
 *             os     (FILE*) pointer to output stream
 */

void
bs_print_sut_store_to_bed7(lookup_t* l, sut_store_t* s, FILE* os)
{
    if (!bs_path_exists(s->attr->fn)) {
        fprintf(stderr, "Error: Store file [%s] does not exist!\n", s->attr->fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    FILE* is = NULL;
    is = fopen(s->attr->fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to input store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* swap indices, if row and column range are in lower triangle */
    if (bs_globals.store_query_idx_start > bs_globals.store_query_idx_end) {
        swap(bs_globals.store_query_idx_start, bs_globals.store_query_idx_end);
    }

    off_t cur_offset = (off_t) ftell(is);
    for (uint32_t row_idx = bs_globals.store_query_idx_start; row_idx <= bs_globals.store_query_idx_end; row_idx++) {
        for (uint32_t col_idx = 0; col_idx < l->nelems; col_idx++) {
            if (row_idx == col_idx)
                continue;
            /* to minimize fseek calls, we only fseek if we need to move the file ptr backwards */
            off_t new_offset = bs_sut_byte_offset_for_element_ij(l->nelems, (col_idx > row_idx) ? row_idx : col_idx, (col_idx > row_idx) ? col_idx : row_idx);
            if (cur_offset != new_offset) {
                fseek(is, new_offset, SEEK_SET);
                cur_offset = new_offset;
            }
            score_t d = (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score((byte_t) fgetc(is)) : bs_decode_byte_to_score_mqz((byte_t) fgetc(is));
            cur_offset++;
            bs_print_pair(os, 
                          l->elems[row_idx]->chr,
                          l->elems[row_idx]->start,
                          l->elems[row_idx]->stop,
                          l->elems[col_idx]->chr,
                          l->elems[col_idx]->start,
                          l->elems[col_idx]->stop,
                          d);
        }
    }

    fclose(is);
}

/**
 * @brief      bs_print_sut_filtered_store_to_bed7(l, s, os, f, op)
 *
 * @details    Queries SUT store for provided index range globals
 *             and prints BED7 (BED3 + BED3 + floating point) to 
 *             specified output stream, after filtered against the
 *             provided score cutoff. The two BED3 elements are 
 *             retrieved from the lookup table and represent a 
 *             score pairing.
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sut_store_t*) pointer to SUT store
 *             os     (FILE*) pointer to output stream
 *             fc     (score_t) score filter cutoff
 *             fo     (score_filter_t) score filter operation
 */

void
bs_print_sut_filtered_store_to_bed7(lookup_t* l, sut_store_t* s, FILE* os, score_t fc, score_filter_t fo)
{
    if (!bs_path_exists(s->attr->fn)) {
        fprintf(stderr, "Error: Store file [%s] does not exist!\n", s->attr->fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    FILE* is = NULL;
    is = fopen(s->attr->fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to input store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* swap indices, if row and column range are in lower triangle */
    if (bs_globals.store_query_idx_start > bs_globals.store_query_idx_end) {
        swap(bs_globals.store_query_idx_start, bs_globals.store_query_idx_end);
    }

    off_t cur_offset = (off_t) ftell(is);
    for (uint32_t row_idx = bs_globals.store_query_idx_start; row_idx <= bs_globals.store_query_idx_end; row_idx++) {
        for (uint32_t col_idx = 0; col_idx < l->nelems; col_idx++) {
            if (row_idx == col_idx)
                continue;
            /* to minimize fseek calls, we only fseek if we need to move the file ptr backwards */
            off_t new_offset = bs_sut_byte_offset_for_element_ij(l->nelems, (col_idx > row_idx) ? row_idx : col_idx, (col_idx > row_idx) ? col_idx : row_idx);
            if (cur_offset != new_offset) {
                fseek(is, new_offset, SEEK_SET);
                cur_offset = new_offset;
            }
            score_t d = (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score((byte_t) fgetc(is)) : bs_decode_byte_to_score_mqz((byte_t) fgetc(is));
            cur_offset++;

            if ( ((fo == kScoreFilterGtEq) && (d >= fc)) ||
                 ((fo == kScoreFilterGt) && (d > fc)) ||
                 ((fo == kScoreFilterEq) && (fabs(d - fc) < kEpsilon)) ||
                 ((fo == kScoreFilterLtEq) && (d <= fc)) ||
                 ((fo == kScoreFilterLt) && (d < fc)) ) {
                bs_print_pair(os,
                              l->elems[row_idx]->chr,
                              l->elems[row_idx]->start,
                              l->elems[row_idx]->stop,
                              l->elems[col_idx]->chr,
                              l->elems[col_idx]->start,
                              l->elems[col_idx]->stop,
                              d);
            }
        }
    }

    fclose(is);
}

/**
 * @brief      bs_print_sut_frequency_to_txt(l, s, os)
 *
 * @details    Prints bin score, count and frequency of 
 *             SUT store to specified output stream.
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sut_store_t*) pointer to SUT store
 *             os     (FILE*) pointer to output stream
 */

void
bs_print_sut_frequency_to_txt(lookup_t* l, sut_store_t* s, FILE* os)
{
    if (!bs_path_exists(s->attr->fn)) {
        fprintf(stderr, "Error: Store file [%s] does not exist!\n", s->attr->fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    FILE* is = NULL;
    is = fopen(s->attr->fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to input store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    byte_t* byte_buf = NULL;
    byte_buf = malloc(l->nelems);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }

    uint64_t freq_table[256] = {0};
    uint32_t row_idx = 0;
    uint32_t col_idx = 1;
    do {
        off_t start_offset = bs_sut_byte_offset_for_element_ij(l->nelems, row_idx, col_idx);
        off_t end_offset = bs_sut_byte_offset_for_element_ij(l->nelems, row_idx, l->nelems);
        size_t nelems = (size_t) (end_offset - start_offset);
        size_t nbyte = 0;
        if (fread(byte_buf, sizeof(*byte_buf), nelems, is) != nelems) {
            fprintf(stderr, "Error: Could not read a row of data from SUT input stream!\n");
            exit(EXIT_FAILURE);
        }
        do {
            freq_table[byte_buf[nbyte]]++;
            col_idx++;
        } while (++nbyte < nelems);
        col_idx = ++row_idx + 1;
    } while ((row_idx + 1) < l->nelems);

    bs_print_frequency_buffer(freq_table, s->attr->nbytes, os);

    /* clean up */
    free(byte_buf);
    fclose(is);
}

/**
 * @brief      bs_delete_sut_store(s)
 *
 * @details    Release memory associated with SUT store pointer.
 *
 * @param      s      (sut_store_t**) pointer to SUT struct pointer
 */

void
bs_delete_sut_store(sut_store_t** s)
{
    free((*s)->attr->fn), (*s)->attr->fn = NULL;
    free((*s)->attr), (*s)->attr = NULL;
    free(*s), *s = NULL;
}

/**
 * @brief      bs_init_sqr_store(n)
 *
 * @details    Initialize sqr_store_t pointer to store square matrix attributes.
 *
 * @param      n      (uint32_t) order of square matrix
 *
 * @return     (sqr_store_t*) pointer to square matrix struct
 */

sqr_store_t* 
bs_init_sqr_store(uint32_t n)
{
    sqr_store_t* s = NULL;
    store_attr_t* a = NULL;

    s = malloc(sizeof(sqr_store_t));
    if (!s) {
        fprintf(stderr, "Error: Could not allocate space for square matrix store!\n");
        exit(EXIT_FAILURE);
    }
    a = malloc(sizeof(store_attr_t));
    if (!a) {
        fprintf(stderr, "Error: Could not allocate space for square matrix store attributes!\n");
        exit(EXIT_FAILURE);
    }
    a->nelems = n;
    a->nbytes = n * n;
    a->fn = NULL;
    a->fn = malloc(strlen(bs_globals.store_fn) + 1);
    if (!a->fn) {
        fprintf(stderr, "Error: Could not allocate space for square matrix store filename attribute!\n");
        exit(EXIT_FAILURE);
    }
    memcpy(a->fn, bs_globals.store_fn, strlen(bs_globals.store_fn) + 1);

    s->attr = a;

    return s;
}

/**
 * @brief      bs_populate_sqr_store_with_random_scores(s)
 *
 * @details    Write randomly-generated byte_t bytes to a
 *             FILE* handle associated with the specified square 
 *             matrix store filename.
 *
 * @param      s      (sqr_store_t*) pointer to square matrix store
 */

void
bs_populate_sqr_store_with_random_scores(sqr_store_t* s)
{
    byte_t score = 0;
    FILE* os = NULL;
    byte_t self_correlation_score =
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_score_to_byte(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_score_to_byte_mqz(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_score_to_byte_custom(kSelfCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_score_to_byte(kSelfCorrelationScore);
    byte_t no_correlation_score =
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_score_to_byte(kNoCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_score_to_byte_mqz(kNoCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_score_to_byte_custom(kNoCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_score_to_byte(kNoCorrelationScore);
    
    /* seed RNG */
    if (bs_globals.rng_seed_flag)
        mt19937_seed_rng(bs_globals.rng_seed_value);
    else
        mt19937_seed_rng(time(NULL));

    os = fopen(bs_globals.store_fn, "wb");
    if (ferror(os)) {
        fprintf(stderr, "Error: Could not open handle to output square matrix store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* write stream of random scores out to os ptr */
    for (uint32_t row_idx = 0; row_idx < s->attr->nelems; row_idx++) {
        for (uint32_t col_idx = 0; col_idx < s->attr->nelems; col_idx++) {
            if (row_idx < col_idx) {
                do {
                    score = (byte_t) (mt19937_generate_random_ulong() % 256);
                } while (score > 200); /* sample until a {0, ..., 200} bin is referenced */
            }
            else if (row_idx == col_idx) {
                score = self_correlation_score;
            }
            else if (row_idx > col_idx) {
                /* write placeholder byte to mtx[row_idx][col_idx] -- we overwrite this further down */
                score = no_correlation_score;
            }
            if (fputc(score, os) != score) {
                fprintf(stderr, "Error: Could not write score to output square matrix store at index (%" PRIu32 ", %" PRIu32 ")!\n", row_idx, col_idx);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(os);
    
    /* do a second pass over matrix to overwrite with "mirror" values */
    
    os = fopen(bs_globals.store_fn, "rb+");
    if (ferror(os)) {
        fprintf(stderr, "Error: Could not open handle to output square matrix store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    byte_t* row_bytes = NULL;
    row_bytes = malloc(s->attr->nelems - 1); /* first byte is a self-correlation score and is not needed */
    if (!row_bytes) {
        fprintf(stderr, "Error: Could not allocate space to row byte buffer!\n");
        exit(EXIT_FAILURE);
    }
    int uc = 0;
    off_t start_offset = 0;
    off_t end_offset = 0;
    off_t offset_idx = 0;
    ssize_t byte_idx = 0;
    for (uint32_t row_idx = 0; row_idx < s->attr->nelems - 1; row_idx++) {
        /* copy row of score bytes to a temporary buffer */
        fseek(os, bs_sqr_byte_offset_for_element_ij(s->attr->nelems, row_idx, row_idx + 1), SEEK_SET);
        size_t bytes_to_read = s->attr->nelems - row_idx - 1;
        if (fread(row_bytes, sizeof(*row_bytes), bytes_to_read, os) != bytes_to_read) {
            fprintf(stderr, "Error: Could not read row bytes from square matrix store!\n");
            exit(EXIT_FAILURE);
        }
        /* copy temporary buffer to equivalent column */
        start_offset =  bs_sqr_byte_offset_for_element_ij(s->attr->nelems, row_idx + 1, row_idx);
        end_offset = bs_sqr_byte_offset_for_element_ij(s->attr->nelems, s->attr->nelems - 1, row_idx);
        for (offset_idx = start_offset, byte_idx = 0; offset_idx <= end_offset; offset_idx += s->attr->nelems, byte_idx++) {
            fseek(os, offset_idx, SEEK_SET);
            uc = row_bytes[byte_idx];
            fputc(uc, os);
        }
    }    
    free(row_bytes);
    row_bytes = NULL;
    
    fclose(os);
}

/**
 * @brief      bs_populate_sqr_store_with_buffered_random_scores(s)
 *
 * @details    Write randomly-generated byte_t bytes to a
 *             FILE* handle associated with the specified square 
 *             matrix store filename, buffering rows of data to 
 *             populate lower diagonal values without a second 
 *             pass through the file handle..
 *
 * @param      s      (sqr_store_t*) pointer to square matrix store
 */

void
bs_populate_sqr_store_with_buffered_random_scores(sqr_store_t* s)
{
    byte_t score = 0;
    FILE* os = NULL;
    byte_t self_correlation_score =
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_score_to_byte(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_score_to_byte_mqz(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_score_to_byte_custom(kSelfCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_score_to_byte(kSelfCorrelationScore);
    
    /* seed RNG */
    if (bs_globals.rng_seed_flag)
        mt19937_seed_rng(bs_globals.rng_seed_value);
    else
        mt19937_seed_rng(time(NULL));

    os = fopen(bs_globals.store_fn, "wb");
    if (ferror(os)) {
        fprintf(stderr, "Error: Could not open handle to output square matrix store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* 
       To eliminate a second pass through the store file (required to write the 
       lower triangle portion of the matrix) we need to buffer values from the 
       just-written upper-triangular-side row. Those buffered values can then be 
       written as we stream through the cells of the square matrix we are 
       populating. 

       However, due to the order in which we need to retrieve cached score values, 
       we keep an n-element array of "row nodes" that each point to the start of 
       a linked list of row-ordered score values.

       A "row_node_ptr" (an element in the n-element array row_node_ptr_buf) 
       points to the "head" of a row's elements in the store_buf_node_t* linked 
       list node_ptr_buf.

       row_node_ptr_buf[0]
       ┌───┐
       │ 0 │ 
       └───┘
         ↓ head, tail
       ┌────────┐   ┌────────┐         ┌────────┐
       │ (0, 1) │ → │ (0, 2) │ → ... → │ (0, n) │
       └────────┘   └────────┘         └────────┘
       list of 0-row scores

       This structure is built for each row, as we stream through values:

       row_node_ptr_buf[1]
       ┌───┐
       │ 1 │ 
       └───┘
         ↓ head, tail
       ┌────────┐   ┌────────┐         ┌────────┐
       │ (1, 2) │ → │ (1, 3) │ → ... → │ (1, n) │
       └────────┘   └────────┘         └────────┘
       list of 1-row scores

       ...

       We populate the matrix one row at a time, like scanlines in an old television set.

       We populate the matrix, starting at the first row, where row_idx < col_idx.
       We take the linked list row_node_ptr_buf[0]->tail and append the just-
       calculated score value. The head node row_node_ptr_buf[0]->head remains 
       the first node pointed to by row_node_ptr_buf[0]->tail.
       
       On the next row (r=1), we run into the immediate case where row_idx > col_idx.
       We grab the head pointer of row_node_ptr_buf[col_idx] -- note the use of col_idx
       -- and retrieve the stored score value. Once we have the value, we delete the head 
       node from the linked list. The head pointer is moved the next node in the list.

       On the same row, we again run into the case where row_idx < col_idx. So we
       start appending nodes to row_node_ptr_buf[1]->list and set row_node_ptr_buf[1]->head
       to the start of this list.

       On the next row (r=2), we again run into the case where row_idx > col_idx.
       We grab the head of row_node_ptr_buf[col_idx], retrieve the score value, delete
       the head node and move the head pointer to the next node. Once row_idx < col_idx,
       we grow a new list.

       Storage
       -------

       We grow and shrink linked lists as we walk through and populate a symmetric square matrix.

       For an even (0 mod 2) order value for n, a quarter of the square matrix should need its 
       scores to be buffered:

       [(n/2)^2 * sizeof(store_buf_node_t)] bytes

       For an odd (1 mod 2) order of n, the limit is:

       [((n+1)/2) * ((n+1)/2 - 1) * sizeof(store_buf_node_t)] bytes

       We also need to store pointers to each row's linked list of buffered scores, as well as
       the head of that list:

       [n * sizeof(store_buf_row_node_t*)] bytes
       
       The total storage for the cache is the sum of these two values for a given n.

       Operations
       ----------

       There are (n * (n - 1))/2 list node insertions and, to match each node creation,
       we need (n * (n - 1))/2 node deletions. Thus, caching is O(n^2).

    */

    /* set up and init array of store_buf_row_node_t* */
    store_buf_row_node_t** row_node_ptr_buf = NULL;
    row_node_ptr_buf = malloc(sizeof(store_buf_row_node_t*) * s->attr->nelems);
    for (uint32_t elem_idx = 0; elem_idx < s->attr->nelems; elem_idx++) {
        row_node_ptr_buf[elem_idx] = NULL;
        row_node_ptr_buf[elem_idx] = malloc(sizeof(store_buf_row_node_t));
        if (!row_node_ptr_buf[elem_idx]) {
            fprintf(stderr, "Error: Could not allocate space for row node ptr buffer!\n");
            exit(EXIT_FAILURE);
        }
        row_node_ptr_buf[elem_idx]->head = NULL;
        row_node_ptr_buf[elem_idx]->tail = NULL;
    }

    /* write stream of random scores out to os ptr */
    for (uint32_t row_idx = 0; row_idx < s->attr->nelems; row_idx++) {
        for (uint32_t col_idx = 0; col_idx < s->attr->nelems; col_idx++) {
            if (row_idx < col_idx) {
                do {
                    score = (byte_t) (mt19937_generate_random_ulong() % 256);
                } while (score > 200); /* sample until a {0, ..., 200} bin is referenced */
                if (fputc(score, os) != score) {
                    fprintf(stderr, "Error: Could not write score to output square matrix store!\n");
                    exit(EXIT_FAILURE);
                }
                /* add score to buffer linked list */
                if (!row_node_ptr_buf[row_idx]->tail) {
                    row_node_ptr_buf[row_idx]->tail = bs_init_store_buf_node(score);
                    row_node_ptr_buf[row_idx]->head = row_node_ptr_buf[row_idx]->tail;
                }
                else {
                    store_buf_node_t* new_node = bs_init_store_buf_node(score);
                    bs_insert_store_buf_node(row_node_ptr_buf[row_idx]->tail, new_node);
                    row_node_ptr_buf[row_idx]->tail = new_node;
                }
            }
            else if (row_idx == col_idx) {
                score = self_correlation_score;
                if (fputc(score, os) != score) {
                    fprintf(stderr, "Error: Could not write kSelfCorrelationScore to output square matrix store!\n");
                    exit(EXIT_FAILURE);
                }
            }
            else if (row_idx > col_idx) {
                /* retrieve cached score from buffer -- we switch to col_idx to get mirror element */
                score = row_node_ptr_buf[col_idx]->head->data;
                /* get pointer to next node (which could be NULL) */
                store_buf_node_t* next_node = row_node_ptr_buf[col_idx]->head->next;
                /* delete what head is pointing to */
                free(row_node_ptr_buf[col_idx]->head);
                /* move head up to next node, if it exists, else set head and tail to NULL */
                if (next_node) {
                    row_node_ptr_buf[col_idx]->head = next_node;
                }
                else {
                    row_node_ptr_buf[col_idx]->head = NULL;
                    row_node_ptr_buf[col_idx]->tail = NULL;
                }
                /* writed cached score value to file handle */
                if (fputc(score, os) != score) {
                    fprintf(stderr, "Error: Could not write cached score to output square matrix store!\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    /* clean up the cache */
    for (uint32_t elem_idx = 0; elem_idx < s->attr->nelems; elem_idx++) {
        if (row_node_ptr_buf[elem_idx]->head || row_node_ptr_buf[elem_idx]->tail) {
            fprintf(stderr, "Error: Some cached row node data was not copied over entirely!\n");
            exit(EXIT_FAILURE);
        }
        free(row_node_ptr_buf[elem_idx]);
    }
    free(row_node_ptr_buf);

    fclose(os);
}

/**
 * @brief      bs_populate_sqr_store(s, l, *sf)
 *
 * @details    Write correlation scores as encoded byte_t bytes to a 
 *             FILE* handle associated with the specified square matrix 
 *             store filename.
 *
 * @param      s      (sqr_store_t*) pointer to square matrix store
 *             l      (lookup_t*) pointer to lookup table
 *             *sf    (score_t) score function pointer
 */

void
bs_populate_sqr_store(sqr_store_t* s, lookup_t* l, score_t (*sf)(signal_t*, signal_t*, uint32_t))
{
    FILE* os = NULL;
    byte_t self_correlation_score =
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_score_to_byte(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_score_to_byte_mqz(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_score_to_byte_custom(kSelfCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_score_to_byte(kSelfCorrelationScore);

    os = fopen(bs_globals.store_fn, "wb");
    if (ferror(os)) {
        fprintf(stderr, "Error: Could not open handle to output square matrix store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* write out a buffer of scores to output stream */
    size_t s_buf = 0;
    size_t n_buf = BUF_MAX_LEN;
    byte_t* buf = NULL;
    buf = malloc(n_buf * sizeof(*buf));
    
    /* write scores to output stream ptr */
    for (uint32_t row_idx = 0; row_idx < s->attr->nelems; row_idx++) {
        signal_t* row_signal = l->elems[row_idx]->signal;
        for (uint32_t col_idx = 0; col_idx < s->attr->nelems; col_idx++) {
            signal_t* col_signal = l->elems[col_idx]->signal;
            if (row_idx != col_idx) {
                if (row_signal->n != col_signal->n) {
                    fprintf(stderr, "Error: Vectors being correlated are of unequal length!\n");
                    bs_print_signal(row_signal, stderr);
                    bs_print_signal(col_signal, stderr);
                    exit(EXIT_FAILURE);
                }
                score_t pairwise_score = NAN;
                if ((row_signal->data_contains_nan == kFalse) && (col_signal->data_contains_nan == kFalse)) {
                    if (((sf == bs_pearson_r_signal) || (sf == bs_spearman_rho_signal_v2)) && (row_signal->sd != 0.0f) && (col_signal->sd != 0.0f)) {
                        pairwise_score = (*sf)(row_signal, col_signal, row_signal->n);
                    }
                    else {
                        pairwise_score = (*sf)(row_signal, col_signal, row_signal->n);
                    }
                }
                else if (!bs_globals.zero_sd_warning_issued) {
                    fprintf(stderr, "Warning: One or more vectors contain NAN or have zero standard deviation!\n");
                    bs_globals.zero_sd_warning_issued = kTrue;
                }
                buf[s_buf++] = 
                    (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_score_to_byte(pairwise_score) : 
                    (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_score_to_byte_mqz(pairwise_score) : 
                    bs_encode_score_to_byte_custom(pairwise_score, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
            }
            else if (row_idx == col_idx) {
                buf[s_buf++] = self_correlation_score;
            }
            if (s_buf == n_buf) {
                if (fwrite(buf, sizeof(*buf), n_buf, os) != n_buf) {
                    fprintf(stderr, "Error: Could not write score buffer to output square matrix store at index (%" PRIu32 ", %" PRIu32 ")!\n", row_idx, col_idx);
                    exit(EXIT_FAILURE);
                }
                s_buf = 0;
            }
        }
    }
    if (s_buf > 0) {
        if (fwrite(buf, sizeof(*buf), s_buf, os) != s_buf) {
            fprintf(stderr, "Error: Could not write final score buffer to output square matrix store!\n");
            exit(EXIT_FAILURE);
        }        
    }

    /* clean up */
    free(buf), buf = NULL;
    fclose(os);
}

/**
 * @brief      bs_init_sqr_split_store_fn_str(d, i)
 *
 * @details    Prints formatted string for per-block raw
 *             file name, given parent directory name
 *
 * @param      d      (char*) directory name
 *             i      (uint32_t) block index
 *
 * @return     (char*) formatted per-block filename string 
 */

char*
bs_init_sqr_split_store_fn_str(char* d, uint32_t i)
{
    char* block_dest_fn = NULL;
    block_dest_fn = malloc(strlen(d) + BLOCK_STR_MAX_LEN + 6);
    if (!block_dest_fn) {
        fprintf(stderr, "Error: Could not allocate space for block destination filename string!\n");
        exit(EXIT_FAILURE);
    }
    snprintf(block_dest_fn, strlen(d) + BLOCK_STR_MAX_LEN + 6, "%s/%0*u.rbs", d, (int) BLOCK_STR_MAX_LEN, i);
    return block_dest_fn;
}

/**
 * @brief      bs_init_sqr_split_store_dir_str(p)
 *
 * @details    Prints formatted string for directory name
 *             storing per-block raw blocks given prefix
 *
 * @param      p      (char*) prefix for directory name
 *
 * @return     (char*) formatted directory string 
 */

char*
bs_init_sqr_split_store_dir_str(char* p)
{
    char* block_dest_dir = NULL;
    if (strlen(kRawMetadataSplitDirSuffix) > 0) {
        block_dest_dir = malloc(strlen(p) + strlen(kRawMetadataSplitDirSuffix) + 2);
        if (!block_dest_dir) {
            fprintf(stderr, "Error: Could not allocate space for block destination string!\n");
            exit(EXIT_FAILURE);
        }
        snprintf(block_dest_dir, strlen(p) + strlen(kRawMetadataSplitDirSuffix) + 2, "%s.%s", p, kRawMetadataSplitDirSuffix);
    }
    else {
        block_dest_dir = malloc(strlen(p) + 1);
        if (!block_dest_dir) {
            fprintf(stderr, "Error: Could not allocate space for block destination string!\n");
            exit(EXIT_FAILURE);
        }
        snprintf(block_dest_dir, strlen(p) + 1, "%s", p);
    }
    return block_dest_dir;
}

/**
 * @brief      bs_init_sqr_split_store_metadata_fn_str(d)
 *
 * @details    Prints formatted string for metadata filename
 *             given parent directory name
 *
 * @param      d      (char*) directory name
 *
 * @return     (char*) formatted metadata filename string 
 */

char*
bs_init_sqr_split_store_metadata_fn_str(char* d)
{
    char* md_dest_fn = NULL;
    md_dest_fn = malloc(strlen(d) + strlen(kRawMetadataSplitFn) + 2);
    if (!md_dest_fn) {
        fprintf(stderr, "Error: Could not allocate space for metadata destination filename string!\n");
        exit(EXIT_FAILURE);
    }
    snprintf(md_dest_fn, strlen(d) + strlen(kRawMetadataSplitFn) + 2, "%s/%s", d, kRawMetadataSplitFn);
    return md_dest_fn;
}

/**
 * @brief      bs_populate_sqr_split_store(s, l, n, *sf, sv)
 *
 * @details    Write each raw block of encoded correlation scores to a FILE* 
 *             handle associated with the specified square matrix store filename. 
 *             Each block and a metadata file are stored in a folder, its name 
 *             determined by the store filename.
 *
 * @param      s      (sqr_store_t*) pointer to square matrix store
 *             l      (lookup_t*) pointer to lookup table
 *             n      (uint32_t) number of rows within a raw chunk 
 *             *sf    (score_t) score function pointer
 *             sv     (score_variety_t) score variety
 */

void
bs_populate_sqr_split_store(sqr_store_t* s, lookup_t* l, uint32_t n, score_t (*sf)(signal_t*, signal_t*, uint32_t), score_variety_t sv)
{
    byte_t score = 0;
    FILE* os = NULL;
    byte_t self_correlation_score =
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_score_to_byte(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_score_to_byte_mqz(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_score_to_byte_custom(kSelfCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_score_to_byte(kSelfCorrelationScore);
    
    /* create an owner read/write/executable directory to contain block files */
    char* block_dest_dir = NULL;
    block_dest_dir = bs_init_sqr_bzip2_split_store_dir_str(s->attr->fn);
    if (bs_path_exists(block_dest_dir)) {
        fprintf(stderr, "Error: Store per-block destination [%s] exists!\n", block_dest_dir);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }    
    mkdir(block_dest_dir, S_IRUSR | S_IWUSR | S_IXUSR);
    
    /* init offset array */
    off_t* offsets = NULL;
    uint32_t num_offsets = floor(l->nelems / n) + 1;
    offsets = malloc(num_offsets * sizeof(*offsets));
    if (!offsets) {
        fprintf(stderr, "Error: Cannot allocate memory for offset array!\n");
        exit(EXIT_FAILURE);
    }
    uint32_t offset_idx = 0;
    
    /* iterate through l->nelems, one block of rows at a time; write a raw stream buffer for each block */
    uint64_t bytes_written = 0;
    uint64_t cumulative_bytes_written = 0;
    uint32_t block_idx = 0;
    char *block_dest_fn = NULL;
    
    /* write out a buffer of 1 row (l->nelems bytes) to output stream */
    byte_t* buf = NULL;
    size_t buf_size = l->nelems;
    buf = malloc(buf_size * sizeof(*buf));
    size_t buf_idx = 0;
    
    for (uint32_t row_idx = 1; row_idx <= s->attr->nelems; row_idx++) {
        if (((row_idx - 1) % n == 0) && (!os)) {
            /* open handle to output sqr matrix store */
            block_dest_fn = bs_init_sqr_split_store_fn_str(block_dest_dir, block_idx++);
            os = fopen(block_dest_fn, "wb");
            if (ferror(os)) {
                fprintf(stderr, "Error: Could not open handle to output square matrix store!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            buf_idx = 0;
            free(block_dest_fn), block_dest_fn = NULL;
            offsets[offset_idx++] = cumulative_bytes_written;
        }
        signal_t* row_signal = l->elems[(row_idx - 1)]->signal;
        for (uint32_t col_idx = 1; col_idx <= s->attr->nelems; col_idx++) {
            signal_t* col_signal = l->elems[(col_idx - 1)]->signal;
            if (row_idx != col_idx) {
                if (row_signal->n != col_signal->n) {
                    fprintf(stderr, "Error: Vectors being correlated are of unequal length!\n");
                    bs_print_signal(row_signal, stderr);
                    bs_print_signal(col_signal, stderr);
                    exit(EXIT_FAILURE);
                }
                score_t pairwise_score = NAN;
                if ((row_signal->data_contains_nan == kFalse) && (col_signal->data_contains_nan == kFalse)) {
                    if (((sf == bs_pearson_r_signal) || (sf == bs_spearman_rho_signal_v2)) && (row_signal->sd != 0.0f) && (col_signal->sd != 0.0f)) {
                        pairwise_score = (*sf)(row_signal, col_signal, row_signal->n);
                    }
                    else {
                        pairwise_score = (*sf)(row_signal, col_signal, row_signal->n);
                    }
                }
                else if (!bs_globals.zero_sd_warning_issued) {
                    fprintf(stderr, "Warning: One or more vectors contain NAN or have zero standard deviation!\n");
                    bs_globals.zero_sd_warning_issued = kTrue;
                }
                score = 
                    (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_score_to_byte(pairwise_score) : 
                    (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_score_to_byte_mqz(pairwise_score) : 
                    bs_encode_score_to_byte_custom(pairwise_score, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
            }
            else {
                score = self_correlation_score;
            }            
            buf[buf_idx++] = score;
        }
            
        /* if buf is full, write its contents to output stream */
        if (buf_idx % buf_size == 0) {
            bytes_written = fwrite(buf, sizeof(*buf), buf_idx, os);
            if (bytes_written != buf_idx) { 
                fprintf(stderr, "Error: Could not write score buffer to output square matrix store!\n");
                exit(EXIT_FAILURE);
            }
            cumulative_bytes_written += bytes_written;
            buf_idx = 0;
        }
        
        /* if file is multiple of n * l->elems, close stream, open new stream */
        if ((cumulative_bytes_written % (l->nelems * n) == 0) && (row_idx != s->attr->nelems)) {
            fclose(os), os = NULL;
            /* open new handle to output sqr matrix store */
            block_dest_fn = bs_init_sqr_split_store_fn_str(block_dest_dir, block_idx++);
            os = fopen(block_dest_fn, "wb");
            if (ferror(os)) {
                fprintf(stderr, "Error: Could not open new handle to output square matrix store within-column!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            free(block_dest_fn), block_dest_fn = NULL;
            buf_idx = 0;
            offsets[offset_idx++] = cumulative_bytes_written;
        }
    
        /* if row index is last index in matrix, write final buf to output stream, and close output stream */    
        else if (row_idx == s->attr->nelems) {
            /* write buf to output stream */
            bytes_written = fwrite(buf, sizeof(*buf), buf_idx, os);
            if (bytes_written != buf_idx) {
                fprintf(stderr, "Error: Could not write score buffer to output square matrix store!\n");
                exit(EXIT_FAILURE);
            }
            cumulative_bytes_written += bytes_written;
            fclose(os), os = NULL;
            offsets[offset_idx++] = cumulative_bytes_written;
        }
    }

    /* convert offsets to formatted metadata string and write to output stream */
    char* md_str = NULL;
    md_str = bs_init_metadata_str(offsets, offset_idx, n, sv);
    if (!md_str) {
        fprintf(stderr, "Error: Could not generate metadata string from offsets!\n");
        exit(EXIT_FAILURE);
    }
    char* md_dest_fn = bs_init_sqr_bzip2_split_store_metadata_fn_str(block_dest_dir);
    FILE* md_os = fopen(md_dest_fn, "wb");
    if (ferror(md_os)) {
        fprintf(stderr, "Error: Could not open handle to output metadata!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    fwrite(md_str, 1, strlen(md_str), md_os);
    fclose(md_os), md_os = NULL;
    
    free(block_dest_dir), block_dest_dir = NULL;
    free(offsets), offsets = NULL;
    free(md_str), md_str = NULL;    
}

/**
 * @brief      bs_populate_sqr_split_store_chunk_with(s, l, n, o, sf)
 *
 * @details    Write one raw block of encoded correlation scores to a FILE* 
 *             handle associated with the specified square matrix store filename. 
 *             Each block and a metadata file are stored in a folder, its name 
 *             determined by the store filename. The folder is created if it 
 *             does not already exist.
 *
 * @param      s      (sqr_store_t*) pointer to square matrix store
 *             l      (lookup_t*) pointer to lookup table
 *             n      (uint32_t) number of rows within a raw chunk
 *             o      (uint32_t) offset to starting row (zero-indexed)
 *             *sf    (score_t) score function pointer
 */

void
bs_populate_sqr_split_store_chunk(sqr_store_t* s, lookup_t* l, uint32_t n, uint32_t o, score_t (*sf)(signal_t*, signal_t*, uint32_t))
{
    byte_t score = 0;
    FILE* os = NULL;
    byte_t self_correlation_score =
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_score_to_byte(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_score_to_byte_mqz(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_score_to_byte_custom(kSelfCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_score_to_byte(kSelfCorrelationScore);
    
    /* if necessary, create an owner read/write/executable directory to contain block files */
    char* block_dest_dir = NULL;
    block_dest_dir = bs_init_sqr_bzip2_split_store_dir_str(s->attr->fn);
    if (!bs_path_exists(block_dest_dir)) {
        mkdir(block_dest_dir, S_IRUSR | S_IWUSR | S_IXUSR);
    }
    
    /* iterate through l->nelems for one block of rows; write a raw stream buffer for that block */
    if (n == 0) {
        fprintf(stderr, "Error: Block size cannot be zero elements in length!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    if (o % n != 0) {
        fprintf(stderr, "Error: Row offset must be exact multiple of block size!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    uint32_t block_idx = o / n;
    uint64_t bytes_written = 0;
    char *block_dest_fn = NULL;
    block_dest_fn = bs_init_sqr_split_store_fn_str(block_dest_dir, block_idx);
    if (bs_path_exists(block_dest_fn)) {
        fprintf(stderr, "Error: Block file already exists! [%s]\n", block_dest_fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    os = fopen(block_dest_fn, "wb");
    if (ferror(os)) {
        fprintf(stderr, "Error: Could not open handle to output square matrix store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    /* write out a buffer of 1 row (l->nelems bytes) to output stream */
    byte_t* buf = NULL;
    size_t buf_size = l->nelems;
    buf = malloc(buf_size * sizeof(*buf));
    size_t buf_idx = 0;
    
    for (uint32_t row_idx = (o + 1); row_idx <= (o + n); row_idx++) {
        signal_t* row_signal = l->elems[(row_idx - 1)]->signal;
        for (uint32_t col_idx = 1; col_idx <= l->nelems; col_idx++) {
            signal_t* col_signal = l->elems[(col_idx - 1)]->signal;
            if (row_idx != col_idx) {
                if (row_signal->n != col_signal->n) {
                    fprintf(stderr, "Error: Vectors being correlated are of unequal length!\n");
                    bs_print_signal(row_signal, stderr);
                    bs_print_signal(col_signal, stderr);
                    exit(EXIT_FAILURE);
                }
                score_t pairwise_score = NAN;
                if ((row_signal->data_contains_nan == kFalse) && (col_signal->data_contains_nan == kFalse)) {
                    if (((sf == bs_pearson_r_signal) || (sf == bs_spearman_rho_signal_v2)) && (row_signal->sd != 0.0f) && (col_signal->sd != 0.0f)) {
                        pairwise_score = (*sf)(row_signal, col_signal, row_signal->n);
                    }
                    else {
                        pairwise_score = (*sf)(row_signal, col_signal, row_signal->n);
                    }
                }
                else if (!bs_globals.zero_sd_warning_issued) {
                    fprintf(stderr, "Warning: One or more vectors contain NAN or have zero standard deviation!\n");
                    bs_globals.zero_sd_warning_issued = kTrue;
                }
                score = 
                    (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_score_to_byte(pairwise_score) : 
                    (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_score_to_byte_mqz(pairwise_score) : 
                    bs_encode_score_to_byte_custom(pairwise_score, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
            }
            else {
                score = self_correlation_score;
            }
            buf[buf_idx++] = score;
        }
        /* at the end of a row, we write out the bytes */
        bytes_written = fwrite(buf, sizeof(*buf), buf_idx, os);
        if (bytes_written != buf_idx) { 
            fprintf(stderr, "Error: Could not write score buffer to output square matrix store (single chunk; bytes written [%" PRIu64 "] buffer size [%zu])!\n", bytes_written, buf_size);
            exit(EXIT_FAILURE);
        }
        /* if we are at the end of the array, we break out */
        if (row_idx == l->nelems) {
            break;
        }
        buf_idx = 0;
    }
    fclose(os), os = NULL;
    free(block_dest_fn), block_dest_fn = NULL;
    free(block_dest_dir), block_dest_dir = NULL;
}

/**
 * @brief      bs_populate_sqr_split_store_chunk_metadata(s, l, n, v)
 *
 * @details    Write metadata to FILE* handle associated with the specified 
 *             per-chunk square matrix store. Metadata are stored in a folder, 
 *             its name determined by the store filename. The folder is created 
 *             if it does not already exist.
 *
 * @param      s      (sqr_store_t*) pointer to square matrix store
 *             l      (lookup_t*) pointer to lookup table
 *             n      (uint32_t) number of rows within a raw chunk
 *             v      (score_variety_t) score variety 
 */

void
bs_populate_sqr_split_store_chunk_metadata(sqr_store_t* s, lookup_t* l, uint32_t n, score_variety_t v)
{
    /* if necessary, create an owner read/write/executable directory to contain block files */
    char* block_dest_dir = NULL;
    block_dest_dir = bs_init_sqr_bzip2_split_store_dir_str(s->attr->fn);
    if (!bs_path_exists(block_dest_dir)) {
        mkdir(block_dest_dir, S_IRUSR | S_IWUSR | S_IXUSR);
    }
    
    uint64_t bytes_written = 0;
    uint64_t cumulative_bytes_written = 0;
    
    /* init offset array */
    off_t* offsets = NULL;
    uint32_t num_offsets = floor(l->nelems / n) + 2;
    offsets = malloc(num_offsets * sizeof(*offsets));
    if (!offsets) {
        fprintf(stderr, "Error: Cannot allocate memory for offset array!\n");
        exit(EXIT_FAILURE);
    }
    uint32_t offset_idx = 0;

    /* create dummy offsets from chunk size */
    for (uint32_t row_idx = 1; row_idx <= s->attr->nelems; row_idx++) {
        bytes_written = l->nelems;
        if ((row_idx - 1) % n == 0) {
            offsets[offset_idx++] = cumulative_bytes_written;
        }
        cumulative_bytes_written += bytes_written;
    }
    offsets[offset_idx++] = cumulative_bytes_written;
    
    /* convert offsets to formatted metadata string and write to output stream */
    char* md_str = NULL;
    md_str = bs_init_metadata_str(offsets, offset_idx, n, v);
    if (!md_str) {
        fprintf(stderr, "Error: Could not generate metadata string from offsets!\n");
        exit(EXIT_FAILURE);
    }
    char* md_dest_fn = bs_init_sqr_bzip2_split_store_metadata_fn_str(block_dest_dir);
    FILE* md_os = fopen(md_dest_fn, "wb");
    if (ferror(md_os)) {
        fprintf(stderr, "Error: Could not open handle to output metadata!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    fwrite(md_str, 1, strlen(md_str), md_os);
    fclose(md_os), md_os = NULL;
    
    free(block_dest_dir), block_dest_dir = NULL;
    free(offsets), offsets = NULL;
    free(md_str), md_str = NULL;    
}

/**
 * @brief      bs_populate_sqr_bzip2_store(s, l, n, *sf, sv)
 *
 * @details    Write bzip2-compressed chunks of encoded correlation 
 *             scores to a FILE* handle associated with the specified 
 *             square matrix store filename.
 *
 * @param      s      (sqr_store_t*) pointer to square matrix store
 *             l      (lookup_t*) pointer to lookup table
 *             n      (uint32_t) number of rows within a compressed chunk 
 *             *sf    (score_t) score function pointer
 *             sv     (score_variety_t) score variety
 */

void
bs_populate_sqr_bzip2_store(sqr_store_t* s, lookup_t* l, uint32_t n, score_t (*sf)(signal_t*, signal_t*, uint32_t), score_variety_t sv)
{
    byte_t score = 0;
    FILE* os = NULL;
    byte_t self_correlation_score =
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_score_to_byte(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_score_to_byte_mqz(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_score_to_byte_custom(kSelfCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_score_to_byte(kSelfCorrelationScore);

    /* test bounds of row-chunk size */
    if (n > kCompressionRowChunkMaximumSize) {
        fprintf(stderr, "Error: Number of specified rows within a chunk cannot be larger than the chunk size maximum (%u)!\n", kCompressionRowChunkMaximumSize);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    /* open handle to output sqr matrix store */
    os = fopen(bs_globals.store_fn, "wb");
    if (ferror(os)) {
        fprintf(stderr, "Error: Could not open handle to output square matrix store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* init offset array */
    off_t* offsets = NULL;
    uint32_t num_offsets = floor(l->nelems / n) + 1;
    offsets = malloc(num_offsets * sizeof(*offsets));
    if (!offsets) {
        fprintf(stderr, "Error: Cannot allocate memory for offset array!\n");
        exit(EXIT_FAILURE);
    }
    uint32_t offset_idx = 0;
    
    /* init bzip2 machinery */
    uint32_t bz_block_size = kCompressionBzip2BlockSize100k * kCompressionBzip2BlockSizeFactor;
    uint32_t bz_uncompressed_buffer_size = bz_block_size;
    char *bz_uncompressed_buffer = NULL;
    bz_uncompressed_buffer = malloc(bz_uncompressed_buffer_size);
    if (!bz_uncompressed_buffer) {
        fprintf(stderr, "Error: Could not allocate space to bzip2 uncompressed byte buffer!\n");
        exit(EXIT_FAILURE);
    }
    BZFILE* bzf = NULL;
    int bzf_error = BZ_OK;
    uint32_t bzf_bytes_read_lo32 = 0;
    uint32_t bzf_bytes_read_hi32 = 0;
    uint32_t bzf_bytes_written_lo32 = 0;
    uint32_t bzf_bytes_written_hi32 = 0;
    uint64_t bzf_cumulative_bytes_written = 0;
    bzf = BZ2_bzWriteOpen(&bzf_error, os, kCompressionBzip2BlockSize100k, kCompressionBzip2Verbosity, kCompressionBzip2WorkFactor);
    switch (bzf_error) {
    case BZ_OK:
        offsets[offset_idx++] = bzf_cumulative_bytes_written;
        break;
    case BZ_CONFIG_ERROR:
    case BZ_PARAM_ERROR:
    case BZ_IO_ERROR:
    case BZ_MEM_ERROR:
        fprintf(stderr, "Error: Could not set up new bzip2 output stream! (initial)\n");
        exit(EXIT_FAILURE);
    }

    /* write compressed bytes, if an end-of-block or -chunk condition is met within a row, or at the end of a series of rows */
    uint32_t uncompressed_buffer_idx = 0;
    for (uint32_t row_idx = 1; row_idx <= s->attr->nelems; row_idx++) {
        signal_t* row_signal = l->elems[(row_idx - 1)]->signal;
        for (uint32_t col_idx = 1; col_idx <= s->attr->nelems; col_idx++) {
            signal_t* col_signal = l->elems[(col_idx - 1)]->signal;
            if (row_idx != col_idx) {
                if (row_signal->n != col_signal->n) {
                    fprintf(stderr, "Error: Vectors being correlated are of unequal length!\n");
                    bs_print_signal(row_signal, stderr);
                    bs_print_signal(col_signal, stderr);
                    exit(EXIT_FAILURE);
                }
                score_t pairwise_score = NAN;
                if ((row_signal->data_contains_nan == kFalse) && (col_signal->data_contains_nan == kFalse)) {
                    if (((sf == bs_pearson_r_signal) || (sf == bs_spearman_rho_signal_v2)) && (row_signal->sd != 0.0f) && (col_signal->sd != 0.0f)) {
                        pairwise_score = (*sf)(row_signal, col_signal, row_signal->n);
                    }
                    else {
                        pairwise_score = (*sf)(row_signal, col_signal, row_signal->n);
                    }
                }
                else if (!bs_globals.zero_sd_warning_issued) {
                    fprintf(stderr, "Warning: One or more vectors contain NAN or have zero standard deviation!\n");
                    bs_globals.zero_sd_warning_issued = kTrue;
                }
                score = 
                    (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_score_to_byte(pairwise_score) : 
                    (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_score_to_byte_mqz(pairwise_score) : 
                    bs_encode_score_to_byte_custom(pairwise_score, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
            }
            else if (row_idx == col_idx) {
                score = self_correlation_score;
            }
            bz_uncompressed_buffer[uncompressed_buffer_idx++] = score;
            /* if bz_uncompressed_buffer is full, compress it, write compressed bytes to output stream, but do not close stream */
            if (uncompressed_buffer_idx % bz_uncompressed_buffer_size == 0) {
                BZ2_bzWrite(&bzf_error, bzf, bz_uncompressed_buffer, uncompressed_buffer_idx);
                switch (bzf_error) {
                case BZ_OK:
                    break;
                case BZ_PARAM_ERROR:
                case BZ_IO_ERROR:
                case BZ_SEQUENCE_ERROR:
                    fprintf(stderr, "Error: Could not write buffer to bzip2 output stream! (full, within row)\n");
                    exit(EXIT_FAILURE);
                }
                uncompressed_buffer_idx = 0;
            }
        }
        /* if row index is last index in matrix, compress bytes, write to output stream, and close bz stream */
        if (row_idx == s->attr->nelems) {
            BZ2_bzWrite(&bzf_error, bzf, bz_uncompressed_buffer, uncompressed_buffer_idx);
            switch (bzf_error) {
            case BZ_OK:
                break;
            case BZ_PARAM_ERROR:
            case BZ_IO_ERROR:
            case BZ_SEQUENCE_ERROR:
                fprintf(stderr, "Error: Could not write buffer to bzip2 output stream! (last row)\n");
                exit(EXIT_FAILURE);
            }
            uncompressed_buffer_idx = 0;
            BZ2_bzWriteClose64(&bzf_error, bzf, kCompressionBzip2AbandonPolicy, &bzf_bytes_read_lo32, &bzf_bytes_read_hi32, &bzf_bytes_written_lo32, &bzf_bytes_written_hi32);
            switch (bzf_error) {
            case BZ_OK:
                bzf_cumulative_bytes_written += ((off_t) bzf_bytes_written_hi32 << 32) + bzf_bytes_written_lo32;
                offsets[offset_idx++] = bzf_cumulative_bytes_written;
                break;
            case BZ_IO_ERROR:
            case BZ_SEQUENCE_ERROR:
                fprintf(stderr, "Error: Could not close bzip2 output stream! (last row)\n");
                exit(EXIT_FAILURE);
            }
        }
        /* else if row index is multiple of row block, compress bytes, write to output stream, and close/reinitialize bz stream */
        else if (row_idx % n == 0) {
            BZ2_bzWrite(&bzf_error, bzf, bz_uncompressed_buffer, uncompressed_buffer_idx);
            switch (bzf_error) {
            case BZ_OK:
                break;
            case BZ_PARAM_ERROR:
            case BZ_IO_ERROR:
            case BZ_SEQUENCE_ERROR:
                fprintf(stderr, "Error: Could not write buffer to bzip2 output stream! (row block)\n");
                exit(EXIT_FAILURE);
            }
            uncompressed_buffer_idx = 0;
            BZ2_bzWriteClose64(&bzf_error, bzf, kCompressionBzip2AbandonPolicy, &bzf_bytes_read_lo32, &bzf_bytes_read_hi32, &bzf_bytes_written_lo32, &bzf_bytes_written_hi32);
            switch (bzf_error) {
            case BZ_OK:
                bzf_cumulative_bytes_written += ((off_t) bzf_bytes_written_hi32 << 32) + bzf_bytes_written_lo32;
                break;
            case BZ_IO_ERROR:
            case BZ_SEQUENCE_ERROR:
                fprintf(stderr, "Error: Could not close bzip2 output stream! (row block)\n");
                exit(EXIT_FAILURE);
            }
            bzf = BZ2_bzWriteOpen(&bzf_error, os, kCompressionBzip2BlockSize100k, kCompressionBzip2Verbosity, kCompressionBzip2WorkFactor);
            switch (bzf_error) {
            case BZ_OK:
                offsets[offset_idx++] = bzf_cumulative_bytes_written;
                break;
            case BZ_CONFIG_ERROR:
            case BZ_PARAM_ERROR:
            case BZ_IO_ERROR:
            case BZ_MEM_ERROR:
                fprintf(stderr, "Error: Could not set up new bzip2 output stream! (post-row block)\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    /* convert offsets to formatted metadata string and write to output stream */
    char* md_str = NULL;
    md_str = bs_init_metadata_str(offsets, offset_idx, n, sv);
    if (!md_str) {
        fprintf(stderr, "Error: Could not generate metadata string from offsets!\n");
        exit(EXIT_FAILURE);
    }
    fwrite(md_str, 1, strlen(md_str), os);

    /* clean up */
    free(bz_uncompressed_buffer), bz_uncompressed_buffer = NULL;
    free(offsets), offsets = NULL;
    free(md_str), md_str = NULL;
    fclose(os);
}

/**
 * @brief      bs_populate_sqr_bzip2_split_store(s, l, n, *sf, sv)
 *
 * @details    Write each bzip2-compressed block of encoded correlation scores 
 *             to a FILE* handle associated with the specified square matrix 
 *             store filename. Each block and a metadata file are stored in a 
 *             folder, its name determined by the store filename.
 *
 * @param      s      (sqr_store_t*) pointer to square matrix store
 *             l      (lookup_t*) pointer to lookup table
 *             n      (uint32_t) number of rows within a compressed chunk 
 *             *sf    (score_t) score function pointer
 *             sv     (score_variety_t) score variety
 */

void
bs_populate_sqr_bzip2_split_store(sqr_store_t* s, lookup_t* l, uint32_t n, score_t (*sf)(signal_t*, signal_t*, uint32_t), score_variety_t sv)
{
    byte_t score = 0;
    FILE* os = NULL;
    byte_t self_correlation_score =
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_score_to_byte(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_score_to_byte_mqz(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_score_to_byte_custom(kSelfCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_score_to_byte(kSelfCorrelationScore);

    /* test bounds of row-chunk size */
    if (n > kCompressionRowChunkMaximumSize) {
        fprintf(stderr, "Error: Number of specified rows within a chunk cannot be larger than the chunk size maximum!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* create an owner read/write/executable directory to contain block files */
    char* block_dest_dir = NULL;
    block_dest_dir = bs_init_sqr_bzip2_split_store_dir_str(s->attr->fn);
    if (bs_path_exists(block_dest_dir)) {
        fprintf(stderr, "Error: Store per-block destination [%s] exists!\n", block_dest_dir);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }    
    mkdir(block_dest_dir, S_IRUSR | S_IWUSR | S_IXUSR);

    /* init offset array */
    off_t* offsets = NULL;
    uint32_t num_offsets = floor(l->nelems / n) + 1;
    offsets = malloc(num_offsets * sizeof(*offsets));
    if (!offsets) {
        fprintf(stderr, "Error: Cannot allocate memory for offset array!\n");
        exit(EXIT_FAILURE);
    }
    uint32_t offset_idx = 0;

    /* init bzip2 machinery */
    uint32_t bz_block_size = kCompressionBzip2BlockSize100k * kCompressionBzip2BlockSizeFactor;
    uint32_t bz_uncompressed_buffer_size = bz_block_size;
    char *bz_uncompressed_buffer = NULL;
    bz_uncompressed_buffer = malloc(bz_uncompressed_buffer_size);
    if (!bz_uncompressed_buffer) {
        fprintf(stderr, "Error: Could not allocate space to bzip2 uncompressed byte buffer!\n");
        exit(EXIT_FAILURE);
    }
    BZFILE* bzf = NULL;
    int bzf_error = BZ_OK;
    uint32_t bzf_bytes_read_lo32 = 0;
    uint32_t bzf_bytes_read_hi32 = 0;
    uint32_t bzf_bytes_written_lo32 = 0;
    uint32_t bzf_bytes_written_hi32 = 0;
    uint64_t bzf_cumulative_bytes_written = 0;    

    /* iterate through l->nelems, one block of rows at a time; write a bzip2 stream for each block */
    uint32_t uncompressed_buffer_idx = 0;
    uint32_t block_idx = 0;
    char *block_dest_fn = NULL;
    for (uint32_t row_idx = 1; row_idx <= s->attr->nelems; row_idx++) {
        if ((row_idx - 1) % n == 0) {
            /* open handle to output sqr matrix store */
            block_dest_fn = bs_init_sqr_bzip2_split_store_fn_str(block_dest_dir, block_idx++);
            os = fopen(block_dest_fn, "wb");
            if (ferror(os)) {
                fprintf(stderr, "Error: Could not open handle to output square matrix store!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            free(block_dest_fn), block_dest_fn = NULL;
            bzf = BZ2_bzWriteOpen(&bzf_error, os, kCompressionBzip2BlockSize100k, kCompressionBzip2Verbosity, kCompressionBzip2WorkFactor);
            switch (bzf_error) {
            case BZ_OK:
                offsets[offset_idx++] = bzf_cumulative_bytes_written;
                break;
            case BZ_CONFIG_ERROR:
            case BZ_PARAM_ERROR:
            case BZ_IO_ERROR:
            case BZ_MEM_ERROR:
                fprintf(stderr, "Error: Could not set up new bzip2 output stream! (initial)\n");
                exit(EXIT_FAILURE);
            }
        }
        signal_t* row_signal = l->elems[(row_idx - 1)]->signal;
        for (uint32_t col_idx = 1; col_idx <= s->attr->nelems; col_idx++) {
            signal_t* col_signal = l->elems[(col_idx - 1)]->signal;
            if (row_idx != col_idx) {
                if (row_signal->n != col_signal->n) {
                    fprintf(stderr, "Error: Vectors being correlated are of unequal length!\n");
                    bs_print_signal(row_signal, stderr);
                    bs_print_signal(col_signal, stderr);
                    exit(EXIT_FAILURE);
                }
                score_t pairwise_score = NAN;
                if ((row_signal->data_contains_nan == kFalse) && (col_signal->data_contains_nan == kFalse)) {
                    if (((sf == bs_pearson_r_signal) || (sf == bs_spearman_rho_signal_v2)) && (row_signal->sd != 0.0f) && (col_signal->sd != 0.0f)) {
                        pairwise_score = (*sf)(row_signal, col_signal, row_signal->n);
                    }
                    else {
                        pairwise_score = (*sf)(row_signal, col_signal, row_signal->n);
                    }
                }
                else if (!bs_globals.zero_sd_warning_issued) {
                    fprintf(stderr, "Warning: One or more vectors contain NAN or have zero standard deviation!\n");
                    bs_globals.zero_sd_warning_issued = kTrue;
                }
                score = 
                    (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_score_to_byte(pairwise_score) : 
                    (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_score_to_byte_mqz(pairwise_score) : 
                    bs_encode_score_to_byte_custom(pairwise_score, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
            }
            else if (row_idx == col_idx) {
                score = self_correlation_score;
            }            
            bz_uncompressed_buffer[uncompressed_buffer_idx++] = score;
            if (uncompressed_buffer_idx % bz_uncompressed_buffer_size == 0) {
                BZ2_bzWrite(&bzf_error, bzf, bz_uncompressed_buffer, uncompressed_buffer_idx);
                switch (bzf_error) {
                case BZ_OK:
                    break;
                case BZ_PARAM_ERROR:
                case BZ_IO_ERROR:
                case BZ_SEQUENCE_ERROR:
                    fprintf(stderr, "Error: Could not write buffer to bzip2 output stream! (full, within row)\n");
                    exit(EXIT_FAILURE);
                }
                uncompressed_buffer_idx = 0;
            }
        }
        if (row_idx == s->attr->nelems) {
            BZ2_bzWrite(&bzf_error, bzf, bz_uncompressed_buffer, uncompressed_buffer_idx);
            switch (bzf_error) {
            case BZ_OK:
                break;
            case BZ_PARAM_ERROR:
            case BZ_IO_ERROR:
            case BZ_SEQUENCE_ERROR:
                fprintf(stderr, "Error: Could not write buffer to bzip2 output stream! (last row)\n");
                exit(EXIT_FAILURE);
            }
            uncompressed_buffer_idx = 0;
            BZ2_bzWriteClose64(&bzf_error, bzf, kCompressionBzip2AbandonPolicy, &bzf_bytes_read_lo32, &bzf_bytes_read_hi32, &bzf_bytes_written_lo32, &bzf_bytes_written_hi32);
            switch (bzf_error) {
            case BZ_OK:
                bzf_cumulative_bytes_written += ((off_t) bzf_bytes_written_hi32 << 32) + bzf_bytes_written_lo32;
                offsets[offset_idx++] = bzf_cumulative_bytes_written;
                break;
            case BZ_IO_ERROR:
            case BZ_SEQUENCE_ERROR:
                fprintf(stderr, "Error: Could not close bzip2 output stream! (last row)\n");
                exit(EXIT_FAILURE);
            }
            fclose(os);
        }
        else if (row_idx % n == 0) {
            BZ2_bzWrite(&bzf_error, bzf, bz_uncompressed_buffer, uncompressed_buffer_idx);
            switch (bzf_error) {
            case BZ_OK:
                break;
            case BZ_PARAM_ERROR:
            case BZ_IO_ERROR:
            case BZ_SEQUENCE_ERROR:
                fprintf(stderr, "Error: Could not write buffer to bzip2 output stream! (row block)\n");
                exit(EXIT_FAILURE);
            }
            uncompressed_buffer_idx = 0;
            BZ2_bzWriteClose64(&bzf_error, bzf, kCompressionBzip2AbandonPolicy, &bzf_bytes_read_lo32, &bzf_bytes_read_hi32, &bzf_bytes_written_lo32, &bzf_bytes_written_hi32);
            switch (bzf_error) {
            case BZ_OK:
                bzf_cumulative_bytes_written += ((off_t) bzf_bytes_written_hi32 << 32) + bzf_bytes_written_lo32;
                break;
            case BZ_IO_ERROR:
            case BZ_SEQUENCE_ERROR:
                fprintf(stderr, "Error: Could not close bzip2 output stream! (row block)\n");
                exit(EXIT_FAILURE);
            }
            fclose(os);
        }
    }
    
    /* convert offsets to formatted metadata string and write to output stream */
    char* md_str = NULL;
    md_str = bs_init_metadata_str(offsets, offset_idx, n, sv);
    if (!md_str) {
        fprintf(stderr, "Error: Could not generate metadata string from offsets!\n");
        exit(EXIT_FAILURE);
    }
    char* md_dest_fn = bs_init_sqr_bzip2_split_store_metadata_fn_str(block_dest_dir);
    os = fopen(md_dest_fn, "wb");
    if (ferror(os)) {
        fprintf(stderr, "Error: Could not open handle to output metadata!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    fwrite(md_str, 1, strlen(md_str), os);
    fclose(os);
    
    /* cleanup */
    free(bz_uncompressed_buffer), bz_uncompressed_buffer = NULL;
    free(offsets), offsets = NULL;
    free(block_dest_dir), block_dest_dir = NULL;
    free(md_dest_fn), md_dest_fn = NULL;
    free(md_str), md_str = NULL;
}

/**
 * @brief      bs_init_sqr_bzip2_split_store_dir_str(p)
 *
 * @details    Prints formatted string for directory name
 *             storing per-block compressed blocks given prefix
 *
 * @param      p      (char*) prefix for directory name
 *
 * @return     (char*) formatted directory string 
 */

char*
bs_init_sqr_bzip2_split_store_dir_str(char* p)
{
    char* block_dest_dir = NULL;
    if (strlen(kCompressionMetadataSplitDirSuffix) > 0) {
        block_dest_dir = malloc(strlen(p) + strlen(kCompressionMetadataSplitDirSuffix) + 2);
        if (!block_dest_dir) {
            fprintf(stderr, "Error: Could not allocate space for block destination string!\n");
            exit(EXIT_FAILURE);
        }
        snprintf(block_dest_dir, strlen(p) + strlen(kCompressionMetadataSplitDirSuffix) + 2, "%s.%s", p, kCompressionMetadataSplitDirSuffix);
    }
    else {
        block_dest_dir = malloc(strlen(p) + 1);
        if (!block_dest_dir) {
            fprintf(stderr, "Error: Could not allocate space for block destination string!\n");
            exit(EXIT_FAILURE);
        }
        snprintf(block_dest_dir, strlen(p) + 1, "%s", p);
    }
    return block_dest_dir;
}

/**
 * @brief      bs_init_sqr_bzip2_split_store_fn_str(d, i)
 *
 * @details    Prints formatted string for per-block compressed
 *             file name, given parent directory name
 *
 * @param      d      (char*) directory name
 *             i      (uint32_t) block index
 *
 * @return     (char*) formatted per-block filename string 
 */

char*
bs_init_sqr_bzip2_split_store_fn_str(char* d, uint32_t i)
{
    char* block_dest_fn = NULL;
    block_dest_fn = malloc(strlen(d) + BLOCK_STR_MAX_LEN + 6);
    if (!block_dest_fn) {
        fprintf(stderr, "Error: Could not allocate space for block destination filename string!\n");
        exit(EXIT_FAILURE);
    }
    snprintf(block_dest_fn, strlen(d) + BLOCK_STR_MAX_LEN + 6, "%s/%0*u.cbs", d, (int) BLOCK_STR_MAX_LEN, i);
    return block_dest_fn;
}

/**
 * @brief      bs_init_sqr_bzip2_split_store_metadata_fn_str(d)
 *
 * @details    Prints formatted string for metadata filename
 *             given parent directory name
 *
 * @param      d      (char*) directory name
 *
 * @return     (char*) formatted metadata filename string 
 */

char*
bs_init_sqr_bzip2_split_store_metadata_fn_str(char* d)
{
    char* md_dest_fn = NULL;
    md_dest_fn = malloc(strlen(d) + strlen(kCompressionMetadataSplitFn) + 2);
    if (!md_dest_fn) {
        fprintf(stderr, "Error: Could not allocate space for metadata destination filename string!\n");
        exit(EXIT_FAILURE);
    }
    snprintf(md_dest_fn, strlen(d) + strlen(kCompressionMetadataSplitFn) + 2, "%s/%s", d, kCompressionMetadataSplitFn);
    return md_dest_fn;
}

/**
 * @brief      bs_sqr_byte_offset_for_element_ij(n, i, j)
 *
 * @details    Returns a zero-indexed byte offset (off_t) value from a linear
 *             representation of bytes in a square matrix of order n, for 
 *             given row i and column j.
 *
 * @param      n      (uint32_t) order of square matrix
 *             i      (uint32_t) i-th row of matrix
 *             j      (uint32_t) j-th column of matrix
 *
 * @return     (off_t) byte offset into SUT byte array
 */

off_t
bs_sqr_byte_offset_for_element_ij(uint32_t n, uint32_t i, uint32_t j)
{
    return i * n + j;
}

/**
 * @brief      bs_print_sqr_store_to_bed7(l, s, m, os, rs, re)
 *
 * @details    Queries square matrix store for provided index range 
 *             globals and prints BED7 (BED3 + BED3 + floating point) 
 *             to specified output stream. The two BED3 elements 
 *             are retrieved from the lookup table and represent 
 *             a score pairing.
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             m      (boolean_t) flag to decide if query is for mutual row-col regions
 *             os     (FILE*) pointer to output stream
 *             rs     (size_t) row start index
 *             re     (size_t) row end index
 */

void
bs_print_sqr_store_to_bed7(lookup_t* l, sqr_store_t* s, boolean_t m, FILE* os, size_t rs, size_t re)
{
    byte_t* byte_buf = NULL;
    byte_buf = malloc(l->nelems);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }

    if (!bs_path_exists(s->attr->fn)) {
        fprintf(stderr, "Error: Store file [%s] does not exist!\n", s->attr->fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    FILE* is = NULL;
    is = fopen(s->attr->fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to input store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    size_t row_idx = rs;
    size_t col_idx = (m) ? rs : 0;

    /* fseek(is) to the starting offset */
    off_t start_offset = bs_sqr_byte_offset_for_element_ij(l->nelems, rs, col_idx);
    fseek(is, start_offset, SEEK_SET);

    /* the maximum column index depends on whether or not we are looking at mutual bytes */
    size_t col_idx_max = (m) ? (re + 1) : l->nelems;

    do {
        size_t items_read = 0;
        if (!m) {
            /* read a row from current block */
            items_read = fread(byte_buf, sizeof(*byte_buf), l->nelems, is);
            if (items_read != l->nelems) {
                fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%u] required)\n", items_read, l->nelems);
                exit(EXIT_FAILURE);
            }
        }
        else {
            /* read only mutual bytes from current block */
            size_t mutual_byte_diff = re - rs + 1;
            items_read = fread(byte_buf, sizeof(*byte_buf), mutual_byte_diff, is);
            if (items_read != mutual_byte_diff) {
                fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%lu] required)\n", items_read, mutual_byte_diff);
                exit(EXIT_FAILURE);
            }
        }
        do {
            if (row_idx != col_idx) {
                bs_print_pair(os, 
                              l->elems[row_idx]->chr,
                              l->elems[row_idx]->start,
                              l->elems[row_idx]->stop,
                              l->elems[col_idx]->chr,
                              l->elems[col_idx]->start,
                              l->elems[col_idx]->stop,
                              (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score(byte_buf[col_idx]) :
                              (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_score_mqz(byte_buf[col_idx]) :
                              bs_decode_byte_to_score_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max)
                              );
            }
            col_idx++;
        } while (col_idx < col_idx_max);
        row_idx++;
        col_idx = (m) ? rs : 0;

        /* if we are reading mutual-region bytes, we need to seek another line of bytes */
        /* so that the file pointer is correctly positioned for the next iteration */
        if (m) {
            fseek(is, l->nelems - 1, SEEK_CUR);
        }
    } while (row_idx <= re);

    free(byte_buf);

    fclose(is);
}

/**
 * @brief      bs_print_sqr_filtered_store_to_bed7(l, s, m, os, fc, flb, fub, fo, rs, re)
 *
 * @details    Queries square matrix store for provided index range 
 *             globals and prints BED7 (BED3 + BED3 + floating point) 
 *             to specified output stream, after filtered against the
 *             provided score cutoff. The two BED3 elements are
 *             retrieved from the lookup table and represent a
 *             score pairing.
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             m      (boolean_t) flag to decide if query is for mutual row-col regions
 *             os     (FILE*) pointer to output stream
 *             fc     (score_t) score filter cutoff
 *             flb    (score_t) score filter cutoff lower bound for ranged thresholds
 *             fub    (score_t) score filter cutoff upper bound for ranged thresholds
 *             fo     (score_filter_t) score filter operation
 *             rs     (size_t) row start index
 *             re     (size_t) row end index
 */

void
bs_print_sqr_filtered_store_to_bed7(lookup_t* l, sqr_store_t* s, boolean_t m, FILE* os, score_t fc, score_t flb, score_t fub, score_filter_t fo, size_t rs, size_t re)
{
    byte_t* byte_buf = NULL;
    byte_buf = malloc(l->nelems);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }

    if (!bs_path_exists(s->attr->fn)) {
        fprintf(stderr, "Error: Store file [%s] does not exist!\n", s->attr->fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    FILE* is = NULL;
    is = fopen(s->attr->fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to input store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    size_t row_idx = rs;
    size_t col_idx = (m) ? rs : 0;

    /* fseek(is) to the starting offset */
    off_t start_offset = bs_sqr_byte_offset_for_element_ij(l->nelems, rs, col_idx);
    fseek(is, start_offset, SEEK_SET);

    /* the maximum column index depends on whether or not we are looking at mutual bytes */
    size_t col_idx_max = (m) ? (re + 1) : l->nelems;

    do {
        size_t items_read = 0;
        if (!m) {
            /* read a row from current block */
            items_read = fread(byte_buf, sizeof(*byte_buf), l->nelems, is);
            if (items_read != l->nelems) {
                fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%u] required)\n", items_read, l->nelems);
                exit(EXIT_FAILURE);
            }
        }
        else {
            /* read only mutual bytes from current block */
            size_t mutual_byte_diff = re - rs + 1;
            items_read = fread(byte_buf, sizeof(*byte_buf), mutual_byte_diff, is);
            if (items_read != mutual_byte_diff) {
                fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%lu] required)\n", items_read, mutual_byte_diff);
                exit(EXIT_FAILURE);
            }
        }

        do {
            if (row_idx != col_idx) {
                score_t d = (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score(byte_buf[col_idx]) :
                    (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_score_mqz(byte_buf[col_idx]) :
                    bs_decode_byte_to_score_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
                if ( ((fo == kScoreFilterGtEq) && (d >= fc)) ||
                     ((fo == kScoreFilterGt) && (d > fc)) ||
                     ((fo == kScoreFilterEq) && (fabs(d - fc) < kEpsilon)) ||
                     ((fo == kScoreFilterLtEq) && (d <= fc)) ||
                     ((fo == kScoreFilterLt) && (d < fc)) ||
                     ((fo == kScoreFilterRangedWithinExclusive) && (d > flb) && (d < fub)) ||
                     ((fo == kScoreFilterRangedWithinInclusive) && (d >= flb) && (d <= fub)) ||
                     ((fo == kScoreFilterRangedOutsideExclusive) && ((d < flb) || (d > fub))) ||
                     ((fo == kScoreFilterRangedOutsideInclusive) && ((d <= flb) || (d >= fub))) ) {
                    bs_print_pair(os, 
                                  l->elems[row_idx]->chr,
                                  l->elems[row_idx]->start,
                                  l->elems[row_idx]->stop,
                                  l->elems[col_idx]->chr,
                                  l->elems[col_idx]->start,
                                  l->elems[col_idx]->stop,
                                  d);
                }
            }
            col_idx++;
        } while (col_idx < col_idx_max);
        row_idx++;
        col_idx = (m) ? rs : 0;

        /* if we are reading mutual-region bytes, we need to seek another line of bytes */
        /* so that the file pointer is correctly positioned for the next iteration */
        if (m) {
            fseek(is, l->nelems - 1, SEEK_CUR);
        }
    } while (row_idx <= re);

    free(byte_buf);

    fclose(is);
}

/**
 * @brief      bs_print_sqr_split_store_separate_rows_to_bed7(l, s, m, os, rs, re, rn)
 *
 * @details    Queries raw square matrix store folder for
 *             provided multiple-index globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output stream. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             m      (boolean_t) flag to decide if query is for mutual row-col regions
 *             os     (FILE*) pointer to output stream
 *             rs     (size_t*) pointer to list of query rows of interest (starts)
 *             re     (size_t*) pointer to list of query rows of interest (ends)
 *             rn     (uint32) number of rows of query interest
 */

void
bs_print_sqr_split_store_separate_rows_to_bed7(lookup_t* l, sqr_store_t* s, boolean_t m, FILE* os, size_t* rs, size_t* re, uint32_t rn)
{
    /* validate query row list size */
    if (rn == 0) {
        fprintf(stderr, "Error: Query row list is empty!\n");
        return;
    }
    
    /* init parent folder name for split blocks */
    char* block_src_dir = NULL;
    block_src_dir = bs_init_sqr_split_store_dir_str(s->attr->fn);
    if (!bs_path_exists(block_src_dir)) {
        fprintf(stderr, "Error: Store per-block destination [%s] does not exist!\n", block_src_dir);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    /* get metadata string and attributes */
    char* md_src_fn = bs_init_sqr_split_store_metadata_fn_str(block_src_dir);
    if (!bs_path_exists(md_src_fn)) {
        fprintf(stderr, "Error: Store per-block metadata file [%s] does not exist!\n", md_src_fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    ssize_t md_fn_size = bs_file_size(md_src_fn);
    FILE *is = NULL;
    is = fopen(md_src_fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to metadata string file!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN, SEEK_SET);
    char md_length[MD_OFFSET_MAX_LEN] = {0};
    if (fread(md_length, sizeof(*md_length), MD_OFFSET_MAX_LEN, is) != MD_OFFSET_MAX_LEN) {
        fprintf(stderr, "Error: Could not read metadata string length from tail of file! [%s]\n", md_length);
        exit(EXIT_FAILURE);
    }
    uint32_t md_string_length = 0;
    sscanf(md_length, "%u", &md_string_length);
    char *md_string = NULL;
    md_string = malloc(md_string_length);
    if (!md_string) {
        fprintf(stderr, "Error: Could not allocate space for intermediate metadata string!\n");
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN - md_string_length, SEEK_SET);
    if (fread(md_string, sizeof(*md_string), md_string_length, is) != md_string_length) {
        fprintf(stderr, "Error: Could not read metadata string innards from file!\n");
        exit(EXIT_FAILURE);
    }
    fclose(is), is = NULL;
    metadata_t* md = NULL;
    md = bs_parse_metadata_str(md_string);
    if (!md) {
        fprintf(stderr, "Error: Could not extract metadata from archive!\n");
        exit(EXIT_FAILURE);
    }
    
    /* allocate byte buffer -- we read in one row of bytes at a time */
    byte_t* byte_buf = NULL;
    ssize_t n_byte_buf = l->nelems;
    byte_buf = malloc(n_byte_buf);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }

    /* iterate through separate rows, calculating associated block */
    int32_t block_row_size = (int32_t) md->block_row_size;
    int32_t current_block_idx = -1;
    int32_t new_block_idx = -1;
    size_t row_idx = 0;
    size_t col_idx = 0;

    for (size_t r_idx = 0; r_idx < rn; r_idx++) {
        size_t first = rs[r_idx];
        size_t last = re[r_idx];
        for (size_t query_row = first; query_row <= last; ++query_row) {
            new_block_idx = (int32_t) (query_row / block_row_size); /* explicit cast */
            
            /* test if we are in a new block */
            if (new_block_idx != current_block_idx) {
                /* close current block file, if one is already open, and then open a new block */
                if (is) {
                    fclose(is), is = NULL;
                }
                is = fopen(bs_init_sqr_split_store_fn_str(block_src_dir, new_block_idx), "rb");
                if (ferror(is)) {
                    fprintf(stderr, "Error: Could not open handle to input store!\n");
                    bs_print_usage(stderr);
                    exit(EXIT_FAILURE);
                }
                /* adjust row_idx so that byte offset calculation is relative to current block */
                row_idx = block_row_size * new_block_idx;
            }

            /* we offset some number of bytes from current position of input stream, as necessary */
            /* note that we subtract a row unit, if we are in the same block and so have already */ 
            /* read through the input stream by one row. we also make sure that we use 64-bit ints */
            /* otherwise we will almost certainly overflow and run into byte offset problems that */
            /* cause garbage output */
        
            int64_t row_diff = query_row - row_idx - ((current_block_idx != new_block_idx) ? 0 : 1);
            int64_t bytes_to_go = row_diff * l->nelems;

            /* if the mutual-region flag is set, then we add "first" bytes to go into the row by another row bytes */
            if (m) {
                bytes_to_go += first;
            }
            
            /* seek to the correct location */
            if (bytes_to_go > 0) {
                fseek(is, bytes_to_go, SEEK_CUR);
            }
        
            size_t items_read = 0;
            if (!m) {
                /* read a row from current block */
                col_idx = 0;
                items_read = fread(byte_buf, sizeof(*byte_buf), l->nelems, is);
                if (items_read != l->nelems) {
                    fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%u] required)\n", items_read, l->nelems);
                    exit(EXIT_FAILURE);
                }
            }
            else {
                /* read only mutual bytes from current block */
                col_idx = first;
                size_t mutual_byte_diff = last - first + 1;
                items_read = fread(byte_buf, sizeof(*byte_buf), mutual_byte_diff, is);
                if (items_read != mutual_byte_diff) {
                    fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%lu] required)\n", items_read, mutual_byte_diff);
                    exit(EXIT_FAILURE);
                }
            }

            /* the maximum column index depends on whether or not we are looking at mutual bytes */
            size_t col_idx_max = (m) ? (last + 1) : l->nelems;

            do {
                if (row_idx != col_idx) {
                    bs_print_pair(os, 
                                  l->elems[row_idx]->chr,
                                  l->elems[row_idx]->start,
                                  l->elems[row_idx]->stop,
                                  l->elems[col_idx]->chr,
                                  l->elems[col_idx]->start,
                                  l->elems[col_idx]->stop,
                                  (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score(byte_buf[col_idx]) :
                                  (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_score_mqz(byte_buf[col_idx]) :
                                  bs_decode_byte_to_score_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max)
                                  );
                }
                col_idx++;
            } while (col_idx < col_idx_max);

            /* if we are reading mutual-region bytes, we need to seek to the end of the */
            /* row so that the file pointer is correctly positioned for the next iteration */
            if (m) {
                bytes_to_go = l->nelems - last - 1;
                fseek(is, bytes_to_go, SEEK_CUR);
            }
            
            /* set current block index */
            current_block_idx = new_block_idx;
        }
    }
    
    /* clean up */
    fclose(is), is = NULL;
    free(block_src_dir), block_src_dir = NULL;
    free(md_src_fn), md_src_fn = NULL;
    free(md_string), md_string = NULL;
    free(md), md = NULL;
    free(byte_buf), byte_buf = NULL;
}

/**
 * @brief      bs_print_sqr_split_store_separate_rows_to_bed7_buffer(l, s, m, b, rs, re, rn)
 *
 * @details    Queries raw square matrix store folder for
 *             provided multiple-index globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output buffer. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             m      (boolean_t) flag to decide if query is for mutual row-col regions
 *             b      (char**) pointer to output buffer
 *             rs     (size_t*) pointer to list of query rows of interest (starts)
 *             re     (size_t*) pointer to list of query rows of interest (ends)
 *             rn     (uint32_t) number of rows of query interest
 */

void
bs_print_sqr_split_store_separate_rows_to_bed7_via_buffer(lookup_t* l, sqr_store_t* s, boolean_t m, char** b, size_t* rs, size_t* re, uint32_t rn)
{
    /* validate query row list size */
    if (rn == 0) {
        fprintf(stderr, "Error: Query row list is empty!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    /* init parent folder name for split blocks */
    char* block_src_dir = NULL;
    block_src_dir = bs_init_sqr_split_store_dir_str(s->attr->fn);
    if (!bs_path_exists(block_src_dir)) {
        fprintf(stderr, "Error: Store per-block destination [%s] does not exist!\n", block_src_dir);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    /* get metadata string and attributes */
    char* md_src_fn = bs_init_sqr_split_store_metadata_fn_str(block_src_dir);
    if (!bs_path_exists(md_src_fn)) {
        fprintf(stderr, "Error: Store per-block metadata file [%s] does not exist!\n", md_src_fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    ssize_t md_fn_size = bs_file_size(md_src_fn);
    FILE *is = NULL;
    is = fopen(md_src_fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to metadata string file!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN, SEEK_SET);
    char md_length[MD_OFFSET_MAX_LEN] = {0};
    if (fread(md_length, sizeof(*md_length), MD_OFFSET_MAX_LEN, is) != MD_OFFSET_MAX_LEN) {
        fprintf(stderr, "Error: Could not read metadata string length from tail of file! [%s]\n", md_length);
        exit(EXIT_FAILURE);
    }
    uint32_t md_string_length = 0;
    sscanf(md_length, "%u", &md_string_length);
    char *md_string = NULL;
    md_string = malloc(md_string_length);
    if (!md_string) {
        fprintf(stderr, "Error: Could not allocate space for intermediate metadata string!\n");
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN - md_string_length, SEEK_SET);
    if (fread(md_string, sizeof(*md_string), md_string_length, is) != md_string_length) {
        fprintf(stderr, "Error: Could not read metadata string innards from file!\n");
        exit(EXIT_FAILURE);
    }
    fclose(is), is = NULL;
    metadata_t* md = NULL;
    md = bs_parse_metadata_str(md_string);
    if (!md) {
        fprintf(stderr, "Error: Could not extract metadata from archive!\n");
        exit(EXIT_FAILURE);
    }
    
    /* allocate byte buffer -- we read in one row of bytes at a time */
    byte_t* byte_buf = NULL;
    ssize_t n_byte_buf = l->nelems;
    byte_buf = malloc(n_byte_buf);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }

    /* allocate result buffer -- it is up to the caller to release this memory! */
    char* result_buf = NULL;
    size_t n_result_buf = OUTPUT_LINE_MAX_LEN;
    size_t l_result_buf = 0;
    result_buf = malloc(n_result_buf);
    if (!result_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr result buffer!\n");
        exit(EXIT_FAILURE);
    }

    /* iterate through separate rows, calculating associated block */
    int32_t block_row_size = (int32_t) md->block_row_size;
    int32_t current_block_idx = -1;
    int32_t new_block_idx = -1;
    size_t row_idx = 0;
    size_t col_idx = 0;

    for (size_t r_idx = 0; r_idx < rn; r_idx++) {
        size_t first = rs[r_idx];
        size_t last = re[r_idx];
        for (size_t query_row = first; query_row <= last; ++query_row) {
            new_block_idx = (int32_t) (query_row / block_row_size); /* explicit cast */

            /* test if we are in a new (different) block */
            if (new_block_idx != current_block_idx) {
                /* close current block file, if one is already open, and then open a new block */
                if (is) {
                    fclose(is), is = NULL;
                }
                is = fopen(bs_init_sqr_split_store_fn_str(block_src_dir, new_block_idx), "rb");
                if (ferror(is)) {
                    fprintf(stderr, "Error: Could not open handle to input store!\n");
                    bs_print_usage(stderr);
                    exit(EXIT_FAILURE);
                }
                /* adjust row_idx so that byte offset calculation is relative to current block */
                row_idx = block_row_size * new_block_idx;
            }

            /* we offset some number of bytes from current position of input stream, as necessary */
            /* note that we subtract a row unit, if we are in the same block and so have already */ 
            /* read through the input stream by one row. we also make sure that we use 64-bit ints */
            /* otherwise we will almost certainly overflow and run into byte offset problems that */
            /* cause garbage output */

            int64_t row_diff = query_row - row_idx - ((current_block_idx != new_block_idx) ? 0 : 1);
            int64_t bytes_to_go = row_diff * l->nelems;

            /* if the mutual-region flag is set, then we add "first" bytes to go into the row by another row bytes */
            if (m) {
                bytes_to_go += first;
            }
            
            /* seek to the correct location */
            if (bytes_to_go > 0) {
                fseek(is, bytes_to_go, SEEK_CUR);
            }

            size_t items_read = 0;
            row_idx = query_row;
            col_idx = 0;
            if (!m) {
                /* read a row from current block */
                items_read = fread(byte_buf, sizeof(*byte_buf), l->nelems, is);
                if (items_read != l->nelems) {
                    fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%u] required)\n", items_read, l->nelems);
                    exit(EXIT_FAILURE);
                }
            }
            else {
                /* read only mutual bytes from current block */
                col_idx = first;
                size_t mutual_byte_diff = last - first + 1;
                items_read = fread(byte_buf, sizeof(*byte_buf), mutual_byte_diff, is);
                if (items_read != mutual_byte_diff) {
                    fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%lu] required)\n", items_read, mutual_byte_diff);
                    exit(EXIT_FAILURE);
                }
            }

            /* the maximum column index depends on whether or not we are looking at mutual bytes */
            size_t col_idx_max = (m) ? (last + 1) : l->nelems;

            do {
                if (row_idx != col_idx) {
                    bs_print_pair_to_buffer(result_buf + l_result_buf,
                                            &l_result_buf,
                                            l->elems[row_idx]->chr,
                                            l->elems[row_idx]->start,
                                            l->elems[row_idx]->stop,
                                            l->elems[col_idx]->chr,
                                            l->elems[col_idx]->start,
                                            l->elems[col_idx]->stop,
                                            (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score(byte_buf[col_idx]) :
                                            (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_score_mqz(byte_buf[col_idx]) :
                                            bs_decode_byte_to_score_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max)
                                            );
                    /* resize the result buffer, if necessary */
                    if (l_result_buf + OUTPUT_LINE_MAX_LEN >= n_result_buf) {
                        char* temp_result_buf = NULL;
                        temp_result_buf = realloc(result_buf, l_result_buf + OUTPUT_LINE_MAX_LEN);
                        if (!temp_result_buf) {
                            fprintf(stderr, "Error: Could not allocate memory to sqr result reallocation buffer!\n");
                            exit(EXIT_FAILURE);
                        }
                        n_result_buf = l_result_buf + OUTPUT_LINE_MAX_LEN;
                        result_buf = temp_result_buf;
                    }
                }
                col_idx++;
            } while (col_idx < col_idx_max);

            /* if we are reading mutual-region bytes, we need to seek to the end of the */
            /* row so that the file pointer is correctly positioned for the next iteration */
            if (m) {
                bytes_to_go = l->nelems - last;
                fseek(is, bytes_to_go, SEEK_CUR);
            }
            
            /* set current block index */
            current_block_idx = new_block_idx;
        }
    }

    /* set result buffer pointer */
    *b = result_buf;
    
    /* clean up */
    fclose(is), is = NULL;
    free(block_src_dir), block_src_dir = NULL;
    free(md_src_fn), md_src_fn = NULL;
    free(md_string), md_string = NULL;
    free(md), md = NULL;
    free(byte_buf), byte_buf = NULL;
}

/**
 * @brief      bs_print_sqr_split_store_separate_rows_to_bed7_file(l, s, qf, m, os)
 *
 * @details    Queries raw square matrix store folder for
 *             provided multiple-index globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output stream. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             qf     (char*) query file name to parse
 *             m      (boolean_t) flag to decide if query is for mutual row-col regions
 *             os     (FILE*) pointer to output stream
 */

void
bs_print_sqr_split_store_separate_rows_to_bed7_file(lookup_t* l, sqr_store_t* s, char* qf, boolean_t m, FILE* os)
{
    /* init parent folder name for split blocks */
    char* block_src_dir = NULL;
    block_src_dir = bs_init_sqr_split_store_dir_str(s->attr->fn);
    if (!bs_path_exists(block_src_dir)) {
        fprintf(stderr, "Error: Store per-block destination [%s] does not exist!\n", block_src_dir);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* check that the query regions file exists */
    FILE* qs = NULL;
    qs = fopen(qf, "r");
    if (ferror(qs)) {
        fprintf(stderr, "Error: Could not open handle to query string file!\n");
        exit(EXIT_FAILURE);
    }
    
    /* get metadata string and attributes */
    char* md_src_fn = bs_init_sqr_split_store_metadata_fn_str(block_src_dir);
    if (!bs_path_exists(md_src_fn)) {
        fprintf(stderr, "Error: Store per-block metadata file [%s] does not exist!\n", md_src_fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    ssize_t md_fn_size = bs_file_size(md_src_fn);
    FILE* is = NULL;
    is = fopen(md_src_fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to metadata string file!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN, SEEK_SET);
    char md_length[MD_OFFSET_MAX_LEN] = {0};
    if (fread(md_length, sizeof(*md_length), MD_OFFSET_MAX_LEN, is) != MD_OFFSET_MAX_LEN) {
        fprintf(stderr, "Error: Could not read metadata string length from tail of file! [%s]\n", md_length);
        exit(EXIT_FAILURE);
    }
    uint32_t md_string_length = 0;
    sscanf(md_length, "%u", &md_string_length);
    char* md_string = NULL;
    md_string = malloc(md_string_length);
    if (!md_string) {
        fprintf(stderr, "Error: Could not allocate space for intermediate metadata string!\n");
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN - md_string_length, SEEK_SET);
    if (fread(md_string, sizeof(*md_string), md_string_length, is) != md_string_length) {
        fprintf(stderr, "Error: Could not read metadata string innards from file!\n");
        exit(EXIT_FAILURE);
    }
    fclose(is), is = NULL;
    metadata_t* md = NULL;
    md = bs_parse_metadata_str(md_string);
    if (!md) {
        fprintf(stderr, "Error: Could not extract metadata from archive!\n");
        exit(EXIT_FAILURE);
    }
    
    /* allocate byte buffer -- we read in one row of bytes at a time */
    byte_t* byte_buf = NULL;
    ssize_t n_byte_buf = l->nelems;
    byte_buf = malloc(n_byte_buf);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }

    int32_t block_row_size = (int32_t) md->block_row_size;
    while (!feof(qs)) {
        int32_t first = 1;
        int32_t last = 0;
        int count = 0;

        count = fscanf(qs, "%" SCNd32 "-%" SCNd32 " ", &first, &last); /* ending ' ' eats any white space */
        if (count != 2) {
            fprintf(stderr, "Warning: Found something not a range of A-B in the input index file!\n");
            break;
        } else if (first > last) {
            fprintf(stderr, "Warning: End-range ID less than start-range ID in the input index file!\n");
            break;
        } else if (last >= (int32_t) l->nelems) {
            fprintf(stderr, "Error: ID found is greater than the number of elements in the input index file!\n");
            bs_print_usage(stderr);
            exit(EXIT_FAILURE);
        }

        /* iterate through separate rows, calculating associated block */
        int32_t current_block_idx = -1;
        int32_t new_block_idx = -1;
        uint32_t row_idx = 0;
        uint32_t col_idx = 0;

        for ( int32_t query_row = first; query_row <= last; ++query_row ) {
            new_block_idx = (int32_t) (query_row / block_row_size); /* explicit cast */

            /* test if we are in a new block */
            if (new_block_idx != current_block_idx) {
                /* close current block file, if one is already open, and then open a new block */
                if (is) {
                    fclose(is), is = NULL;
                }
                is = fopen(bs_init_sqr_split_store_fn_str(block_src_dir, new_block_idx), "rb");
                if (ferror(is)) {
                    fprintf(stderr, "Error: Could not open handle to input store!\n");
                    bs_print_usage(stderr);
                    exit(EXIT_FAILURE);
                }
                /* adjust row_idx so that byte offset calculation is relative to current block */
                row_idx = block_row_size * new_block_idx;
            }

            /* we offset some number of bytes from current position of input stream, as necessary */
            /* note that we subtract a row unit, if we are in the same block and so have already */ 
            /* read through the input stream by one row. we also make sure that we use 64-bit ints */
            /* otherwise we will almost certainly overflow and run into byte offset problems that */
            /* cause garbage output */

            int64_t row_diff = query_row - row_idx - ((current_block_idx != new_block_idx) ? 0 : 1);
            int64_t bytes_to_go = row_diff * l->nelems;
            
            /* if the mutual-region flag is set, then we add "first" bytes to go into the row by another row bytes */
            if (m) {
                bytes_to_go += first;
            }
            
            /* seek to the correct location */
            if (bytes_to_go > 0) {
                fseek(is, bytes_to_go, SEEK_CUR);
            }
            
            size_t items_read = 0;
            row_idx = (uint32_t) query_row;
            col_idx = 0;
            if (!m) {
                /* read a row from current block */
                items_read = fread(byte_buf, sizeof(*byte_buf), l->nelems, is);
                if (items_read != l->nelems) {
                    fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%u] required)\n", items_read, l->nelems);
                    exit(EXIT_FAILURE);
                }
            }
            else {
                /* read only mutual bytes from current block */
                col_idx = first;
                size_t mutual_byte_diff = last - first + 1;
                items_read = fread(byte_buf, sizeof(*byte_buf), mutual_byte_diff, is);
                if (items_read != mutual_byte_diff) {
                    fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%lu] required)\n", items_read, mutual_byte_diff);
                    exit(EXIT_FAILURE);
                }
            }
            
            /* the maximum column index depends on whether or not we are looking at mutual bytes */
            uint32_t col_idx_max = (m) ? (uint32_t) (last + 1) : (uint32_t) l->nelems;

            do {
                if (row_idx != col_idx) {
                    bs_print_pair(os, 
                                  l->elems[row_idx]->chr,
                                  l->elems[row_idx]->start,
                                  l->elems[row_idx]->stop,
                                  l->elems[col_idx]->chr,
                                  l->elems[col_idx]->start,
                                  l->elems[col_idx]->stop,
                                  (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score(byte_buf[col_idx]) :
                                  (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_score_mqz(byte_buf[col_idx]) :
                                  bs_decode_byte_to_score_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max)
                                  );
                }
                col_idx++;
            } while (col_idx < col_idx_max);

            /* if we are reading mutual-region bytes, we need to seek to the end of the */
            /* row so that the file pointer is correctly positioned for the next iteration */
            if (m) {
                bytes_to_go = l->nelems - last - 1;
                fseek(is, bytes_to_go, SEEK_CUR);
            }
        
            /* set current block index */
            current_block_idx = new_block_idx;
        } /* for */
    } /* while */
    
    /* clean up */
    fclose(is);
    is = NULL;
    fclose(qs); 
    qs = NULL;
    free(block_src_dir); 
    block_src_dir = NULL;
    free(md_src_fn); 
    md_src_fn = NULL;
    free(md_string); 
    md_string = NULL;
    free(md); 
    md = NULL;
    free(byte_buf); 
    byte_buf = NULL;
}

/**
 * @brief      bs_print_sqr_split_store_separate_rows_to_bed7_file_via_buffer(l, s, qf, m, b)
 *
 * @details    Queries raw square matrix store folder for
 *             provided multiple-index globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output buffer. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             qf     (char*) query file name to parse
 *             m      (boolean_t) flag to decide if query is for mutual row-col regions
 *             b      (char**) pointer to output buffer
 */

void
bs_print_sqr_split_store_separate_rows_to_bed7_file_via_buffer(lookup_t* l, sqr_store_t* s, char* qf, boolean_t m, char** b)
{
    /* init parent folder name for split blocks */
    char* block_src_dir = NULL;
    block_src_dir = bs_init_sqr_split_store_dir_str(s->attr->fn);
    if (!bs_path_exists(block_src_dir)) {
        fprintf(stderr, "Error: Store per-block destination [%s] does not exist!\n", block_src_dir);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* check that the query regions file exists */
    FILE* qs = NULL;
    qs = fopen(qf, "r");
    if (ferror(qs)) {
        fprintf(stderr, "Error: Could not open handle to query string file!\n");
        exit(EXIT_FAILURE);
    }
    
    /* get metadata string and attributes */
    char* md_src_fn = bs_init_sqr_split_store_metadata_fn_str(block_src_dir);
    if (!bs_path_exists(md_src_fn)) {
        fprintf(stderr, "Error: Store per-block metadata file [%s] does not exist!\n", md_src_fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    ssize_t md_fn_size = bs_file_size(md_src_fn);
    FILE* is = NULL;
    is = fopen(md_src_fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to metadata string file!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN, SEEK_SET);
    char md_length[MD_OFFSET_MAX_LEN] = {0};
    if (fread(md_length, sizeof(*md_length), MD_OFFSET_MAX_LEN, is) != MD_OFFSET_MAX_LEN) {
        fprintf(stderr, "Error: Could not read metadata string length from tail of file! [%s]\n", md_length);
        exit(EXIT_FAILURE);
    }
    uint32_t md_string_length = 0;
    sscanf(md_length, "%u", &md_string_length);
    char* md_string = NULL;
    md_string = malloc(md_string_length);
    if (!md_string) {
        fprintf(stderr, "Error: Could not allocate space for intermediate metadata string!\n");
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN - md_string_length, SEEK_SET);
    if (fread(md_string, sizeof(*md_string), md_string_length, is) != md_string_length) {
        fprintf(stderr, "Error: Could not read metadata string innards from file!\n");
        exit(EXIT_FAILURE);
    }
    fclose(is), is = NULL;
    metadata_t* md = NULL;
    md = bs_parse_metadata_str(md_string);
    if (!md) {
        fprintf(stderr, "Error: Could not extract metadata from archive!\n");
        exit(EXIT_FAILURE);
    }
    
    /* allocate byte buffer -- we read in one row of bytes at a time */
    byte_t* byte_buf = NULL;
    size_t n_byte_buf = l->nelems;
    byte_buf = malloc(n_byte_buf);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }

    /* allocate result buffer -- it is up to the caller to release this memory! */
    char* result_buf = NULL;
    size_t n_result_buf = OUTPUT_LINE_MAX_LEN;
    size_t l_result_buf = 0;
    result_buf = malloc(n_result_buf);
    if (!result_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr result buffer!\n");
        exit(EXIT_FAILURE);
    }

    int32_t block_row_size = (int32_t) md->block_row_size;
    while (!feof(qs)) {
        size_t first = 1;
        size_t last = 0;
        int count = 0;

        count = fscanf(qs, "%lu-%lu ", &first, &last); /* ending ' ' eats any white space */
        if (count != 2) {
            fprintf(stderr, "Warning: Found something not a range of A-B in the input index file!\n");
            break;
        } else if (first > last) {
            fprintf(stderr, "Warning: End-range ID less than start-range ID in the input index file!\n");
            break;
        } else if (last >= l->nelems) {
            fprintf(stderr, "Error: ID found is greater than the number of elements in the input index file!\n");
            bs_print_usage(stderr);
            exit(EXIT_FAILURE);
        }

        /* iterate through separate rows, calculating associated block */
        int32_t current_block_idx = -1;
        int32_t new_block_idx = -1;
        size_t row_idx = 0;
        size_t col_idx = 0;

        for (size_t query_row = first; query_row <= last; ++query_row) {
            new_block_idx = (int32_t) (query_row / block_row_size); /* explicit cast */

            /* test if we are in a new block */
            if (new_block_idx > current_block_idx) {
                /* close current block file, if one is already open, and then open a new block */
                if (is) {
                    fclose(is), is = NULL;
                }
                is = fopen(bs_init_sqr_split_store_fn_str(block_src_dir, new_block_idx), "rb");
                if (ferror(is)) {
                    fprintf(stderr, "Error: Could not open handle to input store!\n");
                    bs_print_usage(stderr);
                    exit(EXIT_FAILURE);
                }
                /* adjust row_idx so that byte offset calculation is relative to current block */
                row_idx = block_row_size * new_block_idx;
            }

            /* we offset some number of bytes from current position of input stream, as necessary */
            /* note that we subtract a row unit, if we are in the same block and so have already */ 
            /* read through the input stream by one row. we also make sure that we use 64-bit ints */
            /* otherwise we will almost certainly overflow and run into byte offset problems that */
            /* cause garbage output */

            int64_t row_diff = query_row - row_idx - ((current_block_idx != new_block_idx) ? 0 : 1);
            int64_t bytes_to_go = row_diff * l->nelems;

            /* if the mutual-region flag is set, then we add "first" bytes to go into the row by another row bytes */
            if (m) {
                bytes_to_go += first;
            }

            if (bytes_to_go > 0) {
                fseek(is, bytes_to_go, SEEK_CUR);
            }

            size_t items_read = 0;
            row_idx = (uint32_t) query_row;
            col_idx = 0;
            if (!m) {
                /* read a row from current block */
                items_read = fread(byte_buf, sizeof(*byte_buf), l->nelems, is);
                if (items_read != l->nelems) {
                    fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%u] required)\n", items_read, l->nelems);
                    exit(EXIT_FAILURE);
                }
            }
            else {
                /* read mutual bytes from current block */
                col_idx = first;
                size_t mutual_byte_diff = last - first + 1;
                items_read = fread(byte_buf, sizeof(*byte_buf), mutual_byte_diff, is);
                if (items_read != mutual_byte_diff) {
                    fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%lu] required)\n", items_read, mutual_byte_diff);
                    exit(EXIT_FAILURE);
                }
            }

            //fprintf(stderr, "offset rows -> row_idx [%u] col_idx [%u]\n", row_idx, col_idx);

            /* the maximum column index depends on whether or not we are looking at mutual bytes */
            size_t col_idx_max = (m) ? (last + 1) : l->nelems;

            do {
                if (row_idx != col_idx) {
                    bs_print_pair_to_buffer(result_buf + l_result_buf,
                                            &l_result_buf,
                                            l->elems[row_idx]->chr,
                                            l->elems[row_idx]->start,
                                            l->elems[row_idx]->stop,
                                            l->elems[col_idx]->chr,
                                            l->elems[col_idx]->start,
                                            l->elems[col_idx]->stop,
                                            (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score(byte_buf[col_idx]) :
                                            (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_score_mqz(byte_buf[col_idx]) :
                                            bs_decode_byte_to_score_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max)
                                            );
                    /* resize the result buffer, if necessary */
                    if (l_result_buf + OUTPUT_LINE_MAX_LEN >= n_result_buf) {
                        char* temp_result_buf = NULL;
                        temp_result_buf = realloc(result_buf, l_result_buf + OUTPUT_LINE_MAX_LEN);
                        if (!temp_result_buf) {
                            fprintf(stderr, "Error: Could not allocate memory to sqr result reallocation buffer!\n");
                            exit(EXIT_FAILURE);
                        }
                        n_result_buf = l_result_buf + OUTPUT_LINE_MAX_LEN;
                        result_buf = temp_result_buf;
                    }
                }
                col_idx++;
            } while (col_idx < col_idx_max);

            /* if we are reading mutual-region bytes, we need to seek to the end of the */
            /* row so that the file pointer is correctly positioned for the next iteration */
            if (m) {
                bytes_to_go = l->nelems - last - 1;
                fseek(is, bytes_to_go, SEEK_CUR);
            }
        
            /* set current block index */
            current_block_idx = new_block_idx;
        } /* for */
    } /* while */

    /* set result buffer pointer */
    *b = result_buf;
    
    /* clean up */
    fclose(is); 
    is = NULL;
    fclose(qs); 
    qs = NULL;
    free(block_src_dir); 
    block_src_dir = NULL;
    free(md_src_fn); 
    md_src_fn = NULL;
    free(md_string); 
    md_string = NULL;
    free(md); 
    md = NULL;
    free(byte_buf); 
    byte_buf = NULL;
}

/**
 * @brief      bs_print_sqr_filtered_split_store_separate_rows_to_bed7_file(l, s, qf, m, os, fc, flb, fub, fo)
 *
 * @details    Queries raw square matrix store folder for
 *             provided multiple-index globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output stream. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             qf     (char*) query file name to parse
 *             m      (boolean_t) flag to decide if query is for mutual row-col regions
 *             os     (FILE*) pointer to output stream
 *             fc     (score_t) score filter cutoff
 *             flb    (score_t) score filter cutoff lower bound for ranged thresholds
 *             fub    (score_t) score filter cutoff upper bound for ranged thresholds
 *             fo     (score_filter_t) score filter operation
 */

void
bs_print_sqr_filtered_split_store_separate_rows_to_bed7_file(lookup_t* l, sqr_store_t* s, char* qf, boolean_t m, FILE* os, score_t fc, score_t flb, score_t fub, score_filter_t fo)
{
    /* init parent folder name for split blocks */
    char* block_src_dir = NULL;
    block_src_dir = bs_init_sqr_split_store_dir_str(s->attr->fn);
    if (!bs_path_exists(block_src_dir)) {
        fprintf(stderr, "Error: Store per-block destination [%s] does not exist!\n", block_src_dir);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* check that the query regions file exists */
    FILE* qs = NULL;
    qs = fopen(qf, "r");
    if (ferror(qs)) {
        fprintf(stderr, "Error: Could not open handle to query string file!\n");
        exit(EXIT_FAILURE);
    }
    
    /* get metadata string and attributes */
    char* md_src_fn = bs_init_sqr_split_store_metadata_fn_str(block_src_dir);
    if (!bs_path_exists(md_src_fn)) {
        fprintf(stderr, "Error: Store per-block metadata file [%s] does not exist!\n", md_src_fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    ssize_t md_fn_size = bs_file_size(md_src_fn);
    FILE* is = NULL;
    is = fopen(md_src_fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to metadata string file!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN, SEEK_SET);
    char md_length[MD_OFFSET_MAX_LEN] = {0};
    if (fread(md_length, sizeof(*md_length), MD_OFFSET_MAX_LEN, is) != MD_OFFSET_MAX_LEN) {
        fprintf(stderr, "Error: Could not read metadata string length from tail of file! [%s]\n", md_length);
        exit(EXIT_FAILURE);
    }
    uint32_t md_string_length = 0;
    sscanf(md_length, "%u", &md_string_length);
    char* md_string = NULL;
    md_string = malloc(md_string_length);
    if (!md_string) {
        fprintf(stderr, "Error: Could not allocate space for intermediate metadata string!\n");
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN - md_string_length, SEEK_SET);
    if (fread(md_string, sizeof(*md_string), md_string_length, is) != md_string_length) {
        fprintf(stderr, "Error: Could not read metadata string innards from file!\n");
        exit(EXIT_FAILURE);
    }
    fclose(is), is = NULL;
    metadata_t* md = NULL;
    md = bs_parse_metadata_str(md_string);
    if (!md) {
        fprintf(stderr, "Error: Could not extract metadata from archive!\n");
        exit(EXIT_FAILURE);
    }
    
    /* allocate byte buffer -- we read in one row of bytes at a time */
    byte_t* byte_buf = NULL;
    ssize_t n_byte_buf = l->nelems;
    byte_buf = malloc(n_byte_buf);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }

    int32_t block_row_size = (int32_t) md->block_row_size;

    while (!feof(qs)) {
        int32_t first = 1;
        int32_t last = 0;
        int count = 0;

        count = fscanf(qs, "%" SCNd32 "-%" SCNd32 "\n", &first, &last);
        if (count != 2) {
            fprintf(stderr, "Warning: Found something not a range of A-B in the input index file!\n");
            break;
        } else if (first > last) {
            fprintf(stderr, "Warning: End-range ID less than start-range ID in the input index file!\n");
            break;
        } else if (last >= (int32_t) l->nelems) {
            fprintf(stderr, "Error: ID found is greater than the number of elements in the input index file!\n");
            bs_print_usage(stderr);
            exit(EXIT_FAILURE);
        }

        /* iterate through separate rows, calculating associated block */
        int32_t current_block_idx = -1;
        int32_t new_block_idx = -1;
        uint32_t row_idx = 0;
        uint32_t col_idx = 0;

        for (int32_t query_row = first; query_row <= last; ++query_row) {
            new_block_idx = (int32_t) (query_row / block_row_size); /* explicit cast */

            /* test if we are in a new block */
            if (new_block_idx > current_block_idx) {
                /* close current block file, if one is already open, and then open a new block */
                if (is) {
                    fclose(is), is = NULL;
                }
                is = fopen(bs_init_sqr_split_store_fn_str(block_src_dir, new_block_idx), "rb");
                if (ferror(is)) {
                    fprintf(stderr, "Error: Could not open handle to input store!\n");
                    bs_print_usage(stderr);
                    exit(EXIT_FAILURE);
                }
                /* adjust row_idx so that byte offset calculation is relative to current block */
                row_idx = block_row_size * new_block_idx;
            }

            /* we offset some number of bytes from current position of input stream, as necessary */
            /* note that we subtract a row unit, if we are in the same block and so have already */ 
            /* read through the input stream by one row. we also make sure that we use 64-bit ints */
            /* otherwise we will almost certainly overflow and run into byte offset problems that */
            /* cause garbage output */

            int64_t row_diff = query_row - row_idx - ((current_block_idx != new_block_idx) ? 0 : 1);
            int64_t bytes_to_go = row_diff * l->nelems;

            /* if the mutual-region flag is set, then we add "first" bytes to go into the row by another row bytes */
            if (m) {
                bytes_to_go += first;
            }
            
            /* seek to the correct location */
            if (bytes_to_go > 0) {
                fseek(is, bytes_to_go, SEEK_CUR);
            }

            size_t items_read = 0;
            row_idx = (uint32_t) query_row;
            col_idx = 0;
            if (!m) {
                /* read a row from current block */
                items_read = fread(byte_buf, sizeof(*byte_buf), l->nelems, is);
                if (items_read != l->nelems) {
                    fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%u] required)\n", items_read, l->nelems);
                    exit(EXIT_FAILURE);
                }
            }
            else {
                /* read only mutual bytes from current block */
                col_idx = first;
                size_t mutual_byte_diff = last - first + 1;
                items_read = fread(byte_buf, sizeof(*byte_buf), mutual_byte_diff, is);
                if (items_read != mutual_byte_diff) {
                    fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%lu] required)\n", items_read, mutual_byte_diff);
                    exit(EXIT_FAILURE);
                }
            }

            /* the maximum column index depends on whether or not we are looking at mutual bytes */
            uint32_t col_idx_max = (m) ? (uint32_t) (last + 1) : (uint32_t) l->nelems;

            do {
                if (row_idx != col_idx) {
                    score_t d = (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score(byte_buf[col_idx]) :
                        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_score_mqz(byte_buf[col_idx]) :
                        bs_decode_byte_to_score_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
                    if ( ((fo == kScoreFilterGtEq) && (d >= fc)) ||
                         ((fo == kScoreFilterGt) && (d > fc)) ||
                         ((fo == kScoreFilterEq) && (fabs(d - fc) < kEpsilon)) ||
                         ((fo == kScoreFilterLtEq) && (d <= fc)) ||
                         ((fo == kScoreFilterLt) && (d < fc)) ||
                         ((fo == kScoreFilterRangedWithinExclusive) && (d > flb) && (d < fub)) ||
                         ((fo == kScoreFilterRangedWithinInclusive) && (d >= flb) && (d <= fub)) ||
                         ((fo == kScoreFilterRangedOutsideExclusive) && ((d < flb) || (d > fub))) ||
                         ((fo == kScoreFilterRangedOutsideInclusive) && ((d <= flb) || (d >= fub))) ) {
                        bs_print_pair(os, 
                                      l->elems[row_idx]->chr,
                                      l->elems[row_idx]->start,
                                      l->elems[row_idx]->stop,
                                      l->elems[col_idx]->chr,
                                      l->elems[col_idx]->start,
                                      l->elems[col_idx]->stop,
                                      (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score(byte_buf[col_idx]) :
                                      (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_score_mqz(byte_buf[col_idx]) :
                                      bs_decode_byte_to_score_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max)
                                      );
                    }
                }
                col_idx++;
            } while (col_idx < col_idx_max);

            /* if we are reading mutual-region bytes, we need to seek to the end of the */
            /* row so that the file pointer is correctly positioned for the next iteration */
            if (m) {
                bytes_to_go = l->nelems - last - 1;
                fseek(is, bytes_to_go, SEEK_CUR);
            }

            /* set current block index */
            current_block_idx = new_block_idx;
        } /* for */
    } /* while */
    
    /* clean up */
    fclose(is); 
    is = NULL;
    fclose(qs);
    qs = NULL;
    free(block_src_dir); 
    block_src_dir = NULL;
    free(md_src_fn); 
    md_src_fn = NULL;
    free(md_string); 
    md_string = NULL;
    free(md); 
    md = NULL;
    free(byte_buf); 
    byte_buf = NULL;
}

/**
 * @brief      bs_print_sqr_filtered_split_store_separate_rows_to_bed7_file_via_buffer(l, s, qf, m, b, fc, flb, fub, fo)
 *
 * @details    Queries raw square matrix store folder for
 *             provided multiple-index globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output buffer. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             qf     (char*) query file name to parse
 *             m      (boolean_t) flag to decide if query is for mutual row-col regions
 *             b      (char**) pointer to output buffer
 *             fc     (score_t) score filter cutoff
 *             flb    (score_t) score filter cutoff lower bound for ranged thresholds
 *             fub    (score_t) score filter cutoff upper bound for ranged thresholds
 *             fo     (score_filter_t) score filter operation
 */

void
bs_print_sqr_filtered_split_store_separate_rows_to_bed7_file_via_buffer(lookup_t* l, sqr_store_t* s, char* qf, boolean_t m, char** b, score_t fc, score_t flb, score_t fub, score_filter_t fo)
{
    /* init parent folder name for split blocks */
    char* block_src_dir = NULL;
    block_src_dir = bs_init_sqr_split_store_dir_str(s->attr->fn);
    if (!bs_path_exists(block_src_dir)) {
        fprintf(stderr, "Error: Store per-block destination [%s] does not exist!\n", block_src_dir);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* check that the query regions file exists */
    FILE* qs = NULL;
    qs = fopen(qf, "r");
    if (ferror(qs)) {
        fprintf(stderr, "Error: Could not open handle to query string file!\n");
        exit(EXIT_FAILURE);
    }
    
    /* get metadata string and attributes */
    char* md_src_fn = bs_init_sqr_split_store_metadata_fn_str(block_src_dir);
    if (!bs_path_exists(md_src_fn)) {
        fprintf(stderr, "Error: Store per-block metadata file [%s] does not exist!\n", md_src_fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    ssize_t md_fn_size = bs_file_size(md_src_fn);
    FILE* is = NULL;
    is = fopen(md_src_fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to metadata string file!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN, SEEK_SET);
    char md_length[MD_OFFSET_MAX_LEN] = {0};
    if (fread(md_length, sizeof(*md_length), MD_OFFSET_MAX_LEN, is) != MD_OFFSET_MAX_LEN) {
        fprintf(stderr, "Error: Could not read metadata string length from tail of file! [%s]\n", md_length);
        exit(EXIT_FAILURE);
    }
    uint32_t md_string_length = 0;
    sscanf(md_length, "%u", &md_string_length);
    char* md_string = NULL;
    md_string = malloc(md_string_length);
    if (!md_string) {
        fprintf(stderr, "Error: Could not allocate space for intermediate metadata string!\n");
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN - md_string_length, SEEK_SET);
    if (fread(md_string, sizeof(*md_string), md_string_length, is) != md_string_length) {
        fprintf(stderr, "Error: Could not read metadata string innards from file!\n");
        exit(EXIT_FAILURE);
    }
    fclose(is), is = NULL;
    metadata_t* md = NULL;
    md = bs_parse_metadata_str(md_string);
    if (!md) {
        fprintf(stderr, "Error: Could not extract metadata from archive!\n");
        exit(EXIT_FAILURE);
    }
    
    /* allocate byte buffer -- we read in one row of bytes at a time */
    byte_t* byte_buf = NULL;
    ssize_t n_byte_buf = l->nelems;
    byte_buf = malloc(n_byte_buf);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }

    /* allocate result buffer -- it is up to the caller to release this memory! */
    char* result_buf = NULL;
    size_t n_result_buf = OUTPUT_LINE_MAX_LEN;
    size_t l_result_buf = 0;
    result_buf = malloc(n_result_buf);
    if (!result_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr result buffer!\n");
        exit(EXIT_FAILURE);
    }

    int32_t block_row_size = (int32_t) md->block_row_size;
    while (!feof(qs)) {
        size_t first = 1;
        size_t last = 0;
        int count = 0;

        count = fscanf(qs, "%lu-%lu\n", &first, &last);
        if (count != 2) {
            fprintf(stderr, "Warning: Found something not a range of A-B in the input index file!\n");
            break;
        } else if (first > last) {
            fprintf(stderr, "Warning: End-range ID less than start-range ID in the input index file!\n");
            break;
        } else if (last >= l->nelems) {
            fprintf(stderr, "Error: ID found is greater than the number of elements in the input index file!\n");
            bs_print_usage(stderr);
            exit(EXIT_FAILURE);
        }

        /* iterate through separate rows, calculating associated block */
        int32_t current_block_idx = -1;
        int32_t new_block_idx = -1;
        size_t row_idx = 0;
        size_t col_idx = 0;

        for (size_t query_row = first; query_row <= last; ++query_row) {

            new_block_idx = (int32_t) (query_row / block_row_size); /* explicit cast */

            /* test if we are in a new block */
            if (new_block_idx > current_block_idx) {
                /* close current block file, if one is already open, and then open a new block */
                if (is) {
                    fclose(is), is = NULL;
                }
                is = fopen(bs_init_sqr_split_store_fn_str(block_src_dir, new_block_idx), "rb");
                if (ferror(is)) {
                    fprintf(stderr, "Error: Could not open handle to input store!\n");
                    bs_print_usage(stderr);
                    exit(EXIT_FAILURE);
                }
                /* adjust row_idx so that byte offset calculation is relative to current block */
                row_idx = block_row_size * new_block_idx;
            }

            /* we offset some number of bytes from current position of input stream, as necessary */
            /* note that we subtract a row unit, if we are in the same block and so have already */ 
            /* read through the input stream by one row. we also make sure that we use 64-bit ints */
            /* otherwise we will almost certainly overflow and run into byte offset problems that */
            /* cause garbage output */

            int64_t row_diff = query_row - row_idx - ((current_block_idx != new_block_idx) ? 0 : 1);
            int64_t bytes_to_go = row_diff * l->nelems;

            /* if the mutual-region flag is set, then we add "first" bytes to go into the row by another row bytes */
            if (m) {
                bytes_to_go += first;
            }
            
            /* seek to the correct location */
            if (bytes_to_go > 0) {
                fseek(is, bytes_to_go, SEEK_CUR);
            }

            size_t items_read = 0;
            row_idx = (uint32_t) query_row;
            col_idx = 0;
            if (!m) {
                /* read a row from current block */
                items_read = fread(byte_buf, sizeof(*byte_buf), l->nelems, is);
                if (items_read != l->nelems) {
                    fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%u] required)\n", items_read, l->nelems);
                    exit(EXIT_FAILURE);
                }
            }
            else {
                /* read only mutual bytes from current block */
                col_idx = first;
                size_t mutual_byte_diff = last - first + 1;
                items_read = fread(byte_buf, sizeof(*byte_buf), mutual_byte_diff, is);
                if (items_read != mutual_byte_diff) {
                    fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%lu] required)\n", items_read, mutual_byte_diff);
                    exit(EXIT_FAILURE);
                }
            }

            /* the maximum column index depends on whether or not we are looking at mutual bytes */
            size_t col_idx_max = (m) ? (last + 1) : l->nelems;

            do {
                if (row_idx != col_idx) {
                    score_t d = (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score(byte_buf[col_idx]) :
                        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_score_mqz(byte_buf[col_idx]) :
                        bs_decode_byte_to_score_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
                    if ( ((fo == kScoreFilterGtEq) && (d >= fc)) ||
                         ((fo == kScoreFilterGt) && (d > fc)) ||
                         ((fo == kScoreFilterEq) && (fabs(d - fc) < kEpsilon)) ||
                         ((fo == kScoreFilterLtEq) && (d <= fc)) ||
                         ((fo == kScoreFilterLt) && (d < fc)) ||
                         ((fo == kScoreFilterRangedWithinExclusive) && (d > flb) && (d < fub)) ||
                         ((fo == kScoreFilterRangedWithinInclusive) && (d >= flb) && (d <= fub)) ||
                         ((fo == kScoreFilterRangedOutsideExclusive) && ((d < flb) || (d > fub))) ||
                         ((fo == kScoreFilterRangedOutsideInclusive) && ((d <= flb) || (d >= fub))) ) {
                        bs_print_pair_to_buffer(result_buf + l_result_buf,
                                                &l_result_buf,
                                                l->elems[row_idx]->chr,
                                                l->elems[row_idx]->start,
                                                l->elems[row_idx]->stop,
                                                l->elems[col_idx]->chr,
                                                l->elems[col_idx]->start,
                                                l->elems[col_idx]->stop,
                                                (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score(byte_buf[col_idx]) :
                                                (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_score_mqz(byte_buf[col_idx]) :
                                                bs_decode_byte_to_score_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max)
                                                );
                        /* resize the result buffer, if necessary */
                        if (l_result_buf + OUTPUT_LINE_MAX_LEN >= n_result_buf) {
                            char* temp_result_buf = NULL;
                            temp_result_buf = realloc(result_buf, l_result_buf + OUTPUT_LINE_MAX_LEN);
                            if (!temp_result_buf) {
                                fprintf(stderr, "Error: Could not allocate memory to sqr result reallocation buffer!\n");
                                exit(EXIT_FAILURE);
                            }
                            n_result_buf = l_result_buf + OUTPUT_LINE_MAX_LEN;
                            result_buf = temp_result_buf;
                        }
                    }
                }
                col_idx++;
            } while (col_idx < col_idx_max);

            /* if we are reading mutual-region bytes, we need to seek to the end of the */
            /* row so that the file pointer is correctly positioned for the next iteration */
            if (m) {
                bytes_to_go = l->nelems - last - 1;
                fseek(is, bytes_to_go, SEEK_CUR);
            }

            /* set current block index */
            current_block_idx = new_block_idx;
        } /* for */
    } /* while */
    
    /* set result buffer pointer */
    *b = result_buf;

    /* clean up */
    fclose(is); 
    is = NULL;
    fclose(qs); 
    qs = NULL;
    free(block_src_dir); 
    block_src_dir = NULL;
    free(md_src_fn); 
    md_src_fn = NULL;
    free(md_string); 
    md_string = NULL;
    free(md); 
    md = NULL;
    free(byte_buf); 
    byte_buf = NULL;
}

/**
 * @brief      bs_print_sqr_filtered_split_store_separate_rows_to_bed7(l, s, os, rs, re, rn, fc, flb, fub, fo)
 *
 * @details    Queries raw split square matrix store for 
 *             provided multiple-index globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output stream. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             os     (FILE*) pointer to output stream
 *             rs     (size_t*) pointer to list of query rows of interest (starts)
 *             re     (size_t*) pointer to list of query rows of interest (ends)
 *             rn     (uint32_t) number of query rows of interest
 *             fc     (score_t) score filter cutoff
 *             flb    (score_t) score filter cutoff lower bound when ranged thresholds
 *             fub    (score_t) score filter cutoff upper bound when ranged thresholds
 *             fo     (score_filter_t) score filter operation
 */

void
bs_print_sqr_filtered_split_store_separate_rows_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os, size_t* rs, size_t* re, uint32_t rn, score_t fc, score_t flb, score_t fub, score_filter_t fo) 
{
    /* validate query row list size */
    if (rn == 0) {
        fprintf(stderr, "Error: Query row list is empty!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    /* init parent folder name for split blocks */
    char* block_src_dir = NULL;
    block_src_dir = bs_init_sqr_split_store_dir_str(s->attr->fn);
    if (!bs_path_exists(block_src_dir)) {
        fprintf(stderr, "Error: Store per-block destination [%s] does not exist!\n", block_src_dir);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    /* get metadata string and attributes */
    char* md_src_fn = bs_init_sqr_split_store_metadata_fn_str(block_src_dir);
    if (!bs_path_exists(md_src_fn)) {
        fprintf(stderr, "Error: Store per-block metadata file [%s] does not exist!\n", md_src_fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    ssize_t md_fn_size = bs_file_size(md_src_fn);
    FILE *is = NULL;
    is = fopen(md_src_fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to metadata string file!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN, SEEK_SET);
    char md_length[MD_OFFSET_MAX_LEN] = {0};
    if (fread(md_length, sizeof(*md_length), MD_OFFSET_MAX_LEN, is) != MD_OFFSET_MAX_LEN) {
        fprintf(stderr, "Error: Could not read metadata string length from tail of file! [%s]\n", md_length);
        exit(EXIT_FAILURE);
    }
    uint32_t md_string_length = 0;
    sscanf(md_length, "%u", &md_string_length);
    char *md_string = NULL;
    md_string = malloc(md_string_length);
    if (!md_string) {
        fprintf(stderr, "Error: Could not allocate space for intermediate metadata string!\n");
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN - md_string_length, SEEK_SET);
    if (fread(md_string, sizeof(*md_string), md_string_length, is) != md_string_length) {
        fprintf(stderr, "Error: Could not read metadata string innards from file!\n");
        exit(EXIT_FAILURE);
    }
    fclose(is), is = NULL;
    metadata_t* md = NULL;
    md = bs_parse_metadata_str(md_string);
    if (!md) {
        fprintf(stderr, "Error: Could not extract metadata from archive!\n");
        exit(EXIT_FAILURE);
    }
    
    /* allocate byte buffer -- we read in one row of bytes at a time */
    byte_t* byte_buf = NULL;
    ssize_t n_byte_buf = l->nelems;
    byte_buf = malloc(n_byte_buf);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }
    
    /* iterate through separate rows, calculating associated block */
    int32_t block_row_size = (int32_t) md->block_row_size;
    int32_t current_block_idx = -1;
    int32_t new_block_idx = -1;
    size_t row_idx = 0;
    size_t col_idx = 0;

    for (uint32_t r_idx = 0; r_idx < rn; r_idx++) {
        size_t first = rs[r_idx];
        size_t last = re[r_idx];
        for (size_t query_row = first; query_row <= last; ++query_row) {

            new_block_idx = (int32_t) (query_row / block_row_size); /* explicit cast */
        
            /* test if we are in a new block */
            if (new_block_idx != current_block_idx) {
                /* close current block file, if one is already open, and then open a new block */
                if (is) {
                    fclose(is), is = NULL;
                }
                is = fopen(bs_init_sqr_split_store_fn_str(block_src_dir, new_block_idx), "rb");
                if (ferror(is)) {
                    fprintf(stderr, "Error: Could not open handle to input store!\n");
                    bs_print_usage(stderr);
                    exit(EXIT_FAILURE);
                }
                /* adjust row_idx so that byte offset calculation is relative to current block */
                row_idx = block_row_size * new_block_idx;
            }
            /* offset some number of bytes from current position of is, if necessary */
            /* note that we subtract a row unit, if we have already read through the input stream by one row */
            int64_t row_diff = query_row - row_idx - ((current_block_idx != new_block_idx) ? 0 : 1);
            int64_t bytes_to_go = row_diff * l->nelems;
            if (bytes_to_go > 0) {
                fseek(is, bytes_to_go, SEEK_CUR);
            }
            /* read a row from current block and print its signal to the output stream os */
            row_idx = query_row;
            col_idx = 0;
            size_t items_read = fread(byte_buf, sizeof(*byte_buf), l->nelems, is);
            if (items_read != l->nelems) {
                fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%u] required)\n", items_read, l->nelems);
                exit(EXIT_FAILURE);
            }
            do {
                if (row_idx != col_idx) {
                    score_t d = (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score(byte_buf[col_idx]) :
                        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_score_mqz(byte_buf[col_idx]) :
                        bs_decode_byte_to_score_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
                    if ( ((fo == kScoreFilterGtEq) && (d >= fc)) ||
                         ((fo == kScoreFilterGt) && (d > fc)) ||
                         ((fo == kScoreFilterEq) && (fabs(d - fc) < kEpsilon)) ||
                         ((fo == kScoreFilterLtEq) && (d <= fc)) ||
                         ((fo == kScoreFilterLt) && (d < fc)) ||
                         ((fo == kScoreFilterRangedWithinExclusive) && (d > flb) && (d < fub)) ||
                         ((fo == kScoreFilterRangedWithinInclusive) && (d >= flb) && (d <= fub)) ||
                         ((fo == kScoreFilterRangedOutsideExclusive) && ((d < flb) || (d > fub))) ||
                         ((fo == kScoreFilterRangedOutsideInclusive) && ((d <= flb) || (d >= fub))) ) {
                        bs_print_pair(os, 
                                      l->elems[row_idx]->chr,
                                      l->elems[row_idx]->start,
                                      l->elems[row_idx]->stop,
                                      l->elems[col_idx]->chr,
                                      l->elems[col_idx]->start,
                                      l->elems[col_idx]->stop,
                                      d);
                    }
                }
                col_idx++;
            } while (col_idx < l->nelems);
            
            /* set current block index */
            current_block_idx = new_block_idx;
        }
    }
    
    /* clean up */
    fclose(is), is = NULL;
    free(block_src_dir), block_src_dir = NULL;
    free(md_src_fn), md_src_fn = NULL;
    free(md_string), md_string = NULL;
    free(md), md = NULL;
    free(byte_buf), byte_buf = NULL;    
}

/**
 * @brief      bs_print_sqr_filtered_split_store_separate_rows_to_bed7_via_buffer(l, s, b, rs, re, rn, fc, flb, fub, fo)
 *
 * @details    Queries raw split square matrix store for 
 *             provided multiple-index globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output buffer. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             b      (char**) pointer to output buffer
 *             rs     (size_t*) pointer to list of query rows of interest (starts)
 *             re     (size_t*) pointer to list of query rows of interest (ends)
 *             rn     (uint32_t) number of query rows of interest
 *             fc     (score_t) score filter cutoff
 *             flb    (score_t) score filter cutoff lower bound when ranged thresholds
 *             fub    (score_t) score filter cutoff upper bound when ranged thresholds
 *             fo     (score_filter_t) score filter operation
 */

void
bs_print_sqr_filtered_split_store_separate_rows_to_bed7_via_buffer(lookup_t* l, sqr_store_t* s, char** b, size_t* rs, size_t* re, uint32_t rn, score_t fc, score_t flb, score_t fub, score_filter_t fo) 
{
    /* validate query row list size */
    if (rn == 0) {
        fprintf(stderr, "Error: Query row list is empty!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    /* init parent folder name for split blocks */
    char* block_src_dir = NULL;
    block_src_dir = bs_init_sqr_split_store_dir_str(s->attr->fn);
    if (!bs_path_exists(block_src_dir)) {
        fprintf(stderr, "Error: Store per-block destination [%s] does not exist!\n", block_src_dir);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    /* get metadata string and attributes */
    char* md_src_fn = bs_init_sqr_split_store_metadata_fn_str(block_src_dir);
    if (!bs_path_exists(md_src_fn)) {
        fprintf(stderr, "Error: Store per-block metadata file [%s] does not exist!\n", md_src_fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    ssize_t md_fn_size = bs_file_size(md_src_fn);
    FILE *is = NULL;
    is = fopen(md_src_fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to metadata string file!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN, SEEK_SET);
    char md_length[MD_OFFSET_MAX_LEN] = {0};
    if (fread(md_length, sizeof(*md_length), MD_OFFSET_MAX_LEN, is) != MD_OFFSET_MAX_LEN) {
        fprintf(stderr, "Error: Could not read metadata string length from tail of file! [%s]\n", md_length);
        exit(EXIT_FAILURE);
    }
    uint32_t md_string_length = 0;
    sscanf(md_length, "%u", &md_string_length);
    char *md_string = NULL;
    md_string = malloc(md_string_length);
    if (!md_string) {
        fprintf(stderr, "Error: Could not allocate space for intermediate metadata string!\n");
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN - md_string_length, SEEK_SET);
    if (fread(md_string, sizeof(*md_string), md_string_length, is) != md_string_length) {
        fprintf(stderr, "Error: Could not read metadata string innards from file!\n");
        exit(EXIT_FAILURE);
    }
    fclose(is), is = NULL;
    metadata_t* md = NULL;
    md = bs_parse_metadata_str(md_string);
    if (!md) {
        fprintf(stderr, "Error: Could not extract metadata from archive!\n");
        exit(EXIT_FAILURE);
    }
    
    /* allocate byte buffer -- we read in one row of bytes at a time */
    byte_t* byte_buf = NULL;
    size_t n_byte_buf = l->nelems;
    byte_buf = malloc(n_byte_buf);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }

    /* allocate result buffer -- it is up to the caller to release this memory! */
    char* result_buf = NULL;
    size_t n_result_buf = OUTPUT_LINE_MAX_LEN;
    size_t l_result_buf = 0;
    result_buf = malloc(n_result_buf);
    if (!result_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr result buffer!\n");
        exit(EXIT_FAILURE);
    }
    
    /* iterate through separate rows, calculating associated block */
    int32_t block_row_size = (int32_t) md->block_row_size;
    int32_t current_block_idx = -1;
    int32_t new_block_idx = -1;
    size_t row_idx = 0;
    size_t col_idx = 0;
    for (size_t r_idx = 0; r_idx < rn; r_idx++) {
        size_t first = rs[r_idx];
        size_t last = re[r_idx];
        for (size_t query_row = first; query_row <= last; ++query_row) {

            new_block_idx = (int32_t) (query_row / block_row_size); /* explicit cast */
            
            /* test if we are in a different block */
            if (new_block_idx != current_block_idx) {
                /* close current block file, if one is already open, and then open a new block */
                if (is) {
                    fclose(is), is = NULL;
                }
                is = fopen(bs_init_sqr_split_store_fn_str(block_src_dir, new_block_idx), "rb");
                if (ferror(is)) {
                    fprintf(stderr, "Error: Could not open handle to input store!\n");
                    bs_print_usage(stderr);
                    exit(EXIT_FAILURE);
                }
                /* adjust row_idx so that byte offset calculation is relative to current block */
                row_idx = block_row_size * new_block_idx;
            }
            /* offset some number of bytes from current position of is, if necessary */
            /* note that we subtract a row unit, if we have already read through the input stream by one row */
            int64_t row_diff = query_row - row_idx - ((current_block_idx != new_block_idx) ? 0 : 1);
            int64_t bytes_to_go = row_diff * l->nelems;
            if (bytes_to_go > 0) {
                fseek(is, bytes_to_go, SEEK_CUR);
            }
            /* read a row from current block and print its signal to the output stream os */
            row_idx = query_row;
            col_idx = 0;
            size_t items_read = fread(byte_buf, sizeof(*byte_buf), l->nelems, is);
            if (items_read != l->nelems) {
                fprintf(stderr, "Error: Could not read correct number of items from store! ([%zu] read, [%u] required)\n", items_read, l->nelems);
                exit(EXIT_FAILURE);
            }
            do {
                if (row_idx != col_idx) {
                    score_t d = (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score(byte_buf[col_idx]) :
                        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_score_mqz(byte_buf[col_idx]) :
                        bs_decode_byte_to_score_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
                    if ( ((fo == kScoreFilterGtEq) && (d >= fc)) ||
                         ((fo == kScoreFilterGt) && (d > fc)) ||
                         ((fo == kScoreFilterEq) && (fabs(d - fc) < kEpsilon)) ||
                         ((fo == kScoreFilterLtEq) && (d <= fc)) ||
                         ((fo == kScoreFilterLt) && (d < fc)) ||
                         ((fo == kScoreFilterRangedWithinExclusive) && (d > flb) && (d < fub)) ||
                         ((fo == kScoreFilterRangedWithinInclusive) && (d >= flb) && (d <= fub)) ||
                         ((fo == kScoreFilterRangedOutsideExclusive) && ((d < flb) || (d > fub))) ||
                         ((fo == kScoreFilterRangedOutsideInclusive) && ((d <= flb) || (d >= fub))) ) {
                        bs_print_pair_to_buffer(result_buf + l_result_buf,
                                                &l_result_buf,
                                                l->elems[row_idx]->chr,
                                                l->elems[row_idx]->start,
                                                l->elems[row_idx]->stop,
                                                l->elems[col_idx]->chr,
                                                l->elems[col_idx]->start,
                                                l->elems[col_idx]->stop,
                                                d);
                        /* resize the result buffer, if necessary */
                        if (l_result_buf + OUTPUT_LINE_MAX_LEN >= n_result_buf) {
                            char* temp_result_buf = NULL;
                            temp_result_buf = realloc(result_buf, l_result_buf + OUTPUT_LINE_MAX_LEN);
                            if (!temp_result_buf) {
                                fprintf(stderr, "Error: Could not allocate memory to sqr result reallocation buffer!\n");
                                exit(EXIT_FAILURE);
                            }
                            n_result_buf = l_result_buf + OUTPUT_LINE_MAX_LEN;
                            result_buf = temp_result_buf;
                        }
                    }
                }
                col_idx++;
            } while (col_idx < l->nelems);
            
            /* set current block index */
            current_block_idx = new_block_idx;
        }
    }

    /* set result buffer pointer */
    *b = result_buf;
    
    /* clean up */
    fclose(is), is = NULL;
    free(block_src_dir), block_src_dir = NULL;
    free(md_src_fn), md_src_fn = NULL;
    free(md_string), md_string = NULL;
    free(md), md = NULL;
    free(byte_buf), byte_buf = NULL;    
}

/**
 * @brief      bs_print_sqr_bzip2_store_to_bed7(l, s, os)
 *
 * @details    Queries bzip2-compressed square matrix store for 
 *             provided index range globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output stream. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             os     (FILE*) pointer to output stream
 */

void
bs_print_sqr_bzip2_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os)
{
    if (!bs_path_exists(s->attr->fn)) {
        fprintf(stderr, "Error: Store file [%s] does not exist!\n", s->attr->fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    ssize_t fn_size = bs_file_size(s->attr->fn);

    FILE* is = NULL;
    is = fopen(s->attr->fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to input store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* metadata string */
    fseek(is, fn_size - MD_OFFSET_MAX_LEN, SEEK_SET);
    char md_length[MD_OFFSET_MAX_LEN] = {0};
    if (fread(md_length, sizeof(*md_length), MD_OFFSET_MAX_LEN, is) != MD_OFFSET_MAX_LEN) {
        fprintf(stderr, "Error: Could not read metadata string length from tail of file!\n");
        exit(EXIT_FAILURE);
    }
    uint32_t md_string_length = 0;
    sscanf(md_length, "%u", &md_string_length);
    char *md_string = NULL;
    md_string = malloc(md_string_length);
    if (!md_string) {
        fprintf(stderr, "Error: Could not allocate space for intermediate metadata string!\n");
        exit(EXIT_FAILURE);
    }
    fseek(is, fn_size - MD_OFFSET_MAX_LEN - md_string_length, SEEK_SET);
    if (fread(md_string, sizeof(*md_string), md_string_length, is) != md_string_length) {
        fprintf(stderr, "Error: Could not read metadata string innards from file!\n");
        exit(EXIT_FAILURE);
    }

    /* metadata struct */
    metadata_t* md = NULL;
    md = bs_parse_metadata_str(md_string);
    if (!md) {
        fprintf(stderr, "Error: Could not extract metadata from archive!\n");
        exit(EXIT_FAILURE);
    }

    int32_t query_start = (int32_t) bs_globals.store_query_idx_start;
    int32_t query_end = (int32_t) bs_globals.store_query_idx_end;

    uint32_t query_start_block = 0;
    while (query_start >= (int32_t) md->block_row_size) {
        query_start_block++;
        query_start -= md->block_row_size;
    } 
    uint32_t query_end_block = 0;
    while (query_end >= (int32_t) md->block_row_size) {
        query_end_block++;
        query_end -= md->block_row_size;
    }

    /* byte buffer */
    byte_t* byte_buf = NULL;
    ssize_t n_byte_buf = l->nelems;
    byte_buf = malloc(n_byte_buf);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }
    
    /* bzip2 machinery */
    BZFILE* bzf = NULL;
    int bzf_error = BZ_OK;

    for (uint32_t block_idx = query_start_block; block_idx <= query_end_block; block_idx++) {
        ssize_t end_row_idx = ((block_idx + 1) * md->block_row_size - 1 < bs_globals.store_query_idx_end) ? (block_idx + 1) * md->block_row_size - 1 : bs_globals.store_query_idx_end;
        off_t start_offset = md->offsets[block_idx];
        fseek(is, start_offset, SEEK_SET);
        bzf = BZ2_bzReadOpen(&bzf_error, is, kCompressionBzip2Verbosity, kCompressionBzip2SmallPolicy, NULL, 0);
        switch (bzf_error) {
        case BZ_OK:
            break;
        case BZ_CONFIG_ERROR:
        case BZ_PARAM_ERROR:
        case BZ_IO_ERROR:
        case BZ_MEM_ERROR:
            fprintf(stderr, "Error: Could not open bzip2 block stream!\n");
            exit(EXIT_FAILURE);
        }

        uint32_t row_idx = block_idx * md->block_row_size;
        uint32_t col_idx = 0;

        do {
        /* read one row -- can require streaming through entire block to get elements at the end of a block! */
        BZ2_bzRead(&bzf_error, bzf, byte_buf, n_byte_buf);
        switch (bzf_error) {
        case BZ_OK:
        case BZ_STREAM_END:
        break;
        case BZ_PARAM_ERROR:
        case BZ_SEQUENCE_ERROR:
        case BZ_IO_ERROR:
        case BZ_UNEXPECTED_EOF:
        case BZ_DATA_ERROR:
        case BZ_DATA_ERROR_MAGIC:
        case BZ_MEM_ERROR:
        fprintf(stderr, "Error: Could not read from bzip2 block stream!\n");
        exit(EXIT_FAILURE);                
        }
        
            do {
                if ((row_idx != col_idx) && (row_idx >= bs_globals.store_query_idx_start) && (row_idx <= bs_globals.store_query_idx_end)) {
                    bs_print_pair(os, 
                                  l->elems[row_idx]->chr,
                                  l->elems[row_idx]->start,
                                  l->elems[row_idx]->stop,
                                  l->elems[col_idx]->chr,
                                  l->elems[col_idx]->start,
                                  l->elems[col_idx]->stop,
                                  (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score(byte_buf[col_idx]) :
                                  (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_score_mqz(byte_buf[col_idx]) :
                                  bs_decode_byte_to_score_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max)
                                  );
                }
                col_idx++;
            } while (col_idx < l->nelems);
            col_idx = 0;
            row_idx++;
        
        } while (row_idx <= end_row_idx);
        
        BZ2_bzReadClose(&bzf_error, bzf);
        switch (bzf_error) {
        case BZ_OK:
            break;
        case BZ_SEQUENCE_ERROR:
            fprintf(stderr, "Error: Could not close bzip2 block stream!\n");
            exit(EXIT_FAILURE);
        }
    }
    
    /* cleanup */
    bs_delete_metadata(&md);
    free(md_string);
    free(byte_buf);
    fclose(is);
}

/**
 * @brief      bs_print_sqr_filtered_bzip2_store_to_bed7(l, s, os, fc, flb, fub, fo)
 *
 * @details    Queries bzip2-compressed square matrix store for 
 *             provided index range globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output stream. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             os     (FILE*) pointer to output stream
 *             fc     (score_t) score filter cutoff
 *             flb    (score_t) score filter cutoff lower bound for ranged thresholds
 *             fub    (score_t) score filter cutoff upper bound for ranged thresholds
 *             fo     (score_filter_t) score filter operation
 */

void
bs_print_sqr_filtered_bzip2_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os, score_t fc, score_t flb, score_t fub, score_filter_t fo)
{
    if (!bs_path_exists(s->attr->fn)) {
        fprintf(stderr, "Error: Store file [%s] does not exist!\n", s->attr->fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    ssize_t fn_size = bs_file_size(s->attr->fn);

    FILE* is = NULL;
    is = fopen(s->attr->fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to input store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* metadata string */
    fseek(is, fn_size - MD_OFFSET_MAX_LEN, SEEK_SET);
    char md_length[MD_OFFSET_MAX_LEN] = {0};
    if (fread(md_length, sizeof(*md_length), MD_OFFSET_MAX_LEN, is) != MD_OFFSET_MAX_LEN) {
        fprintf(stderr, "Error: Could not read metadata string length from tail of file!\n");
        exit(EXIT_FAILURE);
    }
    uint32_t md_string_length = 0;
    sscanf(md_length, "%u", &md_string_length);
    char *md_string = NULL;
    md_string = malloc(md_string_length);
    if (!md_string) {
        fprintf(stderr, "Error: Could not allocate space for intermediate metadata string!\n");
        exit(EXIT_FAILURE);
    }
    fseek(is, fn_size - MD_OFFSET_MAX_LEN - md_string_length, SEEK_SET);
    if (fread(md_string, sizeof(*md_string), md_string_length, is) != md_string_length) {
        fprintf(stderr, "Error: Could not read metadata string innards from file!\n");
        exit(EXIT_FAILURE);
    }

    /* metadata struct */
    metadata_t* md = NULL;
    md = bs_parse_metadata_str(md_string);
    if (!md) {
        fprintf(stderr, "Error: Could not extract metadata from archive!\n");
        exit(EXIT_FAILURE);
    }

    int32_t query_start = (int32_t) bs_globals.store_query_idx_start;
    int32_t query_end = (int32_t) bs_globals.store_query_idx_end;

    uint32_t query_start_block = 0;
    while (query_start >= (int32_t) md->block_row_size) {
        query_start_block++;
        query_start -= md->block_row_size;
    } 
    uint32_t query_end_block = 0;
    while (query_end >= (int32_t) md->block_row_size) {
        query_end_block++;
        query_end -= md->block_row_size;
    }

    /* byte buffer */
    byte_t* byte_buf = NULL;
    ssize_t n_byte_buf = l->nelems;
    byte_buf = malloc(n_byte_buf);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }
    
    /* bzip2 machinery */
    BZFILE* bzf = NULL;
    int bzf_error = BZ_OK;

    for (uint32_t block_idx = query_start_block; block_idx <= query_end_block; block_idx++) {
        ssize_t end_row_idx = ((block_idx + 1) * md->block_row_size - 1 < bs_globals.store_query_idx_end) ? (block_idx + 1) * md->block_row_size - 1 : bs_globals.store_query_idx_end;
        off_t start_offset = md->offsets[block_idx];
        fseek(is, start_offset, SEEK_SET);
        bzf = BZ2_bzReadOpen(&bzf_error, is, kCompressionBzip2Verbosity, kCompressionBzip2SmallPolicy, NULL, 0);
        switch (bzf_error) {
        case BZ_OK:
            break;
        case BZ_CONFIG_ERROR:
        case BZ_PARAM_ERROR:
        case BZ_IO_ERROR:
        case BZ_MEM_ERROR:
            fprintf(stderr, "Error: Could not open bzip2 block stream!\n");
            exit(EXIT_FAILURE);
        }

        uint32_t row_idx = block_idx * md->block_row_size;
        uint32_t col_idx = 0;

        do {
        /* read one row -- can require streaming through entire block to get elements at the end of a block! */
        BZ2_bzRead(&bzf_error, bzf, byte_buf, n_byte_buf);
        switch (bzf_error) {
        case BZ_OK:
        case BZ_STREAM_END:
        break;
        case BZ_PARAM_ERROR:
        case BZ_SEQUENCE_ERROR:
        case BZ_IO_ERROR:
        case BZ_UNEXPECTED_EOF:
        case BZ_DATA_ERROR:
        case BZ_DATA_ERROR_MAGIC:
        case BZ_MEM_ERROR:
        fprintf(stderr, "Error: Could not read from bzip2 block stream!\n");
        exit(EXIT_FAILURE);                
        }
        
            do {
                if ((row_idx != col_idx) && (row_idx >= bs_globals.store_query_idx_start) && (row_idx <= bs_globals.store_query_idx_end)) {
                    score_t d = (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score(byte_buf[col_idx]) :
                        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_score_mqz(byte_buf[col_idx]) :
                        bs_decode_byte_to_score_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
                    if ( ((fo == kScoreFilterGtEq) && (d >= fc)) ||
                         ((fo == kScoreFilterGt) && (d > fc)) ||
                         ((fo == kScoreFilterEq) && (fabs(d - fc) < kEpsilon)) ||
                         ((fo == kScoreFilterLtEq) && (d <= fc)) ||
                         ((fo == kScoreFilterLt) && (d < fc)) ||
                         ((fo == kScoreFilterRangedWithinExclusive) && (d > flb) && (d < fub)) ||
                         ((fo == kScoreFilterRangedWithinInclusive) && (d >= flb) && (d <= fub)) ||
                         ((fo == kScoreFilterRangedOutsideExclusive) && ((d < flb) || (d > fub))) ||
                         ((fo == kScoreFilterRangedOutsideInclusive) && ((d <= flb) || (d >= fub))) ) {
                        bs_print_pair(os, 
                                      l->elems[row_idx]->chr,
                                      l->elems[row_idx]->start,
                                      l->elems[row_idx]->stop,
                                      l->elems[col_idx]->chr,
                                      l->elems[col_idx]->start,
                                      l->elems[col_idx]->stop,
                                      d);
                    }
                }
                col_idx++;
            } while (col_idx < l->nelems);
            col_idx = 0;
            row_idx++;
        
        } while (row_idx <= end_row_idx);
        
        BZ2_bzReadClose(&bzf_error, bzf);
        switch (bzf_error) {
        case BZ_OK:
            break;
        case BZ_SEQUENCE_ERROR:
            fprintf(stderr, "Error: Could not close bzip2 block stream!\n");
            exit(EXIT_FAILURE);
        }
    }
    
    /* cleanup */
    bs_delete_metadata(&md);
    free(md_string);
    free(byte_buf);
    fclose(is);
}

/**
 * @brief      bs_print_sqr_bzip2_split_store_to_bed7(l, s, os)
 *
 * @details    Queries bzip2-compressed square matrix store folder 
 *             for provided index range globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output stream. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             os     (FILE*) pointer to output stream
 */

void
bs_print_sqr_bzip2_split_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os)
{
    /* init parent folder name for split blocks */
    char* block_src_dir = NULL;
    block_src_dir = bs_init_sqr_bzip2_split_store_dir_str(s->attr->fn);
    if (!bs_path_exists(block_src_dir)) {
        fprintf(stderr, "Error: Store per-block destination [%s] does not exist!\n", block_src_dir);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    /* get metadata string and attributes */
    char* md_src_fn = bs_init_sqr_bzip2_split_store_metadata_fn_str(block_src_dir);
    if (!bs_path_exists(md_src_fn)) {
        fprintf(stderr, "Error: Store per-block metadata file [%s] does not exist!\n", md_src_fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    ssize_t md_fn_size = bs_file_size(md_src_fn);
    FILE *is = NULL;
    is = fopen(md_src_fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to metadata string file!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN, SEEK_SET);
    char md_length[MD_OFFSET_MAX_LEN] = {0};
    if (fread(md_length, sizeof(*md_length), MD_OFFSET_MAX_LEN, is) != MD_OFFSET_MAX_LEN) {
        fprintf(stderr, "Error: Could not read metadata string length from tail of file! [%s]\n", md_length);
        exit(EXIT_FAILURE);
    }
    uint32_t md_string_length = 0;
    sscanf(md_length, "%u", &md_string_length);
    char *md_string = NULL;
    md_string = malloc(md_string_length);
    if (!md_string) {
        fprintf(stderr, "Error: Could not allocate space for intermediate metadata string!\n");
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN - md_string_length, SEEK_SET);
    if (fread(md_string, sizeof(*md_string), md_string_length, is) != md_string_length) {
        fprintf(stderr, "Error: Could not read metadata string innards from file!\n");
        exit(EXIT_FAILURE);
    }
    fclose(is);
    metadata_t* md = NULL;
    md = bs_parse_metadata_str(md_string);
    if (!md) {
        fprintf(stderr, "Error: Could not extract metadata from archive!\n");
        exit(EXIT_FAILURE);
    }    
    
    /* build query_start and query_end parameters */
    int32_t query_start = (int32_t) bs_globals.store_query_idx_start;
    int32_t query_end = (int32_t) bs_globals.store_query_idx_end;

    uint32_t query_start_block = 0;
    while (query_start >= (int32_t) md->block_row_size) {
        query_start_block++;
        query_start -= md->block_row_size;
    } 
    uint32_t query_end_block = 0;
    while (query_end >= (int32_t) md->block_row_size) {
        query_end_block++;
        query_end -= md->block_row_size;
    }
    
    /* allocate byte buffer */
    byte_t* byte_buf = NULL;
    ssize_t n_byte_buf = l->nelems;
    byte_buf = malloc(n_byte_buf);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }
    
    /* set up bzip2 machinery */
    BZFILE* bzf = NULL;
    int bzf_error = BZ_OK;
    
    /* iterate through query_start -> query_end blocks */
    for (uint32_t block_idx = query_start_block; block_idx <= query_end_block; block_idx++) {
        /* at start of block, open new bzf ptr */
        is = fopen(bs_init_sqr_bzip2_split_store_fn_str(block_src_dir, block_idx), "rb");
        if (ferror(is)) {
            fprintf(stderr, "Error: Could not open handle to input store!\n");
            bs_print_usage(stderr);
            exit(EXIT_FAILURE);
        }
        bzf = BZ2_bzReadOpen(&bzf_error, is, kCompressionBzip2Verbosity, kCompressionBzip2SmallPolicy, NULL, 0);
        switch (bzf_error) {
        case BZ_OK:
            break;
        case BZ_CONFIG_ERROR:
        case BZ_PARAM_ERROR:
        case BZ_IO_ERROR:
        case BZ_MEM_ERROR:
            fprintf(stderr, "Error: Could not open bzip2 block stream!\n");
            exit(EXIT_FAILURE);
        }

        uint32_t row_idx = block_idx * md->block_row_size;
        uint32_t col_idx = 0;
        ssize_t end_row_idx = ((block_idx + 1) * md->block_row_size - 1 < bs_globals.store_query_idx_end) ? (block_idx + 1) * md->block_row_size - 1 : bs_globals.store_query_idx_end;

        do {
        /* read in row */
        BZ2_bzRead(&bzf_error, bzf, byte_buf, n_byte_buf);
        switch (bzf_error) {
        case BZ_OK:
        case BZ_STREAM_END:
        break;
        case BZ_PARAM_ERROR:
        case BZ_SEQUENCE_ERROR:
        case BZ_IO_ERROR:
        case BZ_UNEXPECTED_EOF:
        case BZ_DATA_ERROR:
        case BZ_DATA_ERROR_MAGIC:
        case BZ_MEM_ERROR:
        fprintf(stderr, "Error: Could not read from bzip2 block stream!\n");
        exit(EXIT_FAILURE);                
        }
            do {
                if ((row_idx != col_idx) && (row_idx >= bs_globals.store_query_idx_start) && (row_idx <= bs_globals.store_query_idx_end)) {
                    bs_print_pair(os, 
                                  l->elems[row_idx]->chr,
                                  l->elems[row_idx]->start,
                                  l->elems[row_idx]->stop,
                                  l->elems[col_idx]->chr,
                                  l->elems[col_idx]->start,
                                  l->elems[col_idx]->stop,
                                  (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score(byte_buf[col_idx]) :
                                  (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_score_mqz(byte_buf[col_idx]) :
                                  bs_decode_byte_to_score_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max)
                                  );
                }
                col_idx++;
            } while (col_idx < l->nelems);
            col_idx = 0;
            row_idx++;
        } while (row_idx <= end_row_idx);
        
        /* at end of block, close bzf ptr */
        BZ2_bzReadClose(&bzf_error, bzf);
        switch (bzf_error) {
        case BZ_OK:
            fclose(is);
            break;
        case BZ_SEQUENCE_ERROR:
            fprintf(stderr, "Error: Could not close bzip2 block stream!\n");
            exit(EXIT_FAILURE);
        }        
    }
    
    
    /* clean up */
    free(block_src_dir), block_src_dir = NULL;
    free(md_src_fn), md_src_fn = NULL;
    free(md_string), md_string = NULL;
    free(md), md = NULL;
    free(byte_buf), byte_buf = NULL;
}

/**
 * @brief      bs_print_sqr_filtered_bzip2_split_store_to_bed7(l, s, os, fc, flb, fub, fo)
 *
 * @details    Queries bzip2-compressed square matrix store folder 
 *             for provided index range globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output stream. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             os     (FILE*) pointer to output stream
 *             fc     (score_t) score filter cutoff
 *             flb    (score_t) score filter cutoff lower bound for ranged thresholds
 *             fub    (score_t) score filter cutoff upper bound for ranged thresholds
 *             fo     (score_filter_t) score filter operation
 */

void
bs_print_sqr_filtered_bzip2_split_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os, score_t fc, score_t flb, score_t fub, score_filter_t fo)
{
    /* init parent folder name for split blocks */
    char* block_src_dir = NULL;
    block_src_dir = bs_init_sqr_bzip2_split_store_dir_str(s->attr->fn);
    if (!bs_path_exists(block_src_dir)) {
        fprintf(stderr, "Error: Store per-block destination [%s] does not exist!\n", block_src_dir);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    /* get metadata string and attributes */
    char* md_src_fn = bs_init_sqr_bzip2_split_store_metadata_fn_str(block_src_dir);
    if (!bs_path_exists(md_src_fn)) {
        fprintf(stderr, "Error: Store per-block metadata file [%s] does not exist!\n", md_src_fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    ssize_t md_fn_size = bs_file_size(md_src_fn);
    FILE *is = NULL;
    is = fopen(md_src_fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to metadata string file!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN, SEEK_SET);
    char md_length[MD_OFFSET_MAX_LEN] = {0};
    if (fread(md_length, sizeof(*md_length), MD_OFFSET_MAX_LEN, is) != MD_OFFSET_MAX_LEN) {
        fprintf(stderr, "Error: Could not read metadata string length from tail of file! [%s]\n", md_length);
        exit(EXIT_FAILURE);
    }
    uint32_t md_string_length = 0;
    sscanf(md_length, "%u", &md_string_length);
    char *md_string = NULL;
    md_string = malloc(md_string_length);
    if (!md_string) {
        fprintf(stderr, "Error: Could not allocate space for intermediate metadata string!\n");
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN - md_string_length, SEEK_SET);
    if (fread(md_string, sizeof(*md_string), md_string_length, is) != md_string_length) {
        fprintf(stderr, "Error: Could not read metadata string innards from file!\n");
        exit(EXIT_FAILURE);
    }
    fclose(is);
    metadata_t* md = NULL;
    md = bs_parse_metadata_str(md_string);
    if (!md) {
        fprintf(stderr, "Error: Could not extract metadata from archive!\n");
        exit(EXIT_FAILURE);
    }    
    
    /* build query_start and query_end parameters */
    int32_t query_start = (int32_t) bs_globals.store_query_idx_start;
    int32_t query_end = (int32_t) bs_globals.store_query_idx_end;

    uint32_t query_start_block = 0;
    while (query_start >= (int32_t) md->block_row_size) {
        query_start_block++;
        query_start -= md->block_row_size;
    } 
    uint32_t query_end_block = 0;
    while (query_end >= (int32_t) md->block_row_size) {
        query_end_block++;
        query_end -= md->block_row_size;
    }
    
    /* allocate byte buffer */
    byte_t* byte_buf = NULL;
    ssize_t n_byte_buf = l->nelems;
    byte_buf = malloc(n_byte_buf);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }
    
    /* set up bzip2 machinery */
    BZFILE* bzf = NULL;
    int bzf_error = BZ_OK;
    
    /* iterate through query_start -> query_end blocks */
    for (uint32_t block_idx = query_start_block; block_idx <= query_end_block; block_idx++) {
        /* at start of block, open new bzf ptr */
        is = fopen(bs_init_sqr_bzip2_split_store_fn_str(block_src_dir, block_idx), "rb");
        if (ferror(is)) {
            fprintf(stderr, "Error: Could not open handle to input store!\n");
            bs_print_usage(stderr);
            exit(EXIT_FAILURE);
        }
        bzf = BZ2_bzReadOpen(&bzf_error, is, kCompressionBzip2Verbosity, kCompressionBzip2SmallPolicy, NULL, 0);
        switch (bzf_error) {
        case BZ_OK:
            break;
        case BZ_CONFIG_ERROR:
        case BZ_PARAM_ERROR:
        case BZ_IO_ERROR:
        case BZ_MEM_ERROR:
            fprintf(stderr, "Error: Could not open bzip2 block stream!\n");
            exit(EXIT_FAILURE);
        }

        uint32_t row_idx = block_idx * md->block_row_size;
        uint32_t col_idx = 0;
        ssize_t end_row_idx = ((block_idx + 1) * md->block_row_size - 1 < bs_globals.store_query_idx_end) ? (block_idx + 1) * md->block_row_size - 1 : bs_globals.store_query_idx_end;

        do {
        /* read in row */
        BZ2_bzRead(&bzf_error, bzf, byte_buf, n_byte_buf);
        switch (bzf_error) {
        case BZ_OK:
        case BZ_STREAM_END:
        break;
        case BZ_PARAM_ERROR:
        case BZ_SEQUENCE_ERROR:
        case BZ_IO_ERROR:
        case BZ_UNEXPECTED_EOF:
        case BZ_DATA_ERROR:
        case BZ_DATA_ERROR_MAGIC:
        case BZ_MEM_ERROR:
        fprintf(stderr, "Error: Could not read from bzip2 block stream!\n");
        exit(EXIT_FAILURE);                
        }
            do {
                if ((row_idx != col_idx) && (row_idx >= bs_globals.store_query_idx_start) && (row_idx <= bs_globals.store_query_idx_end)) {
                    score_t d = (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_score(byte_buf[col_idx]) :
                        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_score_mqz(byte_buf[col_idx]) :
                        bs_decode_byte_to_score_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
                    if ( ((fo == kScoreFilterGtEq) && (d >= fc)) ||
                         ((fo == kScoreFilterGt) && (d > fc)) ||
                         ((fo == kScoreFilterEq) && (fabs(d - fc) < kEpsilon)) ||
                         ((fo == kScoreFilterLtEq) && (d <= fc)) ||
                         ((fo == kScoreFilterLt) && (d < fc)) ||
                         ((fo == kScoreFilterRangedWithinExclusive) && (d > flb) && (d < fub)) ||
                         ((fo == kScoreFilterRangedWithinInclusive) && (d >= flb) && (d <= fub)) ||
                         ((fo == kScoreFilterRangedOutsideExclusive) && ((d < flb) || (d > fub))) ||
                         ((fo == kScoreFilterRangedOutsideInclusive) && ((d <= flb) || (d >= fub))) ) {
                        bs_print_pair(os, 
                                      l->elems[row_idx]->chr,
                                      l->elems[row_idx]->start,
                                      l->elems[row_idx]->stop,
                                      l->elems[col_idx]->chr,
                                      l->elems[col_idx]->start,
                                      l->elems[col_idx]->stop,
                                      d);
                    }
                }
                col_idx++;
            } while (col_idx < l->nelems);
            col_idx = 0;
            row_idx++;
        } while (row_idx <= end_row_idx);
        
        /* at end of block, close bzf ptr */
        BZ2_bzReadClose(&bzf_error, bzf);
        switch (bzf_error) {
        case BZ_OK:
            fclose(is);
            break;
        case BZ_SEQUENCE_ERROR:
            fprintf(stderr, "Error: Could not close bzip2 block stream!\n");
            exit(EXIT_FAILURE);
        }        
    }
    
    
    /* clean up */
    free(block_src_dir), block_src_dir = NULL;
    free(md_src_fn), md_src_fn = NULL;
    free(md_string), md_string = NULL;
    free(md), md = NULL;
    free(byte_buf), byte_buf = NULL;
}

/**
 * @brief      bs_init_metadata_str(o, n, s, v)
 *
 * @details    Prints formatted metadata string from block
 *             offset array and array length.
 *
 * @param      o      (off_t*) array of block offsets
 *             n      (uint32_t) number of offsets in array
 *             s      (uint32_t) size of a row block
 *             v      (score_variety_t) score variety (Pearson r, Spearman rho, etc.)
 *
 * @return     (char*) formatted metadata string 
 */

char*
bs_init_metadata_str(off_t* o, uint32_t n, uint32_t s, score_variety_t v)
{
    char* m_str = NULL;

    if (kCompressionMetadataVersion == kCompressionMetadataVersion1p0) {
        /* 
           byte store metadata format (v1.0)
           ---------------------------------
           "version|row_block_size|number_of_offsets|offset_1|offset_2|offset_3|...|offset_n|metadata_length\0"
        */
        size_t m_size = 0;
        size_t m_len = (OFFSET_MAX_LEN + 1) * n + 1; 

        m_str = calloc(m_len, sizeof(*m_str));
        if (!m_str) {
            fprintf(stderr, "Error: Could not allocate space for compression offset metadata string!\n");
            exit(EXIT_FAILURE);
        }

        m_size += sprintf(m_str + m_size, "%3.1f%c", kCompressionMetadataVersion1p0, kCompressionMetadataDelimiter);
        m_size += sprintf(m_str + m_size, "%d%c", s, kCompressionMetadataDelimiter);
        m_size += sprintf(m_str + m_size, "%d%c", n, kCompressionMetadataDelimiter);

        for (uint32_t o_idx = 0; o_idx < n; o_idx++) {
            m_size += sprintf(m_str + m_size, "%" PRIu64 "%c", o[o_idx], kCompressionMetadataDelimiter);
        }
        m_str[m_size] = '\0';
        m_size += sprintf(m_str + m_size, "%0*zu", (int) MD_OFFSET_MAX_LEN, strlen(m_str));
    }
    else if (kCompressionMetadataVersion == kCompressionMetadataVersion1p1) {
        /* 
           byte store metadata format (v1.1)
           ---------------------------------
           "version|score_variety|row_block_size|number_of_offsets|offset_1|offset_2|offset_3|...|offset_n|metadata_length\0"
        */
        const size_t m_version_length = 6;
        const size_t m_score_variety_length = 2;
        const size_t m_row_block_size_length = BLOCK_STR_MAX_LEN + 1;
        const size_t m_offsets_length = (OFFSET_MAX_LEN + 1) * (n + 1);
        const size_t m_metadata_offset_length = MD_OFFSET_MAX_LEN;
        size_t m_size = 0;
        size_t m_len = m_version_length + m_score_variety_length + m_row_block_size_length + m_offsets_length + m_metadata_offset_length + 1; 

        m_str = calloc(m_len, sizeof(*m_str));
        if (!m_str) {
            fprintf(stderr, "Error: Could not allocate space for compression offset metadata string!\n");
            exit(EXIT_FAILURE);
        }

        m_size += sprintf(m_str + m_size, "%3.1f%c", kCompressionMetadataVersion1p1, kCompressionMetadataDelimiter);
        m_size += sprintf(m_str + m_size, "%d%c", v, kCompressionMetadataDelimiter);
        m_size += sprintf(m_str + m_size, "%d%c", s, kCompressionMetadataDelimiter);
        m_size += sprintf(m_str + m_size, "%d%c", n, kCompressionMetadataDelimiter);

        for (uint32_t o_idx = 0; o_idx < n; o_idx++) {
            m_size += sprintf(m_str + m_size, "%" PRIu64 "%c", o[o_idx], kCompressionMetadataDelimiter);
        }
        m_str[m_size] = '\0';
        m_size += sprintf(m_str + m_size, "%0*zu", (int) MD_OFFSET_MAX_LEN, strlen(m_str));
    }

    return m_str;
}

/**
 * @brief      bs_parse_metadata_str(ms)
 *
 * @details    Returns new metadata struct ptr populated
 *             with member values from input string
 *
 * @param      ms     (char*) formatted metadata string
 *
 * @return     (metadata_t*) pointer to metadata struct ptr
 */

metadata_t*
bs_parse_metadata_str(char* ms)
{
    char md_token[OFFSET_MAX_LEN] = {0};
    char* md_delim_pos_ptr = NULL;
    size_t md_delim_length = 0;
    char* md_string_tok_start = NULL;
    metadata_t* metadata = NULL;
    int md_score_variety_int = 0;
    float md_version = 0.0f;
    size_t md_block_row_size = 0;
    size_t md_num_offsets = 0;
    off_t* md_offsets = NULL;
    
    /* metadata version */
    md_delim_pos_ptr = strchr(ms, (int) kCompressionMetadataDelimiter);
    md_delim_length = md_delim_pos_ptr - ms;    
    memcpy(md_token, ms, md_delim_length);
    md_token[md_delim_length] = '\0';
    if (sscanf(md_token, "%f", &md_version) == EOF) {
        fprintf(stderr, "Error: Could not parse metadata version key!\n");
        exit(EXIT_FAILURE);
    }

    if (md_version == kCompressionMetadataVersion1p0) { /* v1.0 */
        /* row block size */
        md_string_tok_start = ms + md_delim_length + 1;
        md_delim_pos_ptr = strchr(md_string_tok_start, (int) kCompressionMetadataDelimiter);
        md_delim_length = md_delim_pos_ptr - md_string_tok_start;    
        memcpy(md_token, md_string_tok_start, md_delim_length);
        md_token[md_delim_length] = '\0';
        sscanf(md_token, "%zu", &md_block_row_size);
        /* number of offsets */
        md_string_tok_start = md_string_tok_start + md_delim_length + 1;
        md_delim_pos_ptr = strchr(md_string_tok_start, (int) kCompressionMetadataDelimiter);
        md_delim_length = md_delim_pos_ptr - md_string_tok_start;
        memcpy(md_token, md_string_tok_start, md_delim_length);
        md_token[md_delim_length] = '\0';
        sscanf(md_token, "%zu", &md_num_offsets);
        /* offsets */
        md_offsets = malloc(md_num_offsets * sizeof(*md_offsets));
        if (!md_offsets) {
            fprintf(stderr, "Error: Could not allocate space for offset data!\n");
            exit(EXIT_FAILURE);
        }
        for (size_t offset_idx = 0; offset_idx < md_num_offsets; offset_idx++) {
            md_string_tok_start = md_string_tok_start + md_delim_length + 1;            
            md_delim_pos_ptr = strchr(md_string_tok_start, (int) kCompressionMetadataDelimiter);
            md_delim_length = md_delim_pos_ptr - md_string_tok_start;
            memcpy(md_token, md_string_tok_start, md_delim_length);
            md_token[md_delim_length] = '\0';
            sscanf(md_token, "%zd", &md_offsets[offset_idx]);
        }
    }
    else if (md_version == kCompressionMetadataVersion1p1) { /* v1.1 */
        /* score variety type */
        md_string_tok_start = ms + md_delim_length + 1;
        md_delim_pos_ptr = strchr(md_string_tok_start, (int) kCompressionMetadataDelimiter);
        md_delim_length = md_delim_pos_ptr - md_string_tok_start;
        memcpy(md_token, md_string_tok_start, md_delim_length);
        md_token[md_delim_length] = '\0';
        sscanf(md_token, "%d", &md_score_variety_int);
        /* row block size */
        md_string_tok_start = md_string_tok_start + md_delim_length + 1;
        md_delim_pos_ptr = strchr(md_string_tok_start, (int) kCompressionMetadataDelimiter);
        md_delim_length = md_delim_pos_ptr - md_string_tok_start;    
        memcpy(md_token, md_string_tok_start, md_delim_length);
        md_token[md_delim_length] = '\0';
        sscanf(md_token, "%zu", &md_block_row_size);
        /* number of offsets */
        md_string_tok_start = md_string_tok_start + md_delim_length + 1;
        md_delim_pos_ptr = strchr(md_string_tok_start, (int) kCompressionMetadataDelimiter);
        md_delim_length = md_delim_pos_ptr - md_string_tok_start;
        memcpy(md_token, md_string_tok_start, md_delim_length);
        md_token[md_delim_length] = '\0';
        sscanf(md_token, "%zu", &md_num_offsets);
        /* offsets */
        md_offsets = malloc(md_num_offsets * sizeof(*md_offsets));
        if (!md_offsets) {
            fprintf(stderr, "Error: Could not allocate space for offset data!\n");
            exit(EXIT_FAILURE);
        }
        for (size_t offset_idx = 0; offset_idx < md_num_offsets; offset_idx++) {
            md_string_tok_start = md_string_tok_start + md_delim_length + 1;            
            md_delim_pos_ptr = strchr(md_string_tok_start, (int) kCompressionMetadataDelimiter);
            md_delim_length = md_delim_pos_ptr - md_string_tok_start;
            memcpy(md_token, md_string_tok_start, md_delim_length);
            md_token[md_delim_length] = '\0';
            sscanf(md_token, "%zd", &md_offsets[offset_idx]);
        }
    }
    else {
        fprintf(stderr, "Error: Could not parse metadata due to unknown version key!\n");
        exit(EXIT_FAILURE);
    }
    metadata = malloc(sizeof(metadata_t));
    if (!metadata) {
        fprintf(stderr, "Error: Could not allocate space for offset struct!\n");
        exit(EXIT_FAILURE);
    }
    metadata->score_variety = (score_variety_t) md_score_variety_int;
    metadata->offsets = md_offsets;
    metadata->count = md_num_offsets;
    metadata->block_row_size = md_block_row_size;
    metadata->version = md_version;

    return metadata;
}

/**
 * @brief      bs_delete_metadata(m)
 *
 * @details    Releases memory for metadata struct
 *
 * @param      m      (metadata_t**) pointer to metadata struct ptr
 */

void
bs_delete_metadata(metadata_t** m)
{
    free((*m)->offsets), (*m)->offsets = NULL;
    free(*m), *m = NULL;
}

/**
 * @brief      bs_print_sqr_store_frequency_to_txt(l, s, os)
 *
 * @details    Prints bin score, count and frequency of 
 *             square matrix store to specified output stream.
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             os     (FILE*) pointer to output stream
 */

void
bs_print_sqr_store_frequency_to_txt(lookup_t* l, sqr_store_t* s, FILE* os)
{
    if (!bs_path_exists(s->attr->fn)) {
        fprintf(stderr, "Error: Store file [%s] does not exist!\n", s->attr->fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    FILE* is = NULL;
    is = fopen(s->attr->fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to input store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    byte_t* byte_buf = NULL;
    byte_buf = malloc(l->nelems);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }

    uint32_t row_idx = 0;
    uint64_t freq_table[256] = {0};
    do {
        uint32_t nbyte = 0;
        if (fread(byte_buf, sizeof(*byte_buf), l->nelems, is) != l->nelems) {
            fprintf(stderr, "Error: Could not read a row of data from sqr input stream!\n");
            exit(EXIT_FAILURE);
        }
        do {
            freq_table[byte_buf[nbyte++]]++;
        } while (nbyte < l->nelems);
    } while (++row_idx < l->nelems);
    bs_print_frequency_buffer(freq_table, s->attr->nbytes, os);
    
    /* clean up */
    free(byte_buf);
    fclose(is);
}

/**
 * @brief      bs_print_sqr_split_store_frequency_to_txt(l, s, os)
 *
 * @details    Prints bin score, count and frequency of split
 *             square matrix store to specified output stream.
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             os     (FILE*) pointer to output stream
 */

void
bs_print_sqr_split_store_frequency_to_txt(lookup_t* l, sqr_store_t* s, FILE* os)
{
    /* init parent folder name for split blocks */
    char* block_src_dir = NULL;
    block_src_dir = bs_init_sqr_split_store_dir_str(s->attr->fn);
    if (!bs_path_exists(block_src_dir)) {
        fprintf(stderr, "Error: Store per-block destination [%s] does not exist!\n", block_src_dir);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    /* get metadata string and attributes */
    char* md_src_fn = bs_init_sqr_split_store_metadata_fn_str(block_src_dir);
    if (!bs_path_exists(md_src_fn)) {
        fprintf(stderr, "Error: Store per-block metadata file [%s] does not exist!\n", md_src_fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    ssize_t md_fn_size = bs_file_size(md_src_fn);
    FILE *is = NULL;
    is = fopen(md_src_fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to metadata string file!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN, SEEK_SET);
    char md_length[MD_OFFSET_MAX_LEN] = {0};
    if (fread(md_length, sizeof(*md_length), MD_OFFSET_MAX_LEN, is) != MD_OFFSET_MAX_LEN) {
        fprintf(stderr, "Error: Could not read metadata string length from tail of file! [%s]\n", md_length);
        exit(EXIT_FAILURE);
    }
    uint32_t md_string_length = 0;
    sscanf(md_length, "%u", &md_string_length);
    char *md_string = NULL;
    md_string = malloc(md_string_length);
    if (!md_string) {
        fprintf(stderr, "Error: Could not allocate space for intermediate metadata string!\n");
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN - md_string_length, SEEK_SET);
    if (fread(md_string, sizeof(*md_string), md_string_length, is) != md_string_length) {
        fprintf(stderr, "Error: Could not read metadata string innards from file!\n");
        exit(EXIT_FAILURE);
    }
    fclose(is);
    metadata_t* md = NULL;
    md = bs_parse_metadata_str(md_string);
    if (!md) {
        fprintf(stderr, "Error: Could not extract metadata from archive!\n");
        exit(EXIT_FAILURE);
    }
    
    /* read a row of data at a time */
    byte_t* byte_buf = NULL;
    size_t n_byte_buf = l->nelems;
    byte_buf = malloc(n_byte_buf * sizeof(*byte_buf));
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }

    uint32_t row_idx = 0;
    uint64_t freq_table[256] = {0};
    do {
        uint32_t nbyte = 0;
        if (fread(byte_buf, sizeof(*byte_buf), n_byte_buf, is) != n_byte_buf) {
            fprintf(stderr, "Error: Could not read a full row of data from sqr input stream!\n");
            exit(EXIT_FAILURE);
        }
        do {
            freq_table[byte_buf[nbyte++]]++;
        } while (nbyte < l->nelems);
    } while (++row_idx < l->nelems);
    bs_print_frequency_buffer(freq_table, s->attr->nbytes, os);
    
    /* clean up */
    free(block_src_dir), block_src_dir = NULL;
    free(md_src_fn), md_src_fn = NULL;
    free(md_string), md_string = NULL;
    free(md), md = NULL;
    free(byte_buf), byte_buf = NULL;
    fclose(is);
}

/**
 * @brief      bs_print_sqr_bzip2_store_frequency_to_txt(l, s, os)
 *
 * @details    Prints bin score, count and frequency of 
 *             square matrix store to specified output stream.
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             os     (FILE*) pointer to output stream
 */

void
bs_print_sqr_bzip2_store_frequency_to_txt(lookup_t* l, sqr_store_t* s, FILE* os)
{
    if (!bs_path_exists(s->attr->fn)) {
        fprintf(stderr, "Error: Store file [%s] does not exist!\n", s->attr->fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    ssize_t fn_size = bs_file_size(s->attr->fn);

    FILE* is = NULL;
    is = fopen(s->attr->fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to input store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    /* metadata string */
    fseek(is, fn_size - MD_OFFSET_MAX_LEN, SEEK_SET);
    char md_length[MD_OFFSET_MAX_LEN] = {0};
    if (fread(md_length, sizeof(*md_length), MD_OFFSET_MAX_LEN, is) != MD_OFFSET_MAX_LEN) {
        fprintf(stderr, "Error: Could not read metadata string length from tail of file!\n");
        exit(EXIT_FAILURE);
    }
    uint32_t md_string_length = 0;
    sscanf(md_length, "%u", &md_string_length);
    char *md_string = NULL;
    md_string = malloc(md_string_length);
    if (!md_string) {
        fprintf(stderr, "Error: Could not allocate space for intermediate metadata string!\n");
        exit(EXIT_FAILURE);
    }
    fseek(is, fn_size - MD_OFFSET_MAX_LEN - md_string_length, SEEK_SET);
    if (fread(md_string, sizeof(*md_string), md_string_length, is) != md_string_length) {
        fprintf(stderr, "Error: Could not read metadata string innards from file!\n");
        exit(EXIT_FAILURE);
    }

    /* metadata struct */
    metadata_t* md = NULL;
    md = bs_parse_metadata_str(md_string);
    if (!md) {
        fprintf(stderr, "Error: Could not extract metadata from archive!\n");
        exit(EXIT_FAILURE);
    }

    /* block buffer */
    byte_t* byte_buf = NULL;
    ssize_t n_byte_buf = md->block_row_size * l->nelems;
    byte_buf = malloc(n_byte_buf);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to byte buffer!\n");
        exit(EXIT_FAILURE);
    }
    
    /* bzip2 machinery */
    BZFILE* bzf = NULL;
    int bzf_error = BZ_OK;

    uint32_t block_idx = 0;
    uint64_t total_bytes = 0;
    uint64_t freq_table[256] = {0};
    do {
        uint32_t buf_byte = 0;
        off_t start_offset = md->offsets[block_idx++];
        fseek(is, start_offset, SEEK_SET);

        /* open bzip2 stream */
        bzf = BZ2_bzReadOpen(&bzf_error, is, kCompressionBzip2Verbosity, kCompressionBzip2SmallPolicy, NULL, 0);
        switch (bzf_error) {
        case BZ_OK:
            break;
        case BZ_CONFIG_ERROR:
        case BZ_PARAM_ERROR:
        case BZ_IO_ERROR:
        case BZ_MEM_ERROR:
            fprintf(stderr, "Error: Could not open bzip2 block stream!\n");
            exit(EXIT_FAILURE);
        }
        
        /* read a block of data */
        uint32_t buf_byte_read = (uint32_t) BZ2_bzRead(&bzf_error, bzf, byte_buf, n_byte_buf);
        switch (bzf_error) {
        case BZ_OK:
        case BZ_STREAM_END:
            break;
        case BZ_PARAM_ERROR:
        case BZ_SEQUENCE_ERROR:
        case BZ_IO_ERROR:
        case BZ_UNEXPECTED_EOF:
        case BZ_DATA_ERROR:
        case BZ_DATA_ERROR_MAGIC:
        case BZ_MEM_ERROR:
            fprintf(stderr, "Error: Could not read from bzip2 block stream!\n");
            exit(EXIT_FAILURE);                
        }
        do {
            freq_table[byte_buf[buf_byte++]]++;
        } while (buf_byte < buf_byte_read);

        /* close bzip2 stream */
        BZ2_bzReadClose(&bzf_error, bzf);
        switch (bzf_error) {
        case BZ_OK:
            break;
        case BZ_SEQUENCE_ERROR:
            fprintf(stderr, "Error: Could not close bzip2 block stream!\n");
            exit(EXIT_FAILURE);
        }
        total_bytes += buf_byte;
    } while (total_bytes < s->attr->nbytes);

    bs_print_frequency_buffer(freq_table, s->attr->nbytes, os);
    
    /* clean up */
    free(byte_buf), byte_buf = NULL;
    fclose(is);
}

/**
 * @brief      bs_print_sqr_bzip2_split_store_frequency_to_txt(l, s, os)
 *
 * @details    Prints bin score, count and frequency of 
 *             square matrix store to specified output stream.
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             os     (FILE*) pointer to output stream
 */

void
bs_print_sqr_bzip2_split_store_frequency_to_txt(lookup_t* l, sqr_store_t* s, FILE* os)
{
    /* init parent folder name for split blocks */
    char* block_src_dir = NULL;
    block_src_dir = bs_init_sqr_bzip2_split_store_dir_str(s->attr->fn);
    if (!bs_path_exists(block_src_dir)) {
        fprintf(stderr, "Error: Store per-block destination [%s] does not exist!\n", block_src_dir);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    
    /* get metadata string and attributes */
    char* md_src_fn = bs_init_sqr_bzip2_split_store_metadata_fn_str(block_src_dir);
    if (!bs_path_exists(md_src_fn)) {
        fprintf(stderr, "Error: Store per-block metadata file [%s] does not exist!\n", md_src_fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    ssize_t md_fn_size = bs_file_size(s->attr->fn);
    FILE *is = NULL;
    is = fopen(md_src_fn, "rb");
    if (ferror(is)) {
        fprintf(stderr, "Error: Could not open handle to metadata string file!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN, SEEK_SET);
    char md_length[MD_OFFSET_MAX_LEN] = {0};
    if (fread(md_length, sizeof(*md_length), MD_OFFSET_MAX_LEN, is) != MD_OFFSET_MAX_LEN) {
        fprintf(stderr, "Error: Could not read metadata string length from tail of file!\n");
        exit(EXIT_FAILURE);
    }
    uint32_t md_string_length = 0;
    sscanf(md_length, "%u", &md_string_length);
    char *md_string = NULL;
    md_string = malloc(md_string_length);
    if (!md_string) {
        fprintf(stderr, "Error: Could not allocate space for intermediate metadata string!\n");
        exit(EXIT_FAILURE);
    }
    fseek(is, md_fn_size - MD_OFFSET_MAX_LEN - md_string_length, SEEK_SET);
    if (fread(md_string, sizeof(*md_string), md_string_length, is) != md_string_length) {
        fprintf(stderr, "Error: Could not read metadata string innards from file!\n");
        exit(EXIT_FAILURE);
    }
    fclose(is);
    metadata_t* md = NULL;
    md = bs_parse_metadata_str(md_string);
    if (!md) {
        fprintf(stderr, "Error: Could not extract metadata from archive!\n");
        exit(EXIT_FAILURE);
    }    
    
    /* allocate block buffer */
    byte_t* byte_buf = NULL;
    ssize_t n_byte_buf = md->block_row_size * l->nelems;
    byte_buf = malloc(n_byte_buf);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }

    /* bzip2 machinery */
    BZFILE* bzf = NULL;
    int bzf_error = BZ_OK;

    uint32_t block_idx = 0;
    uint64_t total_bytes = 0;
    uint64_t freq_table[256] = {0};
    do {
        uint32_t buf_byte = 0;
        is = fopen(bs_init_sqr_bzip2_split_store_fn_str(block_src_dir, block_idx++), "rb");
        if (ferror(is)) {
            fprintf(stderr, "Error: Could not open handle to input store!\n");
            bs_print_usage(stderr);
            exit(EXIT_FAILURE);
        }
        bzf = BZ2_bzReadOpen(&bzf_error, is, kCompressionBzip2Verbosity, kCompressionBzip2SmallPolicy, NULL, 0);
        switch (bzf_error) {
        case BZ_OK:
            break;
        case BZ_CONFIG_ERROR:
        case BZ_PARAM_ERROR:
        case BZ_IO_ERROR:
        case BZ_MEM_ERROR:
            fprintf(stderr, "Error: Could not open bzip2 block stream!\n");
            exit(EXIT_FAILURE);
        }
        uint32_t buf_byte_read = (uint32_t) BZ2_bzRead(&bzf_error, bzf, byte_buf, n_byte_buf);
        switch (bzf_error) {
        case BZ_OK:
        case BZ_STREAM_END:
            break;
        case BZ_PARAM_ERROR:
        case BZ_SEQUENCE_ERROR:
        case BZ_IO_ERROR:
        case BZ_UNEXPECTED_EOF:
        case BZ_DATA_ERROR:
        case BZ_DATA_ERROR_MAGIC:
        case BZ_MEM_ERROR:
            fprintf(stderr, "Error: Could not read from bzip2 block stream!\n");
            exit(EXIT_FAILURE);                
        }
        do {
            freq_table[byte_buf[buf_byte++]]++;
        } while (buf_byte < buf_byte_read);
        BZ2_bzReadClose(&bzf_error, bzf);
        switch (bzf_error) {
        case BZ_OK:
            break;
        case BZ_SEQUENCE_ERROR:
            fprintf(stderr, "Error: Could not close bzip2 block stream!\n");
            exit(EXIT_FAILURE);
        }
        fclose(is);
        total_bytes += buf_byte;
    } while (total_bytes < s->attr->nbytes);

    bs_print_frequency_buffer(freq_table, s->attr->nbytes, os);

    /* clean up */
    free(byte_buf), byte_buf = NULL;
}

/**
 * @brief      bs_print_frequency_buffer(t, n, os)
 *
 * @details    Print formatted frequency table to output stream
 *
 * @param      t      (uint64_t*) pointer to frequency table
 *             n      (uint64_t) total number of elements in store
 *             os     (FILE*) pointer to output stream (stdout, stderr, etc.)
 */

void
bs_print_frequency_buffer(uint64_t* t, uint64_t n, FILE* os)
{
    for (int bin_idx = 0; bin_idx <= 201; bin_idx++) {
        fprintf(os,
                "%3.6f\t%" PRIu64 "\t%3.6f\n",
                bs_decode_byte_to_score((byte_t) bin_idx),
                t[bin_idx],
                (score_t) t[bin_idx] / n);
    }    
}

/**
 * @brief      bs_delete_sqr_store(s)
 *
 * @details    Release memory associated with square matrix store pointer.
 *
 * @param      s      (sqr_store_t**) pointer to square matrix struct pointer
 */

void
bs_delete_sqr_store(sqr_store_t** s)
{
    free((*s)->attr->fn), (*s)->attr->fn = NULL;
    free((*s)->attr), (*s)->attr = NULL;
    free(*s), *s = NULL;
}

/**
 * @brief      bs_init_store_buf_node(uc)
 *
 * @details    Returns new store buffer node pointer.
 *
 * @param      uc     (byte_t) data value
 *
 * @return     (store_buf_node_t*) pointer to buffer node
 */

store_buf_node_t*
bs_init_store_buf_node(byte_t uc)
{
    store_buf_node_t* b = NULL;
    b = malloc(sizeof(store_buf_node_t));
    if (!b) {
        fprintf(stderr, "Error: Could not allocate space for store buf node!\n");
        exit(EXIT_FAILURE);
    }
    b->data = uc;
    b->next = NULL;
    return b;
}

/**
 * @brief      bs_insert_store_buf_node(n,i)
 *
 * @details    If node 'n->next' exists, inserts node 'i' between nodes 
 *             'n' and 'n->next'. Otherwise, 'n->next' is set to 'i'.
 *
 * @param      n      (store_buf_node_t*) point node before inserted node
 *             i      (store_buf_node_t*) node to insert after point node
 */

void
bs_insert_store_buf_node(store_buf_node_t* n, store_buf_node_t* i)
{
    if (!n)
        return;
    if (n->next)
        i->next = n->next;
    n->next = i;
}
