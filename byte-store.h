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
#include "mt19937.h"

#define BUF_MAX_LEN 1024
#define FN_MAX_LEN 1024
#define QUERY_MAX_LEN 40

#define swap(x,y) do \
{ unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
    memcpy(swap_temp,&y,sizeof(x)); \
    memcpy(&y,&x,       sizeof(x)); \
    memcpy(&x,swap_temp,sizeof(x)); \
} while(0)

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

typedef struct element {
    char* chr;
    uint64_t start;
    uint64_t stop;
    char* id;
} element_t;

typedef struct lookup {
    uint64_t capacity;
    uint32_t nelems;
    element_t** elems;
} lookup_t;

typedef struct store_attr {
    uint32_t nelems;
    uint64_t nbytes;
    char fn[FN_MAX_LEN];
} store_attr_t;

typedef struct sut_store {
    store_attr_t* attr;
} sut_store_t;

typedef struct sqr_store {
    store_attr_t* attr;
} sqr_store_t;

extern const char* kStoreSUTStr;
extern const char* kStoreSquareMatrixStr;
const char* kStoreSUTStr = "sut";
const char* kStoreSquareMatrixStr = "sqr";

typedef enum store_type {
    kStoreSUT = 0,
    kStoreSquareMatrix,
    kStoreUndefined
} store_type_t;

static struct bs_globals_t {
    boolean store_create_flag;
    boolean store_query_flag;
    char store_query_str[QUERY_MAX_LEN];
    uint32_t store_query_idx_start;
    uint32_t store_query_idx_end;
    boolean rng_seed_flag;
    uint32_t rng_seed_value;
    char lookup_fn[FN_MAX_LEN];
    char store_fn[FN_MAX_LEN];
    char store_type_str[BUF_MAX_LEN];
    store_type_t store_type;
} bs_globals;

static struct option bs_client_long_options[] = {
    { "store-type",   required_argument, NULL, 't' },
    { "store-create", no_argument,       NULL, 'c' },
    { "store-query",  no_argument,       NULL, 'q' },
    { "index-query",  required_argument, NULL, 'i' },
    { "lookup",       required_argument, NULL, 'l' },
    { "store",        required_argument, NULL, 's' },
    { "rng-seed",     required_argument, NULL, 'd' },
    { "help",         no_argument,       NULL, 'h' },
    { NULL,           no_argument,       NULL,  0  }
}; 

static const char *bs_client_opt_string = "t:cqi:l:s:dh?";

static const char *bs_name = "byte-store";

/**
 * @brief      bs_encode_unsigned_char_to_double_table[]
 *
 * @details    Converts encoded byte to assigned bin interval start, 
 *             e.g., 0x64 maps to -0.00 or (-0.01, -0.00], while 
 *             0x65 maps to +0.00 or [+0.00, +0.01). 
 *
 *             Note the two bins for -0.00 and +0.00. This allows 
 *             mapping of scores between -0.01 and 0.01 to be mapped
 *             to two seperate bytes.
 */

static const double bs_encode_unsigned_char_to_double_table[256] = 
    {-1.00, 
     -0.99, -0.98, -0.97, -0.96, -0.95, -0.94, -0.93, -0.92, -0.91, -0.90,
     -0.89, -0.88, -0.87, -0.86, -0.85, -0.84, -0.83, -0.82, -0.81, -0.80
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

static inline double         bs_truncate_double_to_precision(double d, int prec);
static inline unsigned char  bs_encode_double_to_unsigned_char(double d);
static inline double         bs_decode_unsigned_char_to_double(unsigned char uc);
void                         bs_parse_query_str(lookup_t* l);
void                         bs_parse_query_str_to_indices(char* qs, uint32_t* start, uint32_t* stop);
lookup_t*                    bs_init_lookup(char* fn);
void                         bs_print_lookup(lookup_t* l);
void                         bs_delete_lookup(lookup_t** l);
element_t*                   bs_init_element(char* chr, uint64_t start, uint64_t stop, char* id);
void                         bs_delete_element(element_t** e);
void                         bs_push_elem_to_lookup(element_t* e, lookup_t** l);
void                         bs_test_score_encoding();
void                         bs_init_globals();
void                         bs_init_command_line_options(int argc, char** argv);
void                         bs_print_usage(FILE* os);
inline boolean               bs_file_exists(const char* fn);
sut_store_t*                 bs_init_sut_store(uint32_t n);
void                         bs_populate_sut_store_with_random_scores(sut_store_t* s);
off_t                        bs_sut_byte_offset_for_element_ij(uint32_t n, uint32_t i, uint32_t j);
void                         bs_print_sut_store_to_bed7(lookup_t* l, sut_store_t* s, FILE* os);
void                         bs_delete_sut_store(sut_store_t** s);
sqr_store_t*                 bs_init_sqr_store(uint32_t n);
void                         bs_populate_sqr_store_with_random_scores(lookup_t* l);
off_t                        bs_sqr_byte_offset_for_element_ij(uint32_t n, uint32_t i, uint32_t j);
void                         bs_print_sqr_store_to_bed7(lookup_t* l, sqr_store_t* s, FILE* os);
void                         bs_delete_sqr_store(sqr_store_t** s);

#endif // BYTE_STORE_H_
