#ifndef BYTE_STORE_H_
#define BYTE_STORE_H_

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
#define FN_MAX_LEN 1024
#define QUERY_MAX_LEN 40
#define ENTRY_MAX_LEN 20
#define OFFSET_MAX_LEN 20
#define MD_OFFSET_MAX_LEN 20
#define BLOCK_STR_MAX_LEN 13

#define swap(x,y) do \
{ unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
    memcpy(swap_temp,&y,sizeof(x)); \
    memcpy(&y,&x,       sizeof(x)); \
    memcpy(&x,swap_temp,sizeof(x)); \
} while(0)

extern const char* kPearsonRTestVectorA;
extern const char* kPearsonRTestVectorB;
extern const double kPearsonRTestCorrelationUnencoded;
extern const double kPearsonRTestCorrelationEncoded;
extern const unsigned char kPearsonRTestCorrelationEncodedByte;
const char* kPearsonRTestVectorA = "20,8,10,31,50,51,15,41,28,28,11,25,23,21,13,19,14,16,36,38,24,15,35,24,61,31,18,49,19,14,27,19,12,18,15,116,21,28,22,16,11,22,29,31,18,17,9,17,8,14,35,43,10,24,13,19,17,119,33,23,40,10,19,60,12,18,22,7,5,27,40,12,7,21,7,18,6,34,26,6,16,11";
const char* kPearsonRTestVectorB = "17,10,9,42,57,56,5,49,24,27,14,22,25,16,21,23,22,10,20,29,14,29,34,14,70,33,5,35,11,13,13,20,15,15,55,19,32,26,10,11,12,16,25,22,31,7,8,2,10,9,14,50,9,38,20,21,14,27,31,14,24,15,14,18,16,26,6,3,8,10,58,16,8,19,10,53,4,76,17,14,29,27";
const double kPearsonRTestCorrelationUnencoded = 0.4134264f;
const double kPearsonRTestCorrelationEncoded = 0.41f;
const unsigned char kPearsonRTestCorrelationEncodedByte = 0x8e;

typedef int boolean;
extern const boolean kTrue;
extern const boolean kFalse;
const boolean kTrue = 1;
const boolean kFalse = 0;

extern const double kEpsilon;
const double kEpsilon = 0.0000001f;

extern const double kSelfCorrelationScore;
const double kSelfCorrelationScore = +1.0f;

extern const double kNoCorrelationScore;
const double kNoCorrelationScore = +0.0f;

extern const int kQueryDelim;
const int kQueryDelim = (int) '-';

extern const int kSignalDelim;
const int kSignalDelim = (int) ',';

typedef struct signal {
    uint32_t n;
    double* data;
    double mean;
    double sd;
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
    char *fn;
} store_attr_t;

typedef struct sut_store {
    store_attr_t* attr;
} sut_store_t;

typedef struct sqr_store {
    store_attr_t* attr;
} sqr_store_t;

typedef struct store_buf_node {
    unsigned char data;
    struct store_buf_node *next;
} store_buf_node_t;

typedef struct store_buf_row_node {
    struct store_buf_node *head;
    struct store_buf_node *tail;
} store_buf_row_node_t;

typedef enum store_type {
    kStorePearsonRSUT = 0,
    kStorePearsonRSquareMatrix,
    kStorePearsonRSquareMatrixBzip2,
    kStorePearsonRSquareMatrixBzip2Split,
    kStoreRandomSUT,
    kStoreRandomSquareMatrix,
    kStoreRandomBufferedSquareMatrix,
    kStoreUndefined
} store_type_t;

extern const char* kStorePearsonRSUTStr;
extern const char* kStorePearsonRSquareMatrixStr;
extern const char* kStorePearsonRSquareMatrixBzip2Str;
extern const char* kStorePearsonRSquareMatrixBzip2SplitStr;
extern const char* kStoreRandomSUTStr;
extern const char* kStoreRandomSquareMatrixStr;
extern const char* kStoreRandomBufferedSquareMatrixStr;
const char* kStorePearsonRSUTStr = "pearson-r-sut";
const char* kStorePearsonRSquareMatrixStr = "pearson-r-sqr";
const char* kStorePearsonRSquareMatrixBzip2Str = "pearson-r-sqr-bzip2";
const char* kStorePearsonRSquareMatrixBzip2SplitStr = "pearson-r-sqr-bzip2-split";
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
extern const double kEncodingStrategyDefaultCutoff;
const char* kEncodingStrategyFullStr = "full";
const char* kEncodingStrategyMidQuarterZeroStr = "mid-quarter-zero";
const char* kEncodingStrategyCustomStr = "custom";
const double kEncodingStrategyDefaultCutoff = -DBL_MAX;

extern const uint32_t kCompressionRowChunkDefaultSize;
extern const uint32_t kCompressionRowChunkMaximumSize;
extern const uint32_t kCompressionBzip2BlockSize100k;
extern const uint32_t kCompressionBzip2BlockSizeFactor;
extern const uint32_t kCompressionBzip2Verbosity;
extern const uint32_t kCompressionBzip2WorkFactor;
extern const uint32_t kCompressionBzip2AbandonPolicy;
extern const uint32_t kCompressionBzip2SmallPolicy;
extern const char kCompressionMetadataDelimiter;
extern const double kCompressionMetadataVersion;
extern const char* kCompressionMetadataSplitFn;
extern const char* kCompressionMetadataSplitDirSuffix;
const uint32_t kCompressionRowChunkDefaultSize = UINT32_MAX;
const uint32_t kCompressionRowChunkMaximumSize = 512;
const uint32_t kCompressionBzip2BlockSize100k = 9;
const uint32_t kCompressionBzip2BlockSizeFactor = 100000;
const uint32_t kCompressionBzip2Verbosity = 0;
const uint32_t kCompressionBzip2WorkFactor = 30;
const uint32_t kCompressionBzip2AbandonPolicy = 0;
const uint32_t kCompressionBzip2SmallPolicy = 0;
const char kCompressionMetadataDelimiter = '|';
const double kCompressionMetadataVersion = 1.0f;
const char* kCompressionMetadataSplitFn = "blocks.md";
const char* kCompressionMetadataSplitDirSuffix = "blocks";

typedef struct metadata {
    off_t* offsets;
    size_t count;
    size_t block_row_size;
    double version;
} metadata_t;

static struct bs_globals_t {
    boolean store_create_flag;
    boolean store_query_flag;
    boolean store_frequency_flag;
    char store_query_str[QUERY_MAX_LEN];
    uint32_t store_query_idx_start;
    uint32_t store_query_idx_end;
    boolean rng_seed_flag;
    uint32_t rng_seed_value;
    char lookup_fn[FN_MAX_LEN];
    char store_fn[FN_MAX_LEN];
    char store_type_str[BUF_MAX_LEN];
    store_type_t store_type;
    encoding_strategy_t encoding_strategy;
    char encoding_strategy_str[BUF_MAX_LEN];
    double encoding_cutoff_zero_min;
    double encoding_cutoff_zero_max;
    boolean store_compression_flag;
    uint32_t store_compression_row_chunk_size;
    boolean lookup_frequency_flag;
    boolean permutation_test_flag;
    int32_t permutation_count;
} bs_globals;

static struct option bs_client_long_options[] = {
    { "store-type",                       required_argument, NULL, 't' },
    { "store-create",                     no_argument,       NULL, 'c' },
    { "store-query",                      no_argument,       NULL, 'q' },
    { "store-frequency",                  no_argument,       NULL, 'f' },
    { "store-compression-row-chunk-size", required_argument, NULL, 'r' },
    { "index-query",                      required_argument, NULL, 'i' },
    { "lookup",                           required_argument, NULL, 'l' },
    { "store",                            required_argument, NULL, 's' },
    { "encoding-strategy",                required_argument, NULL, 'e' },
    { "encoding-cutoff-zero-min",         required_argument, NULL, 'n' },
    { "encoding-cutoff-zero-max",         required_argument, NULL, 'x' },
    { "lookup-frequency",                 no_argument,       NULL, 'u' },
    { "permutation-test",                 required_argument, NULL, 'm' },
    { "rng-seed",                         required_argument, NULL, 'd' },
    { "test-pearson-r",                   no_argument,       NULL, '1' },
    { "help",                             no_argument,       NULL, 'h' },
    { NULL,                               no_argument,       NULL,  0  }
}; 

static const char *bs_client_opt_string = "t:cqfr:i:l:s:e:n:x:um:d:1h?";

static const char *bs_name = "byte-store";

/**
 * @brief      bs_encode_unsigned_char_to_double_table[]
 *
 * @details    Converts encoded byte to assigned bin interval start, 
 *             e.g., 0x64 maps to -0.00 or (-0.01, -0.00], while 
 *             0x65 maps to +0.00 or [+0.00, +0.01). 
 *
 *             Note the two bins for -0.00 and +0.00. This allows 
 *             mapping of scores between -0.01, 0.00, and 0.01 to 
 *             be mapped to two seperate bytes.
 */

static const double bs_encode_unsigned_char_to_double_table[256] = 
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
     +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
     +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
     +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
     +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
     +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
     +0.00, +0.00, +0.00, +0.00};

/**
 * @brief      bs_encode_unsigned_char_to_double_mqz_table[]
 *
 * @details    Converts encoded byte to assigned bin interval start, 
 *             but maps (-0.25, +0.25) to +0.00
 */

static const double bs_encode_unsigned_char_to_double_mqz_table[256] = 
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
     +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
     +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
     +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
     +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
     +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, +0.00, 
     +0.00, +0.00, +0.00, +0.00};

inline double                bs_truncate_double_to_precision(double d, int prec);
inline unsigned char         bs_encode_double_to_unsigned_char(double d);
inline unsigned char         bs_encode_double_to_unsigned_char_mqz(double d);
unsigned char                bs_encode_double_to_unsigned_char_custom(double d, double min, double max);
inline boolean               bs_signbit(double d);
static inline double         bs_decode_unsigned_char_to_double(unsigned char uc);
static inline double         bs_decode_unsigned_char_to_double_mqz(unsigned char uc);
static inline double         bs_decode_unsigned_char_to_double_custom(unsigned char uc, double min, double max);
void                         bs_parse_query_str(lookup_t* l);
void                         bs_parse_query_str_to_indices(char* qs, uint32_t* start, uint32_t* stop);
lookup_t*                    bs_init_lookup(char* fn, boolean pi);
void                         bs_permute_lookup(lookup_t *l, FILE* os);
void                         bs_shuffle_signal_data(double* d, size_t n);
void                         bs_print_lookup(lookup_t* l, FILE* os);
void                         bs_print_lookup_frequency(lookup_t* l, FILE* os);
void                         bs_increment_lookup_frequency(uint64_t* t, lookup_t* l);
void                         bs_delete_lookup(lookup_t** l);
signal_t*                    bs_init_signal(char *cds);
void                         bs_print_signal(signal_t* s, FILE* os);
double                       bs_mean_signal(double* d, uint32_t len);
double                       bs_sample_sd_signal(double* d, uint32_t len, double m);
double                       bs_pearson_r_signal(signal_t* a, signal_t* b);
void                         bs_delete_signal(signal_t** s);
element_t*                   bs_init_element(char* chr, uint64_t start, uint64_t stop, char* id, boolean pi);
void                         bs_delete_element(element_t** e);
void                         bs_push_elem_to_lookup(element_t* e, lookup_t** l, boolean pi);
void                         bs_test_pearsons_r();
void                         bs_test_score_encoding();
void                         bs_init_globals();
void                         bs_init_command_line_options(int argc, char** argv);
void                         bs_print_usage(FILE* os);
inline boolean               bs_path_exists(const char* p);
inline ssize_t               bs_file_size(const char* fn);
sut_store_t*                 bs_init_sut_store(uint32_t n);
void                         bs_populate_sut_store_with_random_scores(sut_store_t* s);
void                         bs_populate_sut_store_with_pearsonr_scores(sut_store_t* s, lookup_t* l);
off_t                        bs_sut_byte_offset_for_element_ij(uint32_t n, uint32_t i, uint32_t j);
void                         bs_print_sut_store_to_bed7(lookup_t* l, sut_store_t* s, FILE* os);
void                         bs_print_sut_frequency_to_txt(lookup_t* l, sut_store_t* s, FILE* os);
void                         bs_delete_sut_store(sut_store_t** s);
sqr_store_t*                 bs_init_sqr_store(uint32_t n);
void                         bs_populate_sqr_store_with_random_scores(sqr_store_t* s);
void                         bs_populate_sqr_store_with_buffered_random_scores(sqr_store_t* s);
void                         bs_populate_sqr_store_with_pearsonr_scores(sqr_store_t* s, lookup_t* l);
void                         bs_populate_sqr_bzip2_store_with_pearsonr_scores(sqr_store_t* s, lookup_t* l, uint32_t n);
void                         bs_populate_sqr_bzip2_split_store_with_pearsonr_scores(sqr_store_t* s, lookup_t* l, uint32_t n);
char*                        bs_init_sqr_bzip2_split_store_dir_str(char* p);
char*                        bs_init_sqr_bzip2_split_store_fn_str(char* p, uint32_t i);
char*                        bs_init_sqr_bzip2_split_store_metadata_fn_str(char* d);
char*                        bs_init_metadata_str(off_t* o, uint32_t n, uint32_t s);
off_t                        bs_sqr_byte_offset_for_element_ij(uint32_t n, uint32_t i, uint32_t j);
void                         bs_print_sqr_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os);
void                         bs_print_sqr_bzip2_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os);
void                         bs_print_sqr_bzip2_split_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os);
metadata_t*                  bs_parse_metadata_str(char* ms);
void                         bs_delete_metadata(metadata_t** m);
void                         bs_print_sqr_store_frequency_to_txt(lookup_t* l, sqr_store_t* s, FILE* os);
void                         bs_print_sqr_bzip2_store_frequency_to_txt(lookup_t* l, sqr_store_t* s, FILE* os);
void                         bs_print_sqr_bzip2_split_store_frequency_to_txt(lookup_t* l, sqr_store_t* s, FILE* os);
void                         bs_print_frequency_buffer(uint64_t* t, uint64_t n, FILE* os);
void                         bs_delete_sqr_store(sqr_store_t** s);
store_buf_node_t*            bs_init_store_buf_node(unsigned char uc);
void                         bs_insert_store_buf_node(store_buf_node_t* n, store_buf_node_t* i);

#endif // BYTE_STORE_H_
