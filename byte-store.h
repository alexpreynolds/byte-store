#ifndef BYTE_STORE_H_
#define BYTE_STORE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdint.h>
#include <time.h>
#include "mt19937.h"

#define BUF_MAX_LEN 1024
#define FN_MAX_LEN 1024

typedef int boolean;
extern const boolean kTrue;
extern const boolean kFalse;

const boolean kTrue = 1;
const boolean kFalse = 0;

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

typedef struct store {
    uint32_t nelems;
    uint64_t nbytes;
} store_t;

static struct bs_global_args_t {
    boolean store_create_flag;
    boolean store_query_flag;
    boolean rng_seed_flag;
    uint32_t rng_seed_value;
    char lookup_fn[FN_MAX_LEN];
    char store_fn[FN_MAX_LEN];
} bs_global_args;

static struct option bs_client_long_options[] = {
    { "store-create", no_argument,       NULL, 'c' },
    { "store-query",  no_argument,       NULL, 'q' },
    { "lookup",       required_argument, NULL, 'l' },
    { "store",        required_argument, NULL, 's' },
    { "rng-seed",     required_argument, NULL, 'd' },
    { "help",         no_argument,       NULL, 'h' },
    { NULL,           no_argument,       NULL,  0  }
}; 

static const char *bs_client_opt_string = "cql:s:dh?";

static const char *bs_name = "byte-store";

off_t      bs_byte_offset_for_element_ij(uint32_t n, uint32_t i, uint32_t j);
lookup_t*  bs_init_lookup(char* fn);
void       bs_print_lookup(lookup_t* l);
void       bs_delete_lookup(lookup_t** l);
element_t* bs_init_element(char* chr, uint64_t start, uint64_t stop, char* id);
void       bs_delete_element(element_t** e);
void       bs_push_elem_to_lookup(element_t* e, lookup_t** l);
store_t*   bs_init_store(uint32_t n);
void       bs_populate_store(store_t* s);
void       bs_delete_store(store_t** s);
void       bs_init_globals();
void       bs_init_command_line_options(int argc, char** argv);
void       bs_print_usage(FILE* os);

#endif // BYTE_STORE_H_
