#ifndef BYTE_STORE_H_
#define BYTE_STORE_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif /* getline() support */
    
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <float.h>
#include <errno.h>
#include <bzlib.h>
#include "mt19937.h"

#define BUF_MAX_LEN 4096
#define CHR_MAX_LEN 256
#define COORD_MAX_LEN 20
#define ID_MAX_LEN 524288
#define FN_MAX_LEN 1024
#define QUERY_MAX_LEN 524288
#define ENTRY_MAX_LEN 20
#define OFFSET_MAX_LEN 20
#define MD_OFFSET_MAX_LEN 20
#define BLOCK_STR_MAX_LEN 13
#define MULT_IDX_MAX_NUM 4096

#define swap(x,y) do                                                    \
        { unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
            memcpy(swap_temp,&y,sizeof(x));                             \
            memcpy(&y,&x,       sizeof(x));                             \
            memcpy(&x,swap_temp,sizeof(x));                             \
        } while(0)

    typedef unsigned char byte_t;
    typedef float score_t;
    typedef float rank_t;

    extern const byte_t kNANEncodedByte;
    const byte_t kNANEncodedByte = 0xca;

    extern const char* kTestVectorA;
    extern const char* kTestVectorB;
    extern const char* kTestVectorC;
    extern const char* kTestVectorX;
    extern const char* kTestVectorY;
    const char* kTestVectorA = "20,8,10,31,50,51,15,41,28,28,11,25,23,21,13,19,14,16,36,38,24,15,35,24,61,31,18,49,19,14,27,19,12,18,15,116,21,28,22,16,11,22,29,31,18,17,9,17,8,14,35,43,10,24,13,19,17,119,33,23,40,10,19,60,12,18,22,7,5,27,40,12,7,21,7,18,6,34,26,6,16,11";
    const char* kTestVectorB = "17,10,9,42,57,56,5,49,24,27,14,22,25,16,21,23,22,10,20,29,14,29,34,14,70,33,5,35,11,13,13,20,15,15,55,19,32,26,10,11,12,16,25,22,31,7,8,2,10,9,14,50,9,38,20,21,14,27,31,14,24,15,14,18,16,26,6,3,8,10,58,16,8,19,10,53,4,76,17,14,29,27";
    const char* kTestVectorC = "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
    const char* kTestVectorX = "1,2,3,20,4";
    const char* kTestVectorY = "2,3,1,15,9";
    const char* kTestVectorZ = "2,NAN,1,15,9";
    
    extern const score_t kPearsonRTestABCorrelationUnencoded;
    extern const score_t kPearsonRTestABCorrelationEncoded;
    extern const byte_t kPearsonRTestABCorrelationEncodedByte;
    extern const score_t kPearsonRTestACCorrelationUnencoded;
    extern const score_t kPearsonRTestACCorrelationEncoded;
    extern const byte_t kPearsonRTestACCorrelationEncodedByte;    
    const score_t kPearsonRTestABCorrelationUnencoded = 0.4134264f;
    const score_t kPearsonRTestABCorrelationEncoded = 0.41f;
    const byte_t kPearsonRTestABCorrelationEncodedByte = 0x8e;
    const score_t kPearsonRTestACCorrelationUnencoded = NAN;
    const score_t kPearsonRTestACCorrelationEncoded = NAN;
    const byte_t kPearsonRTestACCorrelationEncodedByte = 0xca;

    /*
        Sanity checks courtesy of R, e.g.:

        > x <- c(1,2,3,20,4)
        > y <- c(2,3,1,5,4)
        > cor(x, y, use="all", method="spearman")
        [1] 0.7

        etc.
    */
    extern const score_t kSpearmanRhoTestXYCorrelationUnencoded;
    extern const score_t kSpearmanRhoTestXYCorrelationEncoded;
    extern const score_t kSpearmanRhoTestXZCorrelationUnencoded;
    extern const score_t kSpearmanRhoTestXZCorrelationEncoded;
    extern const byte_t kSpearmanRhoTestXZCorrelationEncodedByte;  
    extern const score_t kSpearmanRhoTestABCorrelationUnencoded;
    extern const score_t kSpearmanRhoTestABCorrelationEncoded;
    extern const score_t kSpearmanRhoTestACCorrelationUnencoded;
    extern const score_t kSpearmanRhoTestACCorrelationEncoded;
    extern const byte_t kSpearmanRhoTestACCorrelationEncodedByte;  
    const score_t kSpearmanRhoTestXYCorrelationUnencoded = 0.7f;
    const score_t kSpearmanRhoTestXYCorrelationEncoded = 0.70f;
    const byte_t kSpearmanRhoTestXYCorrelationEncodedByte = 0xab;
    const score_t kSpearmanRhoTestABCorrelationUnencoded = 0.6152396f;
    const score_t kSpearmanRhoTestABCorrelationEncoded = 0.61f;
    const byte_t kSpearmanRhoTestABCorrelationEncodedByte = 0xa2;
    const score_t kSpearmanRhoTestXZCorrelationUnencoded = NAN;
    const score_t kSpearmanRhoTestXZCorrelationEncoded = NAN;
    const byte_t kSpearmanRhoTestXZCorrelationEncodedByte = 0xca;
    const score_t kSpearmanRhoTestACCorrelationUnencoded = NAN;
    const score_t kSpearmanRhoTestACCorrelationEncoded = NAN;
    const byte_t kSpearmanRhoTestACCorrelationEncodedByte = 0xca;

    typedef int boolean_t;
    extern const boolean_t kTrue;
    extern const boolean_t kFalse;
    const boolean_t kTrue = 1;
    const boolean_t kFalse = 0;
    
    /* encoding to two decimals of precision specifies needed accuracy */
    extern const score_t kEpsilon;
    extern const score_t kEpsilonLessStringent;
    const score_t kEpsilon = 0.0000001f;
    const score_t kEpsilonLessStringent = 0.001f;
    
    extern const score_t kSelfCorrelationScore;
    const score_t kSelfCorrelationScore = +1.0f;
    
    extern const score_t kNoCorrelationScore;
    const score_t kNoCorrelationScore = +0.0f;
    
    extern const int kQueryIndexDelim;
    const int kQueryIndexDelim = (int) '-';

    extern const int kQueryRangeBetweenDelim;
    const int kQueryRangeBetweenDelim = (int) '-';

    extern const int kQueryRangeWithinDelim;
    const int kQueryRangeWithinDelim = (int) ':';
    
    extern const int kQueryMultipleIndexDelim;
    const int kQueryMultipleIndexDelim = (int) ',';
    
    extern const int kSignalDelim;
    const int kSignalDelim = (int) ',';
    
    typedef struct signal {
        uint32_t n;
        score_t* data;
        score_t mean;
        score_t sd;
        rank_t* ranks;
        score_t mean_ranks;
        score_t sd_ranks;
    } signal_t;
    
    typedef struct element {
        char* chr;
        uint64_t start;
        uint64_t stop;
        char* id;
        signal_t* signal;
    } element_t;
    
    typedef struct lookup {
        uint64_t capacity;
        uint32_t nelems;
        element_t** elems;
    } lookup_t;
    
    typedef struct store_attr {
        uint32_t nelems;
        uint64_t nbytes;
        char* fn;
    } store_attr_t;
    
    typedef struct sut_store {
        store_attr_t* attr;
    } sut_store_t;
    
    typedef struct sqr_store {
        store_attr_t* attr;
    } sqr_store_t;
    
    typedef struct store_buf_node {
        byte_t data;
        struct store_buf_node* next;
    } store_buf_node_t;
    
    typedef struct store_buf_row_node {
        struct store_buf_node* head;
        struct store_buf_node* tail;
    } store_buf_row_node_t;
    
    typedef enum store_type {
        kStorePearsonRSUT = 0,
        kStorePearsonRSquareMatrix,
        kStorePearsonRSquareMatrixSplit,
        kStorePearsonRSquareMatrixSplitSingleChunk,
        kStorePearsonRSquareMatrixSplitSingleChunkMetadata,
        kStorePearsonRSquareMatrixBzip2,
        kStorePearsonRSquareMatrixBzip2Split,
        kStoreSpearmanRhoSquareMatrix,
        kStoreSpearmanRhoSquareMatrixSplit,
        kStoreSpearmanRhoSquareMatrixSplitSingleChunk,
        kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadata,
        kStoreSpearmanRhoSquareMatrixBzip2,
        kStoreSpearmanRhoSquareMatrixBzip2Split,
        kStoreRandomSUT,
        kStoreRandomSquareMatrix,
        kStoreRandomBufferedSquareMatrix,
        kStoreUndefined
    } store_type_t;

    typedef enum score_variety {
        kScoreVarietyPearsonR = 0, 
        kScoreVarietySpearmanRho,
        kScoreVarietyUndefined
    } score_variety_t;
    
    extern const char* kStorePearsonRSUTStr;
    extern const char* kStorePearsonRSquareMatrixStr;
    extern const char* kStorePearsonRSquareMatrixSplitStr;
    extern const char* kStorePearsonRSquareMatrixSplitSingleChunkStr;
    extern const char* kStorePearsonRSquareMatrixSplitSingleChunkMetadataStr;
    extern const char* kStorePearsonRSquareMatrixBzip2Str;
    extern const char* kStorePearsonRSquareMatrixBzip2SplitStr;
    extern const char* kStoreSpearmanRhoSquareMatrixStr;
    extern const char* kStoreSpearmanRhoSquareMatrixSplitStr;
    extern const char* kStoreSpearmanRhoSquareMatrixSplitSingleChunkStr;
    extern const char* kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadataStr;
    extern const char* kStoreSpearmanRhoSquareMatrixBzip2Str;
    extern const char* kStoreSpearmanRhoSquareMatrixBzip2SplitStr;
    extern const char* kStoreRandomSUTStr;
    extern const char* kStoreRandomSquareMatrixStr;
    extern const char* kStoreRandomBufferedSquareMatrixStr;
    const char* kStorePearsonRSUTStr = "pearson-r-sut";
    const char* kStorePearsonRSquareMatrixStr = "pearson-r-sqr";
    const char* kStorePearsonRSquareMatrixSplitStr = "pearson-r-sqr-split";
    const char* kStorePearsonRSquareMatrixSplitSingleChunkStr = "pearson-r-sqr-split-single-chunk";
    const char* kStorePearsonRSquareMatrixSplitSingleChunkMetadataStr = "pearson-r-sqr-split-single-chunk-metadata";
    const char* kStorePearsonRSquareMatrixBzip2Str = "pearson-r-sqr-bzip2";
    const char* kStorePearsonRSquareMatrixBzip2SplitStr = "pearson-r-sqr-bzip2-split";
    const char* kStoreSpearmanRhoSquareMatrixStr = "spearman-rho-sqr";
    const char* kStoreSpearmanRhoSquareMatrixSplitStr = "spearman-rho-sqr-split";
    const char* kStoreSpearmanRhoSquareMatrixSplitSingleChunkStr = "spearman-rho-sqr-split-single-chunk";
    const char* kStoreSpearmanRhoSquareMatrixSplitSingleChunkMetadataStr = "spearman-rho-sqr-split-single-chunk-metadata";
    const char* kStoreSpearmanRhoSquareMatrixBzip2Str = "spearman-rho-sqr-bzip2";
    const char* kStoreSpearmanRhoSquareMatrixBzip2SplitStr = "spearman-rho-sqr-bzip2-split";
    const char* kStoreRandomSUTStr = "random-sut";
    const char* kStoreRandomSquareMatrixStr = "random-sqr";
    const char* kStoreRandomBufferedSquareMatrixStr = "random-buffered-sqr";
    
    typedef enum encoding_strategy {
        kEncodingStrategyFull = 0,
        kEncodingStrategyMidQuarterZero,
        kEncodingStrategyCustom,
        kEncodingStrategyUndefined
    } encoding_strategy_t;
    
    extern const char* kEncodingStrategyFullStr;
    extern const char* kEncodingStrategyMidQuarterZeroStr;
    extern const char* kEncodingStrategyCustomStr;
    extern const score_t kEncodingStrategyDefaultCutoff;
    const char* kEncodingStrategyFullStr = "full";
    const char* kEncodingStrategyMidQuarterZeroStr = "mid-quarter-zero";
    const char* kEncodingStrategyCustomStr = "custom";
    const score_t kEncodingStrategyDefaultCutoff = -DBL_MAX;
    
    extern const uint32_t kRowChunkDefaultSize;
    extern const uint32_t kRowChunkDefaultOffset;
    const uint32_t kRowChunkDefaultSize = UINT32_MAX;
    const uint32_t kRowChunkDefaultOffset = UINT32_MAX;
    
    extern const char* kRawMetadataSplitFn;
    extern const char* kRawMetadataSplitDirSuffix;
    const char* kRawMetadataSplitFn = "blocks.md";    
    const char* kRawMetadataSplitDirSuffix = "blocks";
    
    extern const uint32_t kCompressionRowChunkMaximumSize;
    extern const uint32_t kCompressionBzip2BlockSize100k;
    extern const uint32_t kCompressionBzip2BlockSizeFactor;
    extern const uint32_t kCompressionBzip2Verbosity;
    extern const uint32_t kCompressionBzip2WorkFactor;
    extern const uint32_t kCompressionBzip2AbandonPolicy;
    extern const uint32_t kCompressionBzip2SmallPolicy;
    const uint32_t kCompressionRowChunkMaximumSize = 512;
    const uint32_t kCompressionBzip2BlockSize100k = 9;
    const uint32_t kCompressionBzip2BlockSizeFactor = 100000;
    const uint32_t kCompressionBzip2Verbosity = 0;
    const uint32_t kCompressionBzip2WorkFactor = 30;
    const uint32_t kCompressionBzip2AbandonPolicy = 0;
    const uint32_t kCompressionBzip2SmallPolicy = 0;

    extern const char kCompressionMetadataDelimiter;
    extern const float kCompressionMetadataVersion;
    extern const float kCompressionMetadataVersion1p0;
    extern const float kCompressionMetadataVersion1p1;
    extern const char* kCompressionMetadataSplitFn;
    extern const char* kCompressionMetadataSplitDirSuffix;
    const char kCompressionMetadataDelimiter = '|';
    const float kCompressionMetadataVersion1p0 = 1.0f;
    const float kCompressionMetadataVersion1p1 = 1.1f;
    const float kCompressionMetadataVersion = kCompressionMetadataVersion1p1;
    const char* kCompressionMetadataSplitFn = "blocks.md";
    const char* kCompressionMetadataSplitDirSuffix = "blocks";

    extern const uint32_t kPermutationTestDefaultCount;
    extern const score_t kPermutationTestDefaultPrecision;
    extern const score_t kPermutationTestDefaultAlpha;
    extern const uint32_t kPermutationTestDefaultSignificanceLevel;
    const uint32_t kPermutationTestDefaultCount = 0;
    const score_t kPermutationTestDefaultPrecision = 0.01f;
    const score_t kPermutationTestDefaultAlpha = 0.01f;
    const uint32_t kPermutationTestDefaultSignificanceLevel = 3;
    
    typedef struct metadata {
        score_variety_t score_variety;
        off_t* offsets;
        size_t count;
        size_t block_row_size;
        double version;
    } metadata_t;

    extern const uint32_t kQueryIndexDefaultStart;
    extern const uint32_t kQueryIndexDefaultEnd;
    extern const char* kQueryRangeDefaultChromosome;
    extern const uint64_t kQueryRangeDefaultStart;
    extern const uint64_t kQueryRangeDefaultEnd;
    
    const uint32_t kQueryIndexDefaultStart = 0;
    const uint32_t kQueryIndexDefaultEnd = 0;
    const char* kQueryRangeDefaultChromosome = "chrXYZ";    
    const uint64_t kQueryRangeDefaultStart = 0;
    const uint64_t kQueryRangeDefaultEnd = 0;
    
    typedef struct bed {
        char* chromosome;
        uint64_t start;
        uint64_t end;
    } bed_t;

    typedef enum query_kind {
        kQueryKindIndex,
        kQueryKindMultipleIndices,
        kQueryKindMultipleIndicesFromFile,
        kQueryKindRange,
        kQueryKindUndefined
    } query_kind_t;

    extern const query_kind_t kQueryKindDefaultKind;

    const query_kind_t kQueryKindDefaultKind = kQueryKindUndefined;

    typedef enum score_filter {
        kScoreFilterNone = 0,
        kScoreFilterGtEq,
        kScoreFilterGt,
        kScoreFilterEq,
        kScoreFilterLtEq,
        kScoreFilterLt,
        kScoreFilterRangedWithinExclusive,
        kScoreFilterRangedWithinInclusive,
        kScoreFilterRangedOutsideExclusive,
        kScoreFilterRangedOutsideInclusive,
        kScoreFilterUndefined
    } score_filter_t;

    extern const score_filter_t kScoreDefaultFilter;

    const score_filter_t kScoreDefaultFilter = kScoreFilterNone;

    static struct bs_globals_t {
        boolean_t store_create_flag;
        boolean_t store_query_flag;
        boolean_t store_frequency_flag;
        query_kind_t store_query_kind;
        char store_query_str[QUERY_MAX_LEN];
        uint32_t store_query_idx_start;
        uint32_t store_query_idx_end;
        int32_t* store_query_indices;
        uint32_t store_query_indices_num;
        bed_t* store_query_range_start;
        bed_t* store_query_range_end;
        score_filter_t store_filter;
        score_t score_filter_cutoff;
        score_t score_filter_cutoff_lower_bound;
        score_t score_filter_cutoff_upper_bound; /* for threshold ranges */
        boolean_t score_filter_range_set;
        boolean_t rng_seed_flag;
        uint32_t rng_seed_value;
        char lookup_fn[FN_MAX_LEN];
        char store_fn[FN_MAX_LEN];
        char store_type_str[BUF_MAX_LEN];
        store_type_t store_type;
        encoding_strategy_t encoding_strategy;
        char encoding_strategy_str[BUF_MAX_LEN];
        score_t encoding_cutoff_zero_min;
        score_t encoding_cutoff_zero_max;
        uint32_t store_row_chunk_size;
        uint32_t store_row_chunk_offset;
        boolean_t store_chunk_size_specified_flag;
        boolean_t store_single_chunk_flag;
        boolean_t store_compression_flag;
        boolean_t lookup_frequency_flag;
        boolean_t permutation_test_flag;
        uint32_t permutation_count;
        score_t permutation_precision;
        score_t permutation_alpha;
        uint32_t permutation_significance_level;
        boolean_t zero_sd_warning_issued;
        score_t* score_ptr;
    } bs_globals;

    static struct option bs_client_long_options[] = {
        { "store-type",                                 required_argument, NULL, 't' },
        { "store-create",                               no_argument,       NULL, 'c' },
        { "store-query",                                no_argument,       NULL, 'q' },
        { "store-frequency",                            no_argument,       NULL, 'f' },
        { "store-row-chunk-size",                       required_argument, NULL, 'r' },
        { "store-row-chunk-offset",                     required_argument, NULL, 'k' },
        { "score-filter-gteq",                          required_argument, NULL, '2' },
        { "score-filter-gt",                            required_argument, NULL, '3' },
        { "score-filter-eq",                            required_argument, NULL, '4' },
        { "score-filter-lteq",                          required_argument, NULL, '5' },
        { "score-filter-lt",                            required_argument, NULL, '6' },
        { "score-filter-ranged-within-exclusive",       required_argument, NULL, '7' },
        { "score-filter-ranged-within-inclusive",       required_argument, NULL, '8' },
        { "score-filter-ranged-outside-exclusive",      required_argument, NULL, '9' },
        { "score-filter-ranged-outside-inclusive",      required_argument, NULL, '0' },
        { "index-query",                                required_argument, NULL, 'i' },
        { "multiple-index-query",                       required_argument, NULL, 'w' },
        { "multiple-index-query-from-file",             required_argument, NULL, 'z' },
        { "range-query",                                required_argument, NULL, 'g' },
        { "lookup",                                     required_argument, NULL, 'l' },
        { "store",                                      required_argument, NULL, 's' },
        { "encoding-strategy",                          required_argument, NULL, 'e' },
        { "encoding-cutoff-zero-min",                   required_argument, NULL, 'n' },
        { "encoding-cutoff-zero-max",                   required_argument, NULL, 'x' },
        { "lookup-frequency",                           no_argument,       NULL, 'u' },
        { "permutation-test",                           no_argument,       NULL, 'm' },
        { "permutation-count",                          required_argument, NULL, 'o' },
        { "permutation-precision",                      required_argument, NULL, 'p' },
        { "permutation-alpha",                          required_argument, NULL, 'a' },
        { "permutation-significance-level",             required_argument, NULL, 'v' },
        { "rng-seed",                                   required_argument, NULL, 'd' },
        { "test-pearson-r",                             no_argument,       NULL, '1' },
        { "test-spearman-rho",                          no_argument,       NULL, '2' },
        { "help",                                       no_argument,       NULL, 'h' },
        { NULL,                                         no_argument,       NULL,  0  }
    }; 
    
    static const char* bs_client_opt_string = "t:cqfr:k:2:3:4:5:6:7:8:9:0:i:w:z:g:l:s:e:n:x:umo:p:a:v:d:SPh?";

    static const char* bs_name = "byte-store";
    
    /**
     * @brief      bs_encode_byte_to_score_table[]
     *
     * @details    Converts encoded byte to assigned bin interval start, 
     *             e.g., 0x64 maps to -0.00 or (-0.01, -0.00], while 
     *             0x65 maps to +0.00 or [+0.00, +0.01). 
     *
     *             Note the two bins for -0.00 and +0.00. This allows 
     *             mapping of scores between -0.01, 0.00, and 0.01 to 
     *             be mapped to two seperate bytes.
     */
    
    static const score_t bs_encode_byte_to_score_table[256] = 
        {-1.00, 
         -0.99, -0.98, -0.97, -0.96, -0.95, -0.94, -0.93, -0.92, -0.91, -0.90,
         -0.89, -0.88, -0.87, -0.86, -0.85, -0.84, -0.83, -0.82, -0.81, -0.80,
         -0.79, -0.78, -0.77, -0.76, -0.75, -0.74, -0.73, -0.72, -0.71, -0.70, 
         -0.69, -0.68, -0.67, -0.66, -0.65, -0.64, -0.63, -0.62, -0.61, -0.60, 
         -0.59, -0.58, -0.57, -0.56, -0.55, -0.54, -0.53, -0.52, -0.51, -0.50, 
         -0.49, -0.48, -0.47, -0.46, -0.45, -0.44, -0.43, -0.42, -0.41, -0.40, 
         -0.39, -0.38, -0.37, -0.36, -0.35, -0.34, -0.33, -0.32, -0.31, -0.30, 
         -0.29, -0.28, -0.27, -0.26, -0.25, -0.24, -0.23, -0.22, -0.21, -0.20, 
         -0.19, -0.18, -0.17, -0.16, -0.15, -0.14, -0.13, -0.12, -0.11, -0.10, 
         -0.09, -0.08, -0.07, -0.06, -0.05, -0.04, -0.03, -0.02, -0.01, -0.00,
         +0.00, +0.01, +0.02, +0.03, +0.04, +0.05, +0.06, +0.07, +0.08, +0.09,
         +0.10, +0.11, +0.12, +0.13, +0.14, +0.15, +0.16, +0.17, +0.18, +0.19,
         +0.20, +0.21, +0.22, +0.23, +0.24, +0.25, +0.26, +0.27, +0.28, +0.29,
         +0.30, +0.31, +0.32, +0.33, +0.34, +0.35, +0.36, +0.37, +0.38, +0.39,
         +0.40, +0.41, +0.42, +0.43, +0.44, +0.45, +0.46, +0.47, +0.48, +0.49,
         +0.50, +0.51, +0.52, +0.53, +0.54, +0.55, +0.56, +0.57, +0.58, +0.59,
         +0.60, +0.61, +0.62, +0.63, +0.64, +0.65, +0.66, +0.67, +0.68, +0.69,
         +0.70, +0.71, +0.72, +0.73, +0.74, +0.75, +0.76, +0.77, +0.78, +0.79,
         +0.80, +0.81, +0.82, +0.83, +0.84, +0.85, +0.86, +0.87, +0.88, +0.89,
         +0.90, +0.91, +0.92, +0.93, +0.94, +0.95, +0.96, +0.97, +0.98, +0.99,
         +1.00, 
           NAN, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
         +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
         +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
         +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
         +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
         +0.00, +0.00, +0.00, +0.00};
    
    /**
     * @brief      bs_encode_unsigned_char_to_score_mqz_table[]
     *
     * @details    Converts encoded byte to assigned bin interval start, 
     *             but maps (-0.25, +0.25) to +0.00
     */
    
    static const score_t bs_encode_byte_to_score_mqz_table[256] = 
        {-1.00, 
         -0.99, -0.98, -0.97, -0.96, -0.95, -0.94, -0.93, -0.92, -0.91, -0.90,
         -0.89, -0.88, -0.87, -0.86, -0.85, -0.84, -0.83, -0.82, -0.81, -0.80,
         -0.79, -0.78, -0.77, -0.76, -0.75, -0.74, -0.73, -0.72, -0.71, -0.70, 
         -0.69, -0.68, -0.67, -0.66, -0.65, -0.64, -0.63, -0.62, -0.61, -0.60, 
         -0.59, -0.58, -0.57, -0.56, -0.55, -0.54, -0.53, -0.52, -0.51, -0.50, 
         -0.49, -0.48, -0.47, -0.46, -0.45, -0.44, -0.43, -0.42, -0.41, -0.40, 
         -0.39, -0.38, -0.37, -0.36, -0.35, -0.34, -0.33, -0.32, -0.31, -0.30, 
         -0.29, -0.28, -0.27, -0.26, -0.25, +0.00, +0.00, +0.00, +0.00, +0.00, 
         +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
         +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00,
         +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00,
         +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00,
         +0.00, +0.00, +0.00, +0.00, +0.00, +0.25, +0.26, +0.27, +0.28, +0.29,
         +0.30, +0.31, +0.32, +0.33, +0.34, +0.35, +0.36, +0.37, +0.38, +0.39,
         +0.40, +0.41, +0.42, +0.43, +0.44, +0.45, +0.46, +0.47, +0.48, +0.49,
         +0.50, +0.51, +0.52, +0.53, +0.54, +0.55, +0.56, +0.57, +0.58, +0.59,
         +0.60, +0.61, +0.62, +0.63, +0.64, +0.65, +0.66, +0.67, +0.68, +0.69,
         +0.70, +0.71, +0.72, +0.73, +0.74, +0.75, +0.76, +0.77, +0.78, +0.79,
         +0.80, +0.81, +0.82, +0.83, +0.84, +0.85, +0.86, +0.87, +0.88, +0.89,
         +0.90, +0.91, +0.92, +0.93, +0.94, +0.95, +0.96, +0.97, +0.98, +0.99,
         +1.00, 
           NAN, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
         +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
         +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
         +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
         +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
         +0.00, +0.00, +0.00, +0.00};
    
    inline score_t               bs_truncate_score_to_precision(score_t d, int prec);
    inline byte_t                bs_encode_score_to_byte(score_t d);
    inline byte_t                bs_encode_score_to_byte_mqz(score_t d);
    byte_t                       bs_encode_score_to_byte_custom(score_t d, score_t min, score_t max);
    inline boolean_t             bs_signbit(score_t d);
    static inline score_t        bs_decode_byte_to_score(byte_t uc);
    static inline score_t        bs_decode_byte_to_score_mqz(byte_t uc);
    static inline score_t        bs_decode_byte_to_score_custom(byte_t uc, score_t min, score_t max);
    boolean_t                    bs_parse_query_range_str(lookup_t* l, char* rs, uint32_t* start, uint32_t* end);
    boolean_t                    bs_parse_query_index_str(lookup_t* l);
    boolean_t                    bs_parse_query_multiple_index_str(lookup_t* l, char* qs);
    int32_t                      bs_parse_query_multiple_index_str_comparator(const void* a, const void* b); 
    void                         bs_parse_query_str_to_indices(char* qs, uint32_t* start, uint32_t* stop);
    bed_t*                       bs_init_bed(const char* chr, uint64_t start, uint64_t end);
    void                         bs_delete_bed(bed_t** b);
    lookup_t*                    bs_init_lookup(char* fn, boolean_t pi, boolean_t ir);
    void                         bs_permute_lookup(lookup_t *l, FILE* os);
    void                         bs_shuffle_signal_data(score_t* d, size_t n);
    void                         bs_print_lookup(lookup_t* l, FILE* os);
    void                         bs_print_lookup_frequency(lookup_t* l, FILE* os);
    void                         bs_increment_lookup_frequency(uint64_t* t, lookup_t* l);
    void                         bs_delete_lookup(lookup_t** l);
    signal_t*                    bs_init_signal(char *cds, boolean_t ir);
    static int                   bs_rank_comparator(const void *a, const void *b);
    void                         bs_print_signal(signal_t* s, FILE* os);
    score_t                      bs_mean_signal(score_t* d, uint32_t len);
    score_t                      bs_mean_ranks(rank_t* d, uint32_t len);
    score_t                      bs_sample_sd_signal(score_t* d, uint32_t len, score_t m);
    score_t                      bs_sample_sd_ranks(rank_t* d, uint32_t len, score_t m);
    score_t                      bs_pearson_r_signal(signal_t* a, signal_t* b);
    score_t                      bs_spearman_rho_signal_v1(signal_t* a, signal_t* b);
    score_t                      bs_spearman_rho_signal_v2(signal_t* a, signal_t* b);
    void                         bs_delete_signal(signal_t** s);
    element_t*                   bs_init_element(char* chr, uint64_t start, uint64_t stop, char* id, boolean_t pi, boolean_t ir);
    void                         bs_delete_element(element_t** e);
    void                         bs_push_elem_to_lookup(element_t* e, lookup_t** l, boolean_t pi, boolean_t ir);
    void                         bs_test_pearsons_r();
    void                         bs_test_spearman_rho();
    void                         bs_test_score_encoding();
    void                         bs_init_globals();
    void                         bs_delete_globals();
    void                         bs_init_command_line_options(int argc, char** argv);
    void                         bs_print_usage(FILE* os);
    inline boolean_t             bs_path_exists(const char* p);
    inline ssize_t               bs_file_size(const char* fn);
    inline void                  bs_print_pair(FILE* os, char* chr_a, uint64_t start_a, uint64_t stop_a, char* chr_b, uint64_t start_b, uint64_t stop_b, score_t score);
    sut_store_t*                 bs_init_sut_store(uint32_t n);
    void                         bs_populate_sut_store_with_random_scores(sut_store_t* s);
    void                         bs_populate_sut_store_with_pearsonr_scores(sut_store_t* s, lookup_t* l);
    off_t                        bs_sut_byte_offset_for_element_ij(uint32_t n, uint32_t i, uint32_t j);
    void                         bs_print_sut_store_to_bed7(lookup_t* l, sut_store_t* s, FILE* os);
    void                         bs_print_sut_filtered_store_to_bed7(lookup_t* l, sut_store_t* s, FILE* os, score_t fc, score_filter_t fo);
    void                         bs_print_sut_frequency_to_txt(lookup_t* l, sut_store_t* s, FILE* os);
    void                         bs_delete_sut_store(sut_store_t** s);
    sqr_store_t*                 bs_init_sqr_store(uint32_t n);
    void                         bs_populate_sqr_store_with_random_scores(sqr_store_t* s);
    void                         bs_populate_sqr_store_with_buffered_random_scores(sqr_store_t* s);
    void                         bs_populate_sqr_store(sqr_store_t* s, lookup_t* l, score_t (*sf)(signal_t*, signal_t*));
    char*                        bs_init_sqr_split_store_fn_str(char* d, uint32_t i);
    char*                        bs_init_sqr_split_store_dir_str(char* p);
    char*                        bs_init_sqr_split_store_metadata_fn_str(char* d);
    void                         bs_populate_sqr_split_store(sqr_store_t* s, lookup_t* l, uint32_t n, score_t (*sf)(signal_t*, signal_t*), score_variety_t sv);
    void                         bs_populate_sqr_split_store_chunk(sqr_store_t* s, lookup_t* l, uint32_t n, uint32_t o, score_t (*sf)(signal_t*, signal_t*));
    void                         bs_populate_sqr_split_store_chunk_metadata(sqr_store_t* s, lookup_t* l, uint32_t n, score_variety_t v);
    void                         bs_populate_sqr_bzip2_store(sqr_store_t* s, lookup_t* l, uint32_t n, score_t (*sf)(signal_t*, signal_t*), score_variety_t sv);
    void                         bs_populate_sqr_bzip2_split_store(sqr_store_t* s, lookup_t* l, uint32_t n, score_t (*sf)(signal_t*, signal_t*), score_variety_t sv);
    char*                        bs_init_sqr_bzip2_split_store_dir_str(char* p);
    char*                        bs_init_sqr_bzip2_split_store_fn_str(char* p, uint32_t i);
    char*                        bs_init_sqr_bzip2_split_store_metadata_fn_str(char* d);
    off_t                        bs_sqr_byte_offset_for_element_ij(uint32_t n, uint32_t i, uint32_t j);
    void                         bs_print_sqr_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os);
    void                         bs_print_sqr_filtered_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os, score_t fc, score_t flb, score_t fub, score_filter_t fo);
    void                         bs_print_sqr_split_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os);
    void                         bs_print_sqr_filtered_split_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os, score_t fc, score_t flb, score_t fub, score_filter_t fo);
    void                         bs_print_sqr_split_store_separate_rows_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os, int32_t* r, uint32_t rn);
    void                         bs_print_sqr_split_store_separate_rows_to_bed7_file(lookup_t* l, sqr_store_t* s, char* qf, FILE* os);
    void                         bs_print_sqr_filtered_split_store_separate_rows_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os, int32_t* r, uint32_t rn, score_t fc, score_t flb, score_t fub, score_filter_t fo);
    void                         bs_print_sqr_filtered_split_store_separate_rows_to_bed7_file(lookup_t* l, sqr_store_t* s, char* qf, FILE* os, score_t fc, score_t flb, score_t fub, score_filter_t fo);
    void                         bs_print_sqr_bzip2_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os);
    void                         bs_print_sqr_filtered_bzip2_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os, score_t fc, score_t flb, score_t fub, score_filter_t fo);
    void                         bs_print_sqr_bzip2_split_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os);
    void                         bs_print_sqr_filtered_bzip2_split_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os, score_t fc, score_t flb, score_t fub, score_filter_t fo);
    char*                        bs_init_metadata_str(off_t* o, uint32_t n, uint32_t s, score_variety_t v);
    metadata_t*                  bs_parse_metadata_str(char* ms);
    void                         bs_delete_metadata(metadata_t** m);
    void                         bs_print_sqr_store_frequency_to_txt(lookup_t* l, sqr_store_t* s, FILE* os);
    void                         bs_print_sqr_split_store_frequency_to_txt(lookup_t* l, sqr_store_t* s, FILE* os);
    void                         bs_print_sqr_bzip2_store_frequency_to_txt(lookup_t* l, sqr_store_t* s, FILE* os);
    void                         bs_print_sqr_bzip2_split_store_frequency_to_txt(lookup_t* l, sqr_store_t* s, FILE* os);
    void                         bs_print_frequency_buffer(uint64_t* t, uint64_t n, FILE* os);
    void                         bs_delete_sqr_store(sqr_store_t** s);
    store_buf_node_t*            bs_init_store_buf_node(byte_t uc);
    void                         bs_insert_store_buf_node(store_buf_node_t* n, store_buf_node_t* i);
    
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif // BYTE_STORE_H_
