#include "byte-store.h"

int
main(int argc, char** argv) 
{
    bs_init_globals();
    bs_init_command_line_options(argc, argv);

    sut_store_t* sut_store = NULL;
    sqr_store_t* sqr_store = NULL;
    lookup_t* lookup = NULL;

    if (bs_globals.store_type == kStorePearsonRSquareMatrixSplitSingleChunkMetadata)
        lookup = bs_init_lookup(bs_globals.lookup_fn, kFalse);
    else 
        lookup = bs_init_lookup(bs_globals.lookup_fn, !bs_globals.store_query_flag);

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
                bs_populate_sut_store_with_pearsonr_scores(sut_store, lookup);
                break;
            case kStoreRandomBufferedSquareMatrix:
            case kStoreRandomSquareMatrix:
            case kStorePearsonRSquareMatrix:
            case kStorePearsonRSquareMatrixSplit:
            case kStorePearsonRSquareMatrixSplitSingleChunk:
            case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
            case kStorePearsonRSquareMatrixBzip2:
            case kStorePearsonRSquareMatrixBzip2Split:
            case kStoreUndefined:
                fprintf(stderr, "Error: You should never see this error! (A)\n");
                exit(EXIT_FAILURE);
            }
        }
        else if (bs_globals.store_query_flag) {
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
            case kQueryKindUndefined:
                fprintf(stderr, "Error: Query type unsupported!\n");
                exit(EXIT_FAILURE);
            }
            if (rows_found && (bs_globals.store_filter == kScoreFilterNone))
                bs_print_sut_store_to_bed7(lookup, sut_store, stdout);
            else if (rows_found)
                bs_print_sut_filtered_store_to_bed7(lookup, sut_store, stdout, bs_globals.score_filter_cutoff, bs_globals.store_filter);
        }
        else if (bs_globals.store_frequency_flag) {
            bs_print_sut_frequency_to_txt(lookup, sut_store, stdout);
        }
        bs_delete_sut_store(&sut_store);
        break;
    case kStorePearsonRSquareMatrix:
    case kStorePearsonRSquareMatrixSplit:
    case kStorePearsonRSquareMatrixSplitSingleChunk:
    case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
    case kStorePearsonRSquareMatrixBzip2:
    case kStorePearsonRSquareMatrixBzip2Split:
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
                bs_populate_sqr_store_with_pearsonr_scores(sqr_store, lookup);
                break;
            case kStorePearsonRSquareMatrixSplit:
                bs_populate_sqr_split_store_with_pearsonr_scores(sqr_store, lookup, bs_globals.store_row_chunk_size);
                break;
            case kStorePearsonRSquareMatrixSplitSingleChunk:
                bs_populate_sqr_split_store_chunk_with_pearsonr_scores(sqr_store, lookup, bs_globals.store_row_chunk_size, bs_globals.store_row_chunk_offset);
                break;
            case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
                bs_populate_sqr_split_store_chunk_metadata(sqr_store, lookup, bs_globals.store_row_chunk_size);
                break;
            case kStorePearsonRSquareMatrixBzip2:
                bs_populate_sqr_bzip2_store_with_pearsonr_scores(sqr_store, lookup, bs_globals.store_row_chunk_size);
                break;
            case kStorePearsonRSquareMatrixBzip2Split:
                bs_populate_sqr_bzip2_split_store_with_pearsonr_scores(sqr_store, lookup, bs_globals.store_row_chunk_size);
                break;
            case kStorePearsonRSUT:
            case kStoreRandomSUT:
            case kStoreUndefined:
                fprintf(stderr, "Error: You should never see this error! (B)\n");
                exit(EXIT_FAILURE);
            }
        }
        else if (bs_globals.store_query_flag) {
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
                separate_rows_found = bs_parse_query_multiple_index_str(lookup, bs_globals.store_query_str);
                break;
            case kQueryKindUndefined:
                fprintf(stderr, "Error: Query type unsupported!\n");
                exit(EXIT_FAILURE);
            }
            if (contiguous_rows_found) {
                /* extract from raw or uncompressed square matrix */
                switch (bs_globals.store_type) {
                case kStoreRandomBufferedSquareMatrix:
                case kStoreRandomSquareMatrix:
                case kStorePearsonRSquareMatrix:
                    if (bs_globals.store_filter == kScoreFilterNone) 
                        bs_print_sqr_store_to_bed7(lookup, sqr_store, stdout);
                    else 
                        bs_print_sqr_filtered_store_to_bed7(lookup, sqr_store, stdout, bs_globals.score_filter_cutoff, bs_globals.store_filter);
                    break;
                case kStorePearsonRSquareMatrixSplit:
                    if (bs_globals.store_filter == kScoreFilterNone)
                        bs_print_sqr_split_store_to_bed7(lookup, sqr_store, stdout);
                    else
                        bs_print_sqr_filtered_split_store_to_bed7(lookup, sqr_store, stdout, bs_globals.score_filter_cutoff, bs_globals.store_filter);		    
                    break;
                case kStorePearsonRSquareMatrixBzip2:
                    if (bs_globals.store_filter == kScoreFilterNone)
                        bs_print_sqr_bzip2_store_to_bed7(lookup, sqr_store, stdout);
                    else
                        bs_print_sqr_filtered_bzip2_store_to_bed7(lookup, sqr_store, stdout, bs_globals.score_filter_cutoff, bs_globals.store_filter);
                    break;
                case kStorePearsonRSquareMatrixBzip2Split:
                    if (bs_globals.store_filter == kScoreFilterNone)
                        bs_print_sqr_bzip2_split_store_to_bed7(lookup, sqr_store, stdout);
                    else
                        bs_print_sqr_filtered_bzip2_split_store_to_bed7(lookup, sqr_store, stdout, bs_globals.score_filter_cutoff, bs_globals.store_filter);
                    break;
                case kStorePearsonRSUT:
                case kStoreRandomSUT:
                case kStorePearsonRSquareMatrixSplitSingleChunk:
                case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
                case kStoreUndefined:
                    fprintf(stderr, "Error: You should never see this error! (C1)\n");
                    exit(EXIT_FAILURE);
                }
            }
            if (separate_rows_found) {
                switch (bs_globals.store_type) {
                case kStorePearsonRSquareMatrixSplit:
                    if (bs_globals.store_filter == kScoreFilterNone)
                        bs_print_sqr_split_store_separate_rows_to_bed7(lookup, 
                                                                       sqr_store, 
                                                                       stdout, 
                                                                       bs_globals.store_query_indices, 
                                                                       bs_globals.store_query_indices_num);
                    else
                        bs_print_sqr_filtered_split_store_separate_rows_to_bed7(lookup, 
                                                                                sqr_store, 
                                                                                stdout, 
                                                                                bs_globals.store_query_indices, 
                                                                                bs_globals.store_query_indices_num, 
                                                                                bs_globals.score_filter_cutoff, 
                                                                                bs_globals.store_filter);		    
                    break;
                case kStoreRandomBufferedSquareMatrix:
                case kStoreRandomSquareMatrix:
                case kStorePearsonRSquareMatrix:
                case kStorePearsonRSquareMatrixSplitSingleChunk:
                case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
                case kStorePearsonRSquareMatrixBzip2:
                case kStorePearsonRSquareMatrixBzip2Split:
                case kStorePearsonRSUT:
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
                bs_print_sqr_store_frequency_to_txt(lookup, sqr_store, stdout);
                break;
            case kStorePearsonRSquareMatrixSplit:
		bs_print_sqr_split_store_frequency_to_txt(lookup, sqr_store, stdout);
                break;
            case kStorePearsonRSquareMatrixBzip2:
                bs_print_sqr_bzip2_store_frequency_to_txt(lookup, sqr_store, stdout);
                break;
            case kStorePearsonRSquareMatrixBzip2Split:
                bs_print_sqr_bzip2_split_store_frequency_to_txt(lookup, sqr_store, stdout);
                break;
            case kStorePearsonRSUT:
            case kStoreRandomSUT:
            case kStorePearsonRSquareMatrixSplitSingleChunk:
            case kStorePearsonRSquareMatrixSplitSingleChunkMetadata:
            case kStoreUndefined:
                fprintf(stderr, "Error: You should never see this error! (D)\n");
                exit(EXIT_FAILURE);
            }
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

/**
 * @brief      bs_truncate_double_to_precision(d, prec)
 *
 * @details    Truncates double-type value to specified precision.
 *
 * @param      d      (double) value to be truncated
 *             prec   (int) value to determine precision of truncated value
 *
 * @return     (double) truncated value
 */

inline double
bs_truncate_double_to_precision(double d, int prec)
{
    double factor = powf(10, prec);
    return (d < 0) ? ceil(d * factor)/factor : floor((d + kEpsilon) * factor)/factor;
}

/**
 * @brief      bs_encode_double_to_byte(d)
 *
 * @details    Encodes double-type value between -1 and +1 to 
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
 * @param      d      (double) value to be encoded
 *
 * @return     (byte_t) encoded score byte value
 *
 * @example    bs_encode_double_to_byte(-0.010) = 0x63
 *             bs_encode_double_to_byte(-0.009) = 0x64
 *             bs_encode_double_to_byte(-0.000) = 0x64
 *             bs_encode_double_to_byte(+0.000) = 0x65
 *             bs_encode_double_to_byte(+0.139) = 0x72
 *             bs_encode_double_to_byte(+0.140) = 0x73
 *             bs_encode_double_to_byte(+0.142) = 0x73
 */

inline byte_t
bs_encode_double_to_byte(double d) 
{
    d += (d < 0) ? -kEpsilon : kEpsilon; // jitter is used to deal with interval edges
    d = bs_truncate_double_to_precision(d, 2);
    int encode_d = (int) ((d < 0) ? (ceil(d * 1000.0f)/10.0f + 100) : (floor(d * 1000.0f)/10.0f + 100)) + bs_signbit(-d);
    return (byte_t) encode_d;
}

/**
 * @brief      bs_encode_double_to_byte_mqz(d)
 *
 * @details    Encodes double-type value between -1 and +1 to 
 *             byte-type byte "bin". If value is between
 *             (-0.25, +0.25) then encoding returns +0.00.
 *
 * @param      d      (double) value to be encoded
 *
 * @return     (byte_t) encoded score byte value
 */

inline byte_t
bs_encode_double_to_byte_mqz(double d) 
{
    d += (d < 0) ? -kEpsilon : kEpsilon; /* jitter is used to deal with interval edges */
    d = bs_truncate_double_to_precision(d, 2);
    int encode_d = (int) ((d < 0) ? (ceil(d * 1000.0f)/10.0f + 100) : (floor(d * 1000.0f)/10.0f + 100)) + bs_signbit(-d);
    if ((encode_d > 76) && (encode_d < 127))
        encode_d = 100;
    return (byte_t) encode_d;
}

/**
 * @brief      bs_encode_double_to_byte_custom(d, min, max)
 *
 * @details    Encodes double-type value between -1 and +1 to 
 *             byte-type byte "bin". If value is between
 *             (min, max) then encoding returns +0.00.
 *
 * @param      d      (double) value to be encoded
 *             min    (double) minimum cutoff value for zero-encoding
 *             max    (double) maximum cutoff value for zero-encoding
 *
 * @return     (byte_t) encoded score byte value
 */

byte_t
bs_encode_double_to_byte_custom(double d, double min, double max) 
{
    d += (d < 0) ? -kEpsilon : kEpsilon; /* jitter is used to deal with interval edges */
    min += (min < 0) ? -kEpsilon : kEpsilon;
    max += (max < 0) ? -kEpsilon : kEpsilon;
    d = bs_truncate_double_to_precision(d, 2);
    min = bs_truncate_double_to_precision(min, 2);
    max = bs_truncate_double_to_precision(max, 2);
    int encode_d = (int) ((d < 0) ? (ceil(d * 1000.0f)/10.0f + 100) : (floor(d * 1000.0f)/10.0f + 100)) + bs_signbit(-d);
    int encode_min = (int) ((min < 0) ? (ceil(min * 1000.0f)/10.0f + 100) : (floor(min * 1000.0f)/10.0f + 100)) + bs_signbit(-min);
    int encode_max = (int) ((max < 0) ? (ceil(max * 1000.0f)/10.0f + 100) : (floor(max * 1000.0f)/10.0f + 100)) + bs_signbit(-max);
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
 * @param      d      (double) value to have its sign evaluated
 *
 * @return     (boolean_t) true, if sign is negative; false, if sign is positive
 */

inline boolean_t
bs_signbit(double d)
{
    return (signbit(d) > 0) ? kTrue : kFalse;
}

/**
 * @brief      bs_decode_byte_to_double(uc)
 *
 * @details    Decodes byte-type byte bin to
 *             equivalent score bin.
 *
 * @param      uc     (byte) value to be decoded
 *
 * @return     (double) decoded score bin start value
 *
 * @example    bs_decode_byte_to_double(0x64) = -0.00 -- or bin (-0.01, -0.00]
 *             bs_decode_byte_to_double(0x65) = +0.00 -- or bin [+0,00, +0.01)
 *             bs_decode_byte_to_double(0x73) = +0.14 -- or bin [+0,14, +0.15)
 */

static inline double
bs_decode_byte_to_double(byte_t uc)
{
    return bs_encode_byte_to_double_table[uc];
}

/**
 * @brief      bs_decode_byte_to_double_mqz(uc)
 *
 * @details    Decodes byte-type byte bin to
 *             equivalent mid-quarter-zero score bin.
 *
 * @param      uc     (byte_t) value to be decoded
 *
 * @return     (double) decoded score bin start value
 */

static inline double
bs_decode_byte_to_double_mqz(byte_t uc)
{
    return bs_encode_byte_to_double_mqz_table[uc];
}

/**
 * @brief      bs_decode_byte_to_double_custom(uc)
 *
 * @details    Decodes byte-type byte bin to
 *             equivalent custom zero-ranged score bin.
 *
 * @param      uc     (byte_t) value to be decoded
 *             min    (double) minimum value to be decoded to zero byte
 *             max    (double) maximum value to be decoded to zero byte
 *
 * @return     (double) decoded score bin start value
 */

static inline double
bs_decode_byte_to_double_custom(byte_t uc, double min, double max)
{
    double test = bs_encode_byte_to_double_table[uc];
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
 *             start  (uint32_t*) start index to be populated
 *             end    (uint32_t*) end index to be populated
 *
 * @return     (boolean_t) whether rows were found in provided range
 */

boolean_t
bs_parse_query_range_str(lookup_t* l, char* rs, uint32_t* start, uint32_t* end)
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
    uint32_t start_elem_idx;
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
    uint32_t end_elem_idx;
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
    int32_t* entries = NULL;
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
        sscanf(entry_buf, "%d", &entries[entry_idx++]);
        start = end + 1;
    } while (!finished);
    
    if (entry_idx == 0) {
        free(entries), entries = NULL;
        return index_not_found_flag;
    }
    
    /* check that all indices are between 0 and l->nelems-1 */
    uint32_t current_idx = 0;
    int32_t max_entry = INT32_MIN;
    do {
        if (entries[current_idx] > max_entry) {
            max_entry = entries[current_idx];
        }
        current_idx++;
    } while (current_idx < entry_idx);
    if (max_entry > ((int32_t) l->nelems - 1)) {
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
    
    /* clean up entries */
    free(entries), entries = NULL;
    
    /* sort entries array copy */
    qsort(bs_globals.store_query_indices, 
          bs_globals.store_query_indices_num, 
          sizeof(*bs_globals.store_query_indices), 
          bs_parse_query_multiple_index_str_comparator);
    
    return indices_found_flag;
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
bs_parse_query_str_to_indices(char* qs, uint32_t* start, uint32_t* end)
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
    
    *start = (uint32_t) strtol(start_str, NULL, 10);
    *end = (uint32_t) strtol(end_str, NULL, 10);
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
 * @brief      bs_init_lookup(fn, pi)
 *
 * @details    Read BED-formatted coordinates into a "lookup table" pointer.
 *             Function allocates memory to lookup table pointer, as needed.
 *
 * @param      fn     (char*) filename string
 *             pi     (boolean_t) flag to decide whether to parse ID string
 *
 * @return     (lookup_t*) lookup table pointer referencing element data
 */

lookup_t*
bs_init_lookup(char* fn, boolean_t pi)
{
    lookup_t* l = NULL;
    FILE* lf = NULL;
    char buf[BUF_MAX_LEN] = {0};
    char chr_str[BUF_MAX_LEN] = {0};
    char start_str[BUF_MAX_LEN] = {0};
    char stop_str[BUF_MAX_LEN] = {0};
    char id_str[BUF_MAX_LEN] = {0};
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
    while (fgets(buf, BUF_MAX_LEN, lf)) {
        sscanf(buf, "%s\t%s\t%s\t%s\n", chr_str, start_str, stop_str, id_str);
        sscanf(start_str, "%" SCNu64, &start_val);
        sscanf(stop_str, "%" SCNu64, &stop_val);
        element_t* e = bs_init_element(chr_str, start_val, stop_val, id_str, pi);
        bs_push_elem_to_lookup(e, &l, pi);
    }

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
        bs_increment_lookup_frequency(freq_table, l);
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
            bs_encode_byte_to_double_table[start_bin_idx],
            bs_encode_byte_to_double_table[end_bin_idx]);

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
 * @details    Apply unbiased Fisher-Yates shuffle on double array entries.
 *
 * @param      d      (double*) data pointer
 *             n      (size_n) number of elements in data pointer
 */

void
bs_shuffle_signal_data(double* d, size_t n)
{
    /* cf. http://blog.codinghorror.com/the-danger-of-naivete/ */
    if (n > 1) {
        for (size_t i = 0; i < n; i++) {   
            size_t s = i + (mt19937_generate_random_ulong() % (n - i));
            double t = d[s];
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
    bs_increment_lookup_frequency(freq_table, l);
    uint64_t n_bytes = (uint64_t) l->nelems * l->nelems;
    bs_print_frequency_buffer(freq_table, n_bytes, os);
    free(freq_table), freq_table = NULL;
}

/**
 * @brief      bs_increment_lookup_frequency(t, l)
 *
 * @details    Update score frequency table from lookup struct.
 *
 * @param      t      (uint64_t*) frequency table
 *             l      (lookup_t*) pointer to lookup struct
 */

void
bs_increment_lookup_frequency(uint64_t* t, lookup_t* l)
{
    byte_t self_correlation_score =
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_double_to_byte(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_double_to_byte_mqz(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_double_to_byte_custom(kSelfCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_double_to_byte(kSelfCorrelationScore);

    for (uint32_t row_idx = 0; row_idx < l->nelems; row_idx++) {
        signal_t* row_signal = l->elems[row_idx]->signal;
        t[self_correlation_score]++;
        for (uint32_t col_idx = row_idx + 1; col_idx < l->nelems; col_idx++) {
            signal_t* col_signal = l->elems[col_idx]->signal;
            double corr = bs_pearson_r_signal(row_signal, col_signal);
            byte_t corr_uc =
                (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_double_to_byte(corr) : 
                (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_double_to_byte_mqz(corr) : 
                bs_encode_double_to_byte_custom(corr, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
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
 * @brief      bs_init_signal(cds)
 *
 * @details    Initialize a signal_t pointer with a vector of doubles,
 *             along with mean and sample standard deviation of the
 *             vector.
 *
 * @param      cds    (char*) pointer to comma-delimited string of numerical values
 *
 * @return     (signal_t*) pointer to signal struct populated with signal data
 */

signal_t*
bs_init_signal(char* cds)
{
    signal_t* s = NULL;
    s = malloc(sizeof(signal_t));
    if (!s) {
        fprintf(stderr, "Error: Could not allocate space for signal pointer!\n");
        exit(EXIT_FAILURE);
    }
    s->n = 1;
    s->data = NULL;
    s->mean = 0.0f;
    s->sd = 0.0f;
    for (uint32_t idx = 0; idx < strlen(cds); idx++) {
        if (cds[idx] == kSignalDelim) {
            s->n++;
        }
    }
    s->data = malloc(sizeof(*s->data) * s->n);
    if (!s->data) {
        fprintf(stderr, "Error: Could not allocate space for signal data pointer!\n");
        exit(EXIT_FAILURE);
    }
    char* start = cds;
    char* end = cds;
    char entry_buf[ENTRY_MAX_LEN];
    uint32_t entry_idx = 0;
    boolean_t finished = kFalse;
    do {
        end = strchr(start, kSignalDelim);
        if (!end) {
            end = cds + strlen(cds);
            finished = kTrue;
        }
        memcpy(entry_buf, start, end - start);
        entry_buf[end - start] = '\0';
        sscanf(entry_buf, "%lf", &s->data[entry_idx++]);
        start = end + 1;
    } while (!finished);
    s->mean = bs_mean_signal(s->data, s->n);
    if (s->n >= 2) {
        s->sd = bs_sample_sd_signal(s->data, s->n, s->mean);
    }
    else {
        fprintf(stderr, "Warning: Vector has one value and therefore does not have a standard deviation!\n");
    }
    return s;
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
 *             provided array of doubles of given length
 *
 * @param      d      (double*) pointer to doubles
 *             len    (uint32_t) length of double array
 *
 * @return     (double) mean value of double array
 */

double
bs_mean_signal(double* d, uint32_t len)
{
    double s = 0.0f;
    for (uint32_t idx = 0; idx < len; idx++) {
        s += d[idx];
    }
    return s / len;
}

/**
 * @brief      bs_sample_sd_signal(d, len, m)
 *
 * @details    Calculates the sample standard deviation of the 
 *             provided array of doubles of given length and mean
 *
 * @param      d      (double*) pointer to doubles
 *             len    (uint32_t) length of double array
 *             m      (double) arithmetic mean of double array
 *
 * @return     (double) sample standard deviation value of double array
 */

double
bs_sample_sd_signal(double* d, uint32_t len, double m)
{
    double s = 0.0f;
    for (uint32_t idx = 0; idx < len; idx++) {
        s += (d[idx] - m) * (d[idx] - m);
    }
    return sqrt(s / (len - 1));
}

/**
 * @brief      bs_pearson_r_signal(a, b)
 *
 * @details    Calculates the Pearson's r correlation of two
 *             signal vectors
 *
 * @param      a      (signal_t*) pointer to first signal struct
 *             b      (signal_t*) pointer to second signal struct
 *
 * @return     (double) Pearson's r correlation score result
 */

double
bs_pearson_r_signal(signal_t* a, signal_t* b)
{
    if (a->n != b->n) {
        fprintf(stderr, "Error: Vectors being correlated are of unequal length!\n");
        bs_print_signal(a, stderr);
        bs_print_signal(b, stderr);
        exit(EXIT_FAILURE);
    }
    if ((a->sd == 0.0f) || (b->sd == 0.0f)) {
        fprintf(stderr, "Error: Vectors must have non-zero standard deviation!\n");
        bs_print_signal(a, stderr);
        bs_print_signal(b, stderr);
        exit(EXIT_FAILURE);
    }
    double s = 0.0f;
    for (uint32_t idx = 0; idx < a->n; idx++)
        s += (a->data[idx] - a->mean) * (b->data[idx] - b->mean);
    return s / ((a->n - 1.0f) * a->sd * b->sd);
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
    free(*s), *s = NULL;
}

/**
 * @brief      bs_init_element(chr, start, stop, id, pi)
 *
 * @details    Allocates space for element_t* and copies chr, start, stop
 *             and id values to element.
 *
 * @param      chr    (char*) chromosome string 
 *             start  (uint64_t) start coordinate position
 *             stop   (uint64_t) stop coordinate position
 *             id     (char*) id string 
 *             pi     (boolean_t) parse ID string
 *
 * @return     (element_t*) element pointer
 */

element_t*
bs_init_element(char* chr, uint64_t start, uint64_t stop, char* id, boolean_t pi)
{
    element_t *e = NULL;

    e = malloc(sizeof(element_t));
    if (!e) {
        fprintf(stderr, "Error: Could not allocate space for element!\n");
        exit(EXIT_FAILURE);
    }
    e->chr = NULL;
    if (strlen(chr) > 0) {
        e->chr = malloc(sizeof(*chr) * strlen(chr) + 1);
        if (!e->chr) {
            fprintf(stderr, "Error: Could not allocate space for element chromosome!\n");
            exit(EXIT_FAILURE);
        }
        memcpy(e->chr, chr, strlen(chr) + 1);
    }
    e->start = start;
    e->stop = stop;
    e->id = NULL;
    if (id && (strlen(id) > 0) && pi) {
        e->id = malloc(sizeof(*id) * strlen(id) + 1);
        if (!e->id) {
            fprintf(stderr,"Error: Could not allocate space for element id!\n");
            exit(EXIT_FAILURE);
        }
        memcpy(e->id, id, strlen(id) + 1);
    }
    e->signal = (e->id && pi) ? bs_init_signal(e->id) : NULL;    
    return e;
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
 * @brief      bs_push_elem_to_lookup(e, l)
 *
 * @details    Pushes element_t pointer to lookup table.
 *
 * @param      e      (element_t*) element pointer
 *             l      (lookup_t**) pointer to lookup table pointer
 *             pi     (boolean_t) parse ID string
 */

void
bs_push_elem_to_lookup(element_t* e, lookup_t** l, boolean_t pi)
{
    if ((*l)->capacity == 0) {
        (*l)->capacity++;
        (*l)->elems = malloc(sizeof(element_t *));
    }
    else if ((*l)->nelems >= (*l)->capacity) {
        (*l)->capacity *= 2;
        element_t** new_elems = malloc(sizeof(element_t *) * (*l)->capacity);
        for (uint32_t idx = 0; idx < (*l)->nelems; idx++) {
            new_elems[idx] = bs_init_element((*l)->elems[idx]->chr,
                                             (*l)->elems[idx]->start,
                                             (*l)->elems[idx]->stop,
                                             (*l)->elems[idx]->id,
                                             pi);
            bs_delete_element(&((*l)->elems[idx]));
        }   
        (*l)->elems = new_elems;     
    }
    uint32_t n = (*l)->nelems;
    (*l)->elems[n] = e;
    (*l)->nelems++;
}

/**
 * @brief      bs_test_pearsons_r()
 *
 * @details    Tests calculation and encoding of Pearson's 
 *             r score from test vectors
 */

void
bs_test_pearsons_r()
{
    signal_t* a = NULL;
    a = bs_init_signal((char*) kPearsonRTestVectorA);
    if (!a) {
        fprintf(stderr, "Error: Could not allocate space for test (A) Pearson's r vector!\n");
        exit(EXIT_FAILURE);
    }
    signal_t* b = NULL;
    b = bs_init_signal((char*) kPearsonRTestVectorB);
    if (!b) {
        fprintf(stderr, "Error: Could not allocate space for test (B) Pearson's r vector!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Comparing AB\n---\nA -> %s\nB -> %s\n---\n", kPearsonRTestVectorA, kPearsonRTestVectorB);
    double unencoded_observed_score = bs_pearson_r_signal(a, b);
    fprintf(stderr, "Expected - unencoded AB Pearson's r score: %3.6f\n", kPearsonRTestCorrelationUnencoded);
    fprintf(stderr, "Observed - unencoded AB Pearson's r score: %3.6f\n", unencoded_observed_score);
    double absolute_diff_unencoded_scores = fabs(kPearsonRTestCorrelationUnencoded - unencoded_observed_score);
    assert(absolute_diff_unencoded_scores + kEpsilon > 0 && absolute_diff_unencoded_scores - kEpsilon < 0);
    fprintf(stderr, "\t-> Expected and observed scores do not differ within %3.7f error\n", kEpsilon);

    byte_t encoded_expected_score_byte = bs_encode_double_to_byte(kPearsonRTestCorrelationUnencoded);
    byte_t encoded_observed_score_byte = bs_encode_double_to_byte(unencoded_observed_score);
    fprintf(stderr, "Expected - encoded, precomputed AB Pearson's r score: 0x%02x\n", kPearsonRTestCorrelationEncodedByte);
    fprintf(stderr, "Expected - encoded, computed AB Pearson's r score: 0x%02x\n", encoded_expected_score_byte);
    fprintf(stderr, "Observed - encoded, computed AB Pearson's r score: 0x%02x\n", encoded_observed_score_byte);
    assert(kPearsonRTestCorrelationEncodedByte == encoded_expected_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and computed scores do not differ\n");
    assert(kPearsonRTestCorrelationEncodedByte == encoded_observed_score_byte);
    fprintf(stderr, "\t-> Expected precomputed and observed computed scores do not differ\n");
    assert(encoded_expected_score_byte == encoded_observed_score_byte);
    fprintf(stderr, "\t-> Expected computed and observed computed scores do not differ\n");

    bs_delete_signal(&a);
    bs_delete_signal(&b);
}

/**
 * @brief      bs_test_score_encoding()
 *
 * @details    Tests encoding of scores in the interval 
 *             [-1.0, +1.0] at kEpsilon increments. 
 *
 *             Encoded scores are byte_t byte values 
 *             equivalent to the provided score value. Decoded 
 *             scores are the double-typed "bin" with which the 
 *             original score associates.
 */

void
bs_test_score_encoding()
{
    double d, decode_d;
    int count;
    byte_t encode_d;

    if (bs_globals.encoding_strategy == kEncodingStrategyFull) {
        for (d = -1.0f, count = 0; d <= 1.0f; d += kEpsilon, ++count) {
            encode_d = bs_encode_double_to_byte(d);
            decode_d = bs_decode_byte_to_double(encode_d);
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
            encode_d = bs_encode_double_to_byte_mqz(d);
            decode_d = bs_decode_byte_to_double_mqz(encode_d);
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
            encode_d = bs_encode_double_to_byte_custom(d, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
            decode_d = bs_decode_byte_to_double_custom(encode_d, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
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
    bs_globals.store_query_str[0] = '\0';
    bs_globals.store_query_idx_start = kQueryIndexDefaultStart;
    bs_globals.store_query_idx_end = kQueryIndexDefaultEnd;
    bs_globals.store_query_indices = NULL;
    bs_globals.store_query_range_start = bs_init_bed(kQueryRangeDefaultChromosome, kQueryRangeDefaultStart, kQueryRangeDefaultEnd);
    bs_globals.store_query_range_end = bs_init_bed(kQueryRangeDefaultChromosome, kQueryRangeDefaultStart, kQueryRangeDefaultEnd);
    bs_globals.store_row_chunk_size = kRowChunkDefaultSize;
    bs_globals.store_row_chunk_offset = kRowChunkDefaultOffset;
    bs_globals.store_chunk_size_specified_flag = kFalse;
    bs_globals.store_single_chunk_flag = kFalse;
    bs_globals.store_compression_flag = kFalse;
    bs_globals.store_filter = kScoreDefaultFilter;
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
}

/**
 * @brief      bs_delete_globals()
 *
 * @details    Release application global variables from heap.
 */

void
bs_delete_globals()
{
    if (bs_globals.store_query_indices) { free(bs_globals.store_query_indices), bs_globals.store_query_indices = NULL; }
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
            bs_output_flag_counter++;
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
            sscanf(optarg, "%lf", &bs_globals.score_filter_cutoff);
            bs_score_filter_counter++;
            break;
        case '3':
            bs_globals.store_filter = kScoreFilterGt;
            if (!optarg) {
                fprintf(stderr, "Error: Store filter operation parameter specified without filter cutoff value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%lf", &bs_globals.score_filter_cutoff);
            bs_score_filter_counter++;
            break;
        case '4':
            bs_globals.store_filter = kScoreFilterEq;
            if (!optarg) {
                fprintf(stderr, "Error: Store filter operation parameter specified without filter cutoff value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%lf", &bs_globals.score_filter_cutoff);
            bs_score_filter_counter++;
            break;
        case '5':
            bs_globals.store_filter = kScoreFilterLtEq;
            if (!optarg) {
                fprintf(stderr, "Error: Store filter operation parameter specified without filter cutoff value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%lf", &bs_globals.score_filter_cutoff);
            bs_score_filter_counter++;
            break;
        case '6':
            bs_globals.store_filter = kScoreFilterLt;
            if (!optarg) {
                fprintf(stderr, "Error: Store filter operation parameter specified without filter cutoff value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%lf", &bs_globals.score_filter_cutoff);
            bs_score_filter_counter++;
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
            sscanf(optarg, "%lf", &bs_globals.encoding_cutoff_zero_min);
            break;
        case 'x':
            if (!optarg) {
                fprintf(stderr, "Error: Encoding cutoff parameter specified without cutoff maximum value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%lf", &bs_globals.encoding_cutoff_zero_max);
            break;
        case 'm':
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
            sscanf(optarg, "%lf", &bs_globals.permutation_precision);
            break;
        case 'a':
            bs_permutation_flag_counter++;
            if (!optarg) {
                fprintf(stderr, "Error: Permutation alpha parameter specified without alpha value!\n");
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            sscanf(optarg, "%lf", &bs_globals.permutation_alpha);
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
        case '1':
            bs_test_pearsons_r();
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
            "     %s --store-query --store-type [ type-of-store ] --lookup=fn --store=fn [ --index-query=str | --multiple-index-query=str | --range-query=str ] [ --score-filter-gteq=float | --score-filter-gt=float | --score-filter-eq=float | --score-filter-lteq=float | --score-filter-lt=float ]\n\n" \
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
 *             double   (double) correlation score
 */ 

inline void
bs_print_pair(FILE* os, char* chr_a, uint64_t start_a, uint64_t stop_a, char* chr_b, uint64_t start_b, uint64_t stop_b, double score)
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
 * @brief      bs_populate_sut_store_with_pearsonr_scores(s, l)
 *
 * @details    Write Pearson's r correlation scores as encoded 
 *             byte_t bytes to a FILE* handle associated 
 *             with the specified SUT store filename.
 *
 * @param      s      (sut_store_t*) pointer to SUT struct
 *             l      (lookup_t*) pointer to lookup table
 */

void
bs_populate_sut_store_with_pearsonr_scores(sut_store_t* s, lookup_t* l)
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

    /* write Pearson's r correlation scores to output stream ptr */
    for (uint32_t row_idx = 0; row_idx < s->attr->nelems; row_idx++) {
        signal_t* row_signal = l->elems[row_idx]->signal;
        for (uint32_t col_idx = row_idx + 1; col_idx < s->attr->nelems; col_idx++) {
            signal_t* col_signal = l->elems[col_idx]->signal;
            double corr = bs_pearson_r_signal(row_signal, col_signal);
            buf[s_buf++] = 
                (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_double_to_byte(corr) : 
                (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_double_to_byte_mqz(corr) : 
                bs_encode_double_to_byte_custom(corr, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
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
            double d = (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_double((byte_t) fgetc(is)) : bs_decode_byte_to_double_mqz((byte_t) fgetc(is));
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
 *             fc     (double) score filter cutoff
 *             fo     (score_filter_t) score filter operation
 */

void
bs_print_sut_filtered_store_to_bed7(lookup_t* l, sut_store_t* s, FILE* os, double fc, score_filter_t fo)
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
            double d = (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_double((byte_t) fgetc(is)) : bs_decode_byte_to_double_mqz((byte_t) fgetc(is));
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
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_double_to_byte(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_double_to_byte_mqz(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_double_to_byte_custom(kSelfCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_double_to_byte(kSelfCorrelationScore);
    byte_t no_correlation_score =
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_double_to_byte(kNoCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_double_to_byte_mqz(kNoCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_double_to_byte_custom(kNoCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_double_to_byte(kNoCorrelationScore);
    
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
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_double_to_byte(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_double_to_byte_mqz(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_double_to_byte_custom(kSelfCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_double_to_byte(kSelfCorrelationScore);
    
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
 * @brief      bs_populate_sqr_store_with_pearsonr_scores(s, l)
 *
 * @details    Write Pearson's r correlation scores as encoded
 *             byte_t bytes to a FILE* handle associated 
 *             with the specified square matrix store filename.
 *
 * @param      s      (sqr_store_t*) pointer to square matrix store
 *             l      (lookup_t*) pointer to lookup table
 */

void
bs_populate_sqr_store_with_pearsonr_scores(sqr_store_t* s, lookup_t* l)
{
    FILE* os = NULL;
    byte_t self_correlation_score =
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_double_to_byte(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_double_to_byte_mqz(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_double_to_byte_custom(kSelfCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_double_to_byte(kSelfCorrelationScore);

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
    
    /* write Pearson's r correlation scores to output stream ptr */
    for (uint32_t row_idx = 0; row_idx < s->attr->nelems; row_idx++) {
        signal_t* row_signal = l->elems[row_idx]->signal;
        for (uint32_t col_idx = 0; col_idx < s->attr->nelems; col_idx++) {
            signal_t* col_signal = l->elems[col_idx]->signal;
            if (row_idx != col_idx) {
                double corr = bs_pearson_r_signal(row_signal, col_signal);
                buf[s_buf++] = 
                    (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_double_to_byte(corr) : 
                    (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_double_to_byte_mqz(corr) : 
                    bs_encode_double_to_byte_custom(corr, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
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
    block_dest_dir = malloc(strlen(p) + strlen(kRawMetadataSplitDirSuffix) + 2);
    if (!block_dest_dir) {
        fprintf(stderr, "Error: Could not allocate space for block destination string!\n");
        exit(EXIT_FAILURE);
    }
    snprintf(block_dest_dir, strlen(p) + strlen(kRawMetadataSplitDirSuffix) + 2, "%s.%s", p, kRawMetadataSplitDirSuffix);
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
 * @brief      bs_populate_sqr_split_store_with_pearsonr_scores(s, l, n)
 *
 * @details    Write each raw block of encoded Pearson's r correlation scores to 
 *             a FILE* handle associated with the specified square matrix store 
 *             filename. Each block and a metadata file are stored in a folder, 
 *             its name determined by the store filename.
 *
 * @param      s      (sqr_store_t*) pointer to square matrix store
 *             l      (lookup_t*) pointer to lookup table
 *             n      (uint32_t) number of rows within a raw chunk 
 */

void
bs_populate_sqr_split_store_with_pearsonr_scores(sqr_store_t* s, lookup_t* l, uint32_t n)
{
    byte_t score = 0;
    FILE* os = NULL;
    byte_t self_correlation_score =
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_double_to_byte(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_double_to_byte_mqz(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_double_to_byte_custom(kSelfCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_double_to_byte(kSelfCorrelationScore);
    
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
                double corr = bs_pearson_r_signal(row_signal, col_signal);
                score = 
                    (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_double_to_byte(corr) : 
                    (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_double_to_byte_mqz(corr) : 
                    bs_encode_double_to_byte_custom(corr, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
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
    md_str = bs_init_metadata_str(offsets, offset_idx, n);
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
 * @brief      bs_populate_sqr_split_store_chunk_with_pearsonr_scores(s, l, n, o)
 *
 * @details    Write one raw block of encoded Pearson's r correlation scores to 
 *             a FILE* handle associated with the specified square matrix store 
 *             filename. Each block and a metadata file are stored in a folder, 
 *             its name determined by the store filename. The folder is created 
 *             if it does not already exist.
 *
 * @param      s      (sqr_store_t*) pointer to square matrix store
 *             l      (lookup_t*) pointer to lookup table
 *             n      (uint32_t) number of rows within a raw chunk
 *             o      (uint32_t) offset to starting row (zero-indexed)  
 */

void
bs_populate_sqr_split_store_chunk_with_pearsonr_scores(sqr_store_t* s, lookup_t* l, uint32_t n, uint32_t o)
{
    byte_t score = 0;
    FILE* os = NULL;
    byte_t self_correlation_score =
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_double_to_byte(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_double_to_byte_mqz(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_double_to_byte_custom(kSelfCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_double_to_byte(kSelfCorrelationScore);
    
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
                double corr = bs_pearson_r_signal(row_signal, col_signal);
                score = 
                    (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_double_to_byte(corr) : 
                    (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_double_to_byte_mqz(corr) : 
                    bs_encode_double_to_byte_custom(corr, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
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
 * @brief      bs_populate_sqr_split_store_chunk_metadata(s, l, n)
 *
 * @details    Write metadata to FILE* handle associated with the specified 
 *             per-chunk square matrix store. Metadata are stored in a folder, 
 *             its name determined by the store filename. The folder is created 
 *             if it does not already exist.
 *
 * @param      s      (sqr_store_t*) pointer to square matrix store
 *             l      (lookup_t*) pointer to lookup table
 *             n      (uint32_t) number of rows within a raw chunk
 */

void
bs_populate_sqr_split_store_chunk_metadata(sqr_store_t* s, lookup_t* l, uint32_t n)
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
    md_str = bs_init_metadata_str(offsets, offset_idx, n);
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
 * @brief      bs_populate_sqr_bzip2_store_with_pearsonr_scores(s, l, n)
 *
 * @details    Write bzip2-compressed chunks of encoded Pearson's r 
 *             correlation scores to a FILE* handle associated with the 
 *             specified square matrix store filename.
 *
 * @param      s      (sqr_store_t*) pointer to square matrix store
 *             l      (lookup_t*) pointer to lookup table
 *             n      (uint32_t) number of rows within a compressed chunk 
 */

void
bs_populate_sqr_bzip2_store_with_pearsonr_scores(sqr_store_t* s, lookup_t* l, uint32_t n)
{
    byte_t score = 0;
    FILE* os = NULL;
    byte_t self_correlation_score =
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_double_to_byte(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_double_to_byte_mqz(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_double_to_byte_custom(kSelfCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_double_to_byte(kSelfCorrelationScore);

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
                double corr = bs_pearson_r_signal(row_signal, col_signal);
                score = 
                    (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_double_to_byte(corr) : 
                    (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_double_to_byte_mqz(corr) : 
                    bs_encode_double_to_byte_custom(corr, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
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
    md_str = bs_init_metadata_str(offsets, offset_idx, n);
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
 * @brief      bs_populate_sqr_bzip2_split_store_with_pearsonr_scores(s, l, n)
 *
 * @details    Write each bzip2-compressed block of encoded Pearson's r 
 *             correlation scores to a FILE* handle associated with the 
 *             specified square matrix store filename. Each block and a metadata
 *             file are stored in a folder, its name determined by the store
 *             filename.
 *
 * @param      s      (sqr_store_t*) pointer to square matrix store
 *             l      (lookup_t*) pointer to lookup table
 *             n      (uint32_t) number of rows within a compressed chunk 
 */

void
bs_populate_sqr_bzip2_split_store_with_pearsonr_scores(sqr_store_t* s, lookup_t* l, uint32_t n)
{
    byte_t score = 0;
    FILE* os = NULL;
    byte_t self_correlation_score =
        (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_double_to_byte(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_double_to_byte_mqz(kSelfCorrelationScore) :
        (bs_globals.encoding_strategy == kEncodingStrategyCustom) ? bs_encode_double_to_byte_custom(kSelfCorrelationScore, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max) :
        bs_encode_double_to_byte(kSelfCorrelationScore);

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
                double corr = bs_pearson_r_signal(row_signal, col_signal);
                score = 
                    (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_encode_double_to_byte(corr) : 
                    (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_encode_double_to_byte_mqz(corr) : 
                    bs_encode_double_to_byte_custom(corr, bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
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
    md_str = bs_init_metadata_str(offsets, offset_idx, n);
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
    block_dest_dir = malloc(strlen(p) + strlen(kCompressionMetadataSplitDirSuffix) + 2);
    if (!block_dest_dir) {
        fprintf(stderr, "Error: Could not allocate space for block destination string!\n");
        exit(EXIT_FAILURE);
    }
    snprintf(block_dest_dir, strlen(p) + strlen(kCompressionMetadataSplitDirSuffix) + 2, "%s.%s", p, kCompressionMetadataSplitDirSuffix);
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
 * @brief      bs_init_metadata_str(o, n, s)
 *
 * @details    Prints formatted metadata string from block
 *             offset array and array length.
 *
 * @param      o      (off_t*) array of block offsets
 *             n      (uint32_t) number of offsets in array
 *             s      (uint32_t) size of a row block
 *
 * @return     (char*) formatted metadata string 
 */

char*
bs_init_metadata_str(off_t* o, uint32_t n, uint32_t s)
{
    /* 
       byte store metadata format (v1.0)
       ---------------------------------
       "version|row_block_size|number_of_offsets|offset_1|offset_2|offset_3|...|offset_n|\0"
    */

    size_t m_size = 0;
    size_t m_len = (OFFSET_MAX_LEN + 1) * n + 1; 
    char* m_str = NULL;

    m_str = calloc(m_len, sizeof(*m_str));
    if (!m_str) {
        fprintf(stderr, "Error: Could not allocate space for compression offset metadata string!\n");
        exit(EXIT_FAILURE);
    }

    m_size += sprintf(m_str + m_size, "%3.1f%c", kCompressionMetadataVersion, kCompressionMetadataDelimiter);
    m_size += sprintf(m_str + m_size, "%d%c", s, kCompressionMetadataDelimiter);
    m_size += sprintf(m_str + m_size, "%d%c", n, kCompressionMetadataDelimiter);

    for (uint32_t o_idx = 0; o_idx < n; o_idx++) {
        m_size += sprintf(m_str + m_size, "%" PRIu64 "%c", o[o_idx], kCompressionMetadataDelimiter);
    }
    m_str[m_size] = '\0';
    m_size += sprintf(m_str + m_size, "%0*zu", (int) MD_OFFSET_MAX_LEN, strlen(m_str));

    return m_str;
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
 * @brief      bs_print_sqr_store_to_bed7(l, s, os)
 *
 * @details    Queries square matrix store for provided index range 
 *             globals and prints BED7 (BED3 + BED3 + floating point) 
 *             to specified output stream. The two BED3 elements 
 *             are retrieved from the lookup table and represent 
 *             a score pairing.
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             os     (FILE*) pointer to output stream
 */

void
bs_print_sqr_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os)
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

    /* fseek(is) to the starting offset */
    off_t start_offset = bs_sqr_byte_offset_for_element_ij(l->nelems, bs_globals.store_query_idx_start, 0);
    fseek(is, start_offset, SEEK_SET);

    uint32_t row_idx = bs_globals.store_query_idx_start;
    uint32_t col_idx = 0;

    do {
        if (fread(byte_buf, sizeof(*byte_buf), l->nelems, is) != l->nelems) {
            fprintf(stderr, "Error: Could not read a row of data from sqr input stream!\n");
            exit(EXIT_FAILURE);
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
                              (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_double(byte_buf[col_idx]) :
                              (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_double_mqz(byte_buf[col_idx]) :
                              bs_decode_byte_to_double_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max)
                              );
            }
            col_idx++;
        } while (col_idx < l->nelems);
        row_idx++;
        col_idx = 0;
    } while (row_idx <= bs_globals.store_query_idx_end);

    free(byte_buf);

    fclose(is);
}

/**
 * @brief      bs_print_sqr_filtered_store_to_bed7(l, s, os, fc, fo)
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
 *             os     (FILE*) pointer to output stream
 *             fc     (double) score filter cutoff
 *             fo     (score_filter_t) score filter operation
 */

void
bs_print_sqr_filtered_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os, double fc, score_filter_t fo)
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

    /* fseek(is) to the starting offset */
    off_t start_offset = bs_sqr_byte_offset_for_element_ij(l->nelems, bs_globals.store_query_idx_start, 0);
    fseek(is, start_offset, SEEK_SET);

    uint32_t row_idx = bs_globals.store_query_idx_start;
    uint32_t col_idx = 0;

    do {
        if (fread(byte_buf, sizeof(*byte_buf), l->nelems, is) != l->nelems) {
            fprintf(stderr, "Error: Could not read a row of data from sqr input stream!\n");
            exit(EXIT_FAILURE);
        }
        do {
            if (row_idx != col_idx) {
                double d = (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_double(byte_buf[col_idx]) :
                    (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_double_mqz(byte_buf[col_idx]) :
                    bs_decode_byte_to_double_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
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
            col_idx++;
        } while (col_idx < l->nelems);
        row_idx++;
        col_idx = 0;
    } while (row_idx <= bs_globals.store_query_idx_end);

    free(byte_buf);

    fclose(is);
}

/**
 * @brief      bs_print_sqr_split_store_to_bed7(l, s, os)
 *
 * @details    Queries raw square matrix store folder for
 *             provided index range globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output stream. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             os     (FILE*) pointer to output stream
 */

void
bs_print_sqr_split_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os)
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
    
    /* allocate byte buffer -- we read in one row of bytes at a time */
    byte_t* byte_buf = NULL;
    ssize_t n_byte_buf = l->nelems;
    byte_buf = malloc(n_byte_buf);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }
    
    /* iterate through query_start -> query_end blocks */
    for (uint32_t block_idx = query_start_block; block_idx <= query_end_block; block_idx++) {
        /* at start of block, open input stream */
        is = fopen(bs_init_sqr_split_store_fn_str(block_src_dir, block_idx), "rb");
        if (ferror(is)) {
            fprintf(stderr, "Error: Could not open handle to input store!\n");
            bs_print_usage(stderr);
            exit(EXIT_FAILURE);
        }
	
       /* set up bounds */
        uint32_t row_idx = block_idx * md->block_row_size;
        uint32_t col_idx = 0;
        ssize_t end_of_block_row_idx = ((block_idx + 1) * md->block_row_size - 1 < bs_globals.store_query_idx_end) ? (block_idx + 1) * md->block_row_size - 1 : bs_globals.store_query_idx_end;

        /* first offset the number of bytes required to get to the starting point within the split block, if necessary */
        if (row_idx < bs_globals.store_query_idx_start) {
            fseek(is, (bs_globals.store_query_idx_start - row_idx) * l->nelems, SEEK_SET);
            row_idx = bs_globals.store_query_idx_start;
        }

        do {
            /* read a row of bytes */
            fread(byte_buf, sizeof(*byte_buf), l->nelems, is);
            do {
                if ((row_idx != col_idx) && (row_idx >= bs_globals.store_query_idx_start) && (row_idx <= bs_globals.store_query_idx_end)) {
                    bs_print_pair(os, 
                                  l->elems[row_idx]->chr,
                                  l->elems[row_idx]->start,
                                  l->elems[row_idx]->stop,
                                  l->elems[col_idx]->chr,
                                  l->elems[col_idx]->start,
                                  l->elems[col_idx]->stop,
                                  (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_double(byte_buf[col_idx]) :
                                  (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_double_mqz(byte_buf[col_idx]) :
                                  bs_decode_byte_to_double_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max)
                                  );
                }
                col_idx++;
            } while (col_idx < l->nelems);
            col_idx = 0;
            row_idx++;
        } while (row_idx <= end_of_block_row_idx);
        
        /* at end of block, close input streams */
        fclose(is);
    }
    
    /* clean up */
    free(block_src_dir), block_src_dir = NULL;
    free(md_src_fn), md_src_fn = NULL;
    free(md_string), md_string = NULL;
    free(md), md = NULL;
    free(byte_buf), byte_buf = NULL;
}

/**
 * @brief      bs_print_sqr_filtered_split_store_to_bed7(l, s, os, fc, fo)
 *
 * @details    Queries raw split square matrix store for 
 *             provided index range globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output stream. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             os     (FILE*) pointer to output stream
 *             fc     (double) score filter cutoff
 *             fo     (score_filter_t) score filter operation
 */

void
bs_print_sqr_filtered_split_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os, double fc, score_filter_t fo)
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
    
    /* allocate byte buffer -- we read in one row at a time */
    byte_t* byte_buf = NULL;
    ssize_t n_byte_buf = l->nelems;
    byte_buf = malloc(n_byte_buf);
    if (!byte_buf) {
        fprintf(stderr, "Error: Could not allocate memory to sqr byte buffer!\n");
        exit(EXIT_FAILURE);
    }
    
    /* iterate through query_start -> query_end blocks */
    for (uint32_t block_idx = query_start_block; block_idx <= query_end_block; block_idx++) {
        /* at start of block, open input stream */
        is = fopen(bs_init_sqr_split_store_fn_str(block_src_dir, block_idx), "rb");
        if (ferror(is)) {
            fprintf(stderr, "Error: Could not open handle to input store!\n");
            bs_print_usage(stderr);
            exit(EXIT_FAILURE);
        }
	
        /* set up bounds */
        uint32_t row_idx = block_idx * md->block_row_size;
        uint32_t col_idx = 0;
        ssize_t end_of_block_row_idx = ((block_idx + 1) * md->block_row_size - 1 < bs_globals.store_query_idx_end) ? (block_idx + 1) * md->block_row_size - 1 : bs_globals.store_query_idx_end;

        /* first offset the number of bytes required to get to the starting point within the split block, if necessary */
        if (row_idx < bs_globals.store_query_idx_start) {
            fseek(is, (bs_globals.store_query_idx_start - row_idx) * l->nelems, SEEK_SET);
            row_idx = bs_globals.store_query_idx_start;
        }

        do {
            /* read a row of bytes */
            fread(byte_buf, sizeof(*byte_buf), l->nelems, is);
            do {
                if ((row_idx != col_idx) && (row_idx >= bs_globals.store_query_idx_start) && (row_idx <= bs_globals.store_query_idx_end)) {
                    double d = (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_double(byte_buf[col_idx]) :
                        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_double_mqz(byte_buf[col_idx]) :
                        bs_decode_byte_to_double_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
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
                col_idx++;
            } while (col_idx < l->nelems);
            col_idx = 0;
            row_idx++;
        } while (row_idx <= end_of_block_row_idx);
        
        /* at end of block, close input streams */
        fclose(is);
    }
    
    /* clean up */
    free(block_src_dir), block_src_dir = NULL;
    free(md_src_fn), md_src_fn = NULL;
    free(md_string), md_string = NULL;
    free(md), md = NULL;
    free(byte_buf), byte_buf = NULL;
}

/**
 * @brief      bs_print_sqr_split_store_separate_rows_to_bed7(l, s, os, r, rn)
 *
 * @details    Queries raw square matrix store folder for
 *             provided multiple-index globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output stream. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             os     (FILE*) pointer to output stream
 *             r      (int32_t*) pointer to list of query rows of interest 
 *             rn     (uint32) number of rows of query interest
 */

void
bs_print_sqr_split_store_separate_rows_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os, int32_t* r, uint32_t rn)
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
    uint32_t row_idx = 0;
    uint32_t col_idx = 0;
    for (uint32_t r_idx = 0; r_idx < rn; r_idx++) {
        int32_t query_row = r[r_idx];
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
        if (bytes_to_go > 0) {
            fseek(is, bytes_to_go, SEEK_CUR);
        }

        /* read a row from current block and print its signal to the output stream os */
        row_idx = (uint32_t) query_row;
        col_idx = 0;
        fread(byte_buf, sizeof(*byte_buf), l->nelems, is);
        do {
            if (row_idx != col_idx) {
                bs_print_pair(os, 
                              l->elems[row_idx]->chr,
                              l->elems[row_idx]->start,
                              l->elems[row_idx]->stop,
                              l->elems[col_idx]->chr,
                              l->elems[col_idx]->start,
                              l->elems[col_idx]->stop,
                              (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_double(byte_buf[col_idx]) :
                              (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_double_mqz(byte_buf[col_idx]) :
                              bs_decode_byte_to_double_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max)
                              );
            }
            col_idx++;
        } while (col_idx < l->nelems);
        
        /* set current block index */
        current_block_idx = new_block_idx;
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
 * @brief      bs_print_sqr_filtered_split_store_separate_rows_to_bed7(l, s, os, r, fc, fo)
 *
 * @details    Queries raw split square matrix store for 
 *             provided multiple-index globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output stream. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             os     (FILE*) pointer to output stream
 *             r      (int32_t*) pointer to list of query rows of interest  
 *             rn     (uint32_t) number of query rows of interest
 *             fc     (double) score filter cutoff
 *             fo     (score_filter_t) score filter operation
 */

void
bs_print_sqr_filtered_split_store_separate_rows_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os, int32_t* r, uint32_t rn, double fc, score_filter_t fo) 
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
    uint32_t row_idx = 0;
    uint32_t col_idx = 0;
    for (uint32_t r_idx = 0; r_idx < rn; r_idx++) {
        int32_t query_row = r[r_idx];
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
        /* offset some number of bytes from current position of is, if necessary */
        /* note that we subtract a row unit, if we have already read through the input stream by one row */
        int32_t row_diff = query_row - row_idx - ((current_block_idx == -1) ? 0 : 1);
        int32_t bytes_to_go = row_diff * l->nelems;
        if (bytes_to_go > 0) {
            fseek(is, bytes_to_go, SEEK_CUR);
        }
        /* read a row from current block and print its signal to the output stream os */
        row_idx = (uint32_t) query_row;
        col_idx = 0;
        fread(byte_buf, sizeof(*byte_buf), l->nelems, is);
        do {
            if (row_idx != col_idx) {
                double d = (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_double(byte_buf[col_idx]) :
                    (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_double_mqz(byte_buf[col_idx]) :
                    bs_decode_byte_to_double_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
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
            col_idx++;
        } while (col_idx < l->nelems);
        
        /* set current block index */
        current_block_idx = new_block_idx;
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
                                  (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_double(byte_buf[col_idx]) :
                                  (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_double_mqz(byte_buf[col_idx]) :
                                  bs_decode_byte_to_double_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max)
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
 * @brief      bs_print_sqr_filtered_bzip2_store_to_bed7(l, s, os, fc, fo)
 *
 * @details    Queries bzip2-compressed square matrix store for 
 *             provided index range globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output stream. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             os     (FILE*) pointer to output stream
 *             fc     (double) score filter cutoff
 *             fo     (score_filter_t) score filter operation
 */

void
bs_print_sqr_filtered_bzip2_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os, double fc, score_filter_t fo)
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
                    double d = (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_double(byte_buf[col_idx]) :
                        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_double_mqz(byte_buf[col_idx]) :
                        bs_decode_byte_to_double_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
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
                                  (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_double(byte_buf[col_idx]) :
                                  (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_double_mqz(byte_buf[col_idx]) :
                                  bs_decode_byte_to_double_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max)
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
 * @brief      bs_print_sqr_filtered_bzip2_split_store_to_bed7(l, s, os, fc, fo)
 *
 * @details    Queries bzip2-compressed square matrix store folder 
 *             for provided index range globals and prints BED7 (BED3 
 *             + BED3 + floating point) to specified output stream. 
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             s      (sqr_store_t*) pointer to square matrix store
 *             os     (FILE*) pointer to output stream
 *             fc     (double) score filter cutoff
 *             fo     (score_filter_t) score filter operation
 */

void
bs_print_sqr_filtered_bzip2_split_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os, double fc, score_filter_t fo)
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
                    double d = (bs_globals.encoding_strategy == kEncodingStrategyFull) ? bs_decode_byte_to_double(byte_buf[col_idx]) :
                        (bs_globals.encoding_strategy == kEncodingStrategyMidQuarterZero) ? bs_decode_byte_to_double_mqz(byte_buf[col_idx]) :
                        bs_decode_byte_to_double_custom(byte_buf[col_idx], bs_globals.encoding_cutoff_zero_min, bs_globals.encoding_cutoff_zero_max);
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
    double md_version = 0.0f;
    size_t md_block_row_size = 0;
    size_t md_num_offsets = 0;
    off_t* md_offsets = NULL;
    
    /* metadata version */
    md_delim_pos_ptr = strchr(ms, (int) kCompressionMetadataDelimiter);
    md_delim_length = md_delim_pos_ptr - ms;    
    memcpy(md_token, ms, md_delim_length);
    md_token[md_delim_length] = '\0';
    if (sscanf(md_token, "%lf", &md_version) == EOF) {
	fprintf(stderr, "Error: Could not parse metadata version key!\n");
	exit(EXIT_FAILURE);
    }

    if (md_version == kCompressionMetadataVersion) { /* v1.0 */
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
	metadata = malloc(sizeof(metadata_t));
	if (!metadata) {
	    fprintf(stderr, "Error: Could not allocate space for offset struct!\n");
	    exit(EXIT_FAILURE);
	}
	metadata->offsets = md_offsets;
	metadata->count = md_num_offsets;
	metadata->block_row_size = md_block_row_size;
	metadata->version = md_version;
    }
    else {
	fprintf(stderr, "Error: Could not parse metadata due to unknown version key!\n");
	exit(EXIT_FAILURE);
    }

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
                "%3.6f\t%" PRIu64 "\t%3.12f\n",
                bs_decode_byte_to_double((byte_t) bin_idx),
                t[bin_idx],
                (double) t[bin_idx] / n);
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
