#include "byte-store.h"

int
main(int argc, char** argv) 
{
    bs_init_globals();
    bs_init_command_line_options(argc, argv);

    sut_store_t* sut_store = NULL;
    lookup_t* lookup = NULL;
    lookup = bs_init_lookup(bs_globals.lookup_fn);

    switch(bs_globals.store_type) {
    case kStoreSUT:
        sut_store = bs_init_sut_store(lookup->nelems);
        if (bs_globals.store_create_flag) {
            bs_populate_sut_store_with_random_scores(sut_store);
        }
        else if (bs_globals.store_query_flag) {
            bs_parse_query_str(lookup);
            bs_print_sut_store_to_bed7(lookup, sut_store, stdout);
        }
        bs_delete_sut_store(&sut_store);
        break;
    case kStoreSquareMatrix:
        break;
    case kStoreUndefined:
        fprintf(stderr, "Error: You should never see this error!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
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
 *             prec   (signed integer) value to determine 
 *                    precision of truncated value
 *
 * @return     (double) truncated value
 */

inline static double
bs_truncate_double_to_precision(double d, int prec)
{
    double factor = powf(10, prec);
    return (d < 0) ? ceil(d * factor)/factor : floor((d + kEpsilon) * factor)/factor;
}

/**
 * @brief      bs_encode_double_to_unsigned_char(d)
 *
 * @details    Encodes double-type value between -1 and +1 to 
 *             unsigned char-type byte "bin".
 *
 *             (-1.00, -0.99] (-0.99, -0.98] ...
 *                   ... (-0.01, -0.00] [+0.00, +0.01) ... 
 *                           ... [+0.98, +0.99) [+0.99, +1.00)
 *
 *                            ---to---
 *
 *             { 0x00, 0x01, ... , 0x64, 0x65, ... , 0xc9 }
 *
 * @param      d      (double) value to be encoded
 *
 * @return     (unsigned char) encoded score byte value
 *
 * @example    bs_encode_double_to_unsigned_char(-0.010) = 0x63
 *             bs_encode_double_to_unsigned_char(-0.009) = 0x64
 *             bs_encode_double_to_unsigned_char(-0.000) = 0x64
 *             bs_encode_double_to_unsigned_char(+0.000) = 0x65
 *             bs_encode_double_to_unsigned_char(+0.139) = 0x72
 *             bs_encode_double_to_unsigned_char(+0.140) = 0x73
 *             bs_encode_double_to_unsigned_char(+0.142) = 0x73
 */

inline static unsigned char
bs_encode_double_to_unsigned_char(double d) 
{
    d += (d < 0) ? -kEpsilon : kEpsilon; /* jitter is used to deal with interval edges */
    d = bs_truncate_double_to_precision(d, 2);
    int encode_d = (int) ((d < 0) ? (ceil(d * 1000.0f)/10.0f + 100) : (floor(d * 1000.0f)/10.0f + 100)) + signbit(-d);
    return (unsigned char) encode_d;
}

/**
 * @brief      bs_parse_query_str()
 *
 * @details    Parses index-query string and performs some validation
 *
 * @param      l      (lookup_t*) pointer to lookup table
 */

void
bs_parse_query_str(lookup_t* l)
{
    /* parse query string for index values */
    bs_parse_query_str_to_indices(bs_globals.store_query_str, 
                                  &bs_globals.store_query_idx_start, 
                                  &bs_globals.store_query_idx_end);
    
    if (((bs_globals.store_query_idx_start + 1) > l->nelems) || ((bs_globals.store_query_idx_end + 1) > l->nelems)) {
        fprintf(stderr, "Error: Index range outside of number of elements in lookup table!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }
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
    char* qs_delim = strchr(qs, kQueryDelim);
    ssize_t start_len = qs_delim - qs;
    ssize_t end_len = qs_len - start_len;

    if ((!qs_delim) || 
        (start_len < 0) || 
        (start_len >= QUERY_MAX_LEN) || 
        (end_len < 0) || 
        (end_len >= QUERY_MAX_LEN)) 
        {
            fprintf(stderr, "Error: Index query string not formatted correctly?\n");
            bs_print_usage(stderr);
            exit(EXIT_FAILURE);
        }

    memcpy(start_str, qs, start_len);
    memcpy(end_str, qs_delim + 1, end_len);
    
    *start = (uint32_t) strtol(start_str, NULL, 10);
    *end = (uint32_t) strtol(end_str, NULL, 10);
}

/**
 * @brief      bs_decode_unsigned_char_to_double(uc)
 *
 * @details    Decodes unsigned char-type byte bin to
 *             equivalent score bin.
 *
 * @param      uc     (unsigned char) value to be decoded
 *
 * @return     (double) decoded score bin start value
 *
 * @example    bs_decode_unsigned_char_to_double(0x64) = -0.00 -- or bin (-0.01, -0.00]
 *             bs_decode_unsigned_char_to_double(0x65) = +0.00 -- or bin [+0,00, +0.01)
 *             bs_decode_unsigned_char_to_double(0x73) = +0.14 -- or bin [+0,14, +0.15)
 */

inline static double
bs_decode_unsigned_char_to_double(unsigned char uc)
{
    return bs_encode_unsigned_char_to_double_table[uc];
}

/**
 * @brief      bs_init_lookup(fn)
 *
 * @details    Read BED-formatted coordinates into a "lookup table" pointer.
 *             Function allocates memory to lookup table pointer, as needed.
 *
 * @param      fn     (char*) filename string
 *
 * @return     (lookup_t*) lookup table pointer referencing element data
 */

lookup_t*
bs_init_lookup(char* fn)
{
    lookup_t* l = NULL;
    FILE* lf = NULL;
    char buf[BUF_MAX_LEN];
    char chr_str[BUF_MAX_LEN];
    char start_str[BUF_MAX_LEN];
    char stop_str[BUF_MAX_LEN];
    char id_str[BUF_MAX_LEN];
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
        element_t* e = bs_init_element(chr_str, start_val, stop_val, id_str);
        bs_push_elem_to_lookup(e, &l);
    }

    fclose(lf);

    return l;
}

/**
 * @brief      bs_print_lookup(l)
 *
 * @details    Print contents of lookup_t* to standard output (for debugging).
 *
 * @param      l      (lookup_t*) lookup table pointer
 */

void
bs_print_lookup(lookup_t* l)
{
    fprintf(stdout, "----------------------\n");
    fprintf(stdout, "Lookup\n");
    fprintf(stdout, "----------------------\n");
    for (uint32_t idx = 0; idx < l->nelems; idx++) {
        fprintf(stdout, 
                "Element [%09d] [%s | %" PRIu64 " | %" PRIu64 " | %s]\n", 
                idx, 
                l->elems[idx]->chr,
                l->elems[idx]->start,
                l->elems[idx]->stop,
                l->elems[idx]->id);
    }
    fprintf(stdout, "----------------------\n");
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
    free((*l)->elems);
    (*l)->elems = NULL;
    (*l)->nelems = 0;
    (*l)->capacity = 0;
    free(*l);
    *l = NULL;
}

/**
 * @brief      bs_init_element(chr, start, stop, id)
 *
 * @details    Allocates space for element_t* and copies chr, start, stop
 *             and id values to element.
 *
 * @param      chr    (char*) chromosome string 
 *             start  (uint64_t) start coordinate position
 *             stop   (uint64_t) stop coordinate position
 *             id     (char*) id string 
 *
 * @return     (element_t*) element pointer
 */

element_t*
bs_init_element(char* chr, uint64_t start, uint64_t stop, char* id)
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
    if (strlen(id) > 0) {
        e->id = malloc(sizeof(*id) * strlen(id) + 1);
        if (!e->id) {
            fprintf(stderr,"Error: Could not allocate space for element id!\n");
            exit(EXIT_FAILURE);
        }
        memcpy(e->id, id, strlen(id) + 1);
    }
    
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
    free((*e)->chr);
    (*e)->chr = NULL;
    (*e)->start = 0;
    (*e)->stop = 0;
    free((*e)->id);
    (*e)->id = NULL;
    free(*e);
    *e = NULL;
}

/**
 * @brief      bs_push_elem_to_lookup(e, l)
 *
 * @details    Pushes element_t pointer to lookup table.
 *
 * @param      e      (element_t*) element pointer
 *             l      (lookup_t**) pointer to lookup table pointer
 */

void
bs_push_elem_to_lookup(element_t* e, lookup_t** l)
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
                                             (*l)->elems[idx]->id);
            bs_delete_element(&((*l)->elems[idx]));
        }   
        (*l)->elems = new_elems;     
    }
    uint32_t n = (*l)->nelems;
    (*l)->elems[n] = e;
    (*l)->nelems++;
}

/**
 * @brief      bs_test_score_encoding()
 *
 * @details    Tests encoding of scores in the interval 
 *             [-1.0, +1.0] at kEpsilon increments. 
 *
 *             Encoded scores are unsigned char byte values 
 *             equivalent to the provided score value. Decoded 
 *             scores are the double-typed "bin" with which the 
 *             original score associates.
 */

void
bs_test_score_encoding()
{
    double d;
    int count;

    for (d = -1.0f, count = 0; d <= 1.0f; d += kEpsilon, ++count) {
	unsigned char encode_d = bs_encode_double_to_unsigned_char(d);
	double decode_d = bs_decode_unsigned_char_to_double(encode_d);
        fprintf(stderr, 
                "Test [%07d] [ %3.7f ]\t-> [ 0x%02x ]\t-> [ %3.7f ]\n",
		count,
                d, 
                encode_d,
                decode_d);
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
    bs_globals.store_query_str[0] = '\0';
    bs_globals.store_query_idx_start = 0;
    bs_globals.store_query_idx_end = 0;
    bs_globals.rng_seed_flag = kFalse;
    bs_globals.rng_seed_value = 0;
    bs_globals.lookup_fn[0] = '\0';
    bs_globals.store_fn[0] = '\0';
    bs_globals.store_type_str[0] = '\0';
    bs_globals.store_type = kStoreUndefined;
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
    /*
    fprintf(stderr, "argc:\t\t[%d]\n", argc);
    for (int argc_idx = 0; argc_idx < argc; argc_idx++)
        fprintf(stderr, "argv[%02d]:\t[%s]\n", argc_idx, argv[argc_idx]); 
    */

    int bs_client_long_index;
    int bs_client_opt = getopt_long(argc,
                                    argv,
                                    bs_client_opt_string,
                                    bs_client_long_options,
                                    &bs_client_long_index);

    opterr = 0;

    while (bs_client_opt != -1) {
        switch (bs_client_opt) {
        case 't':
            memcpy(bs_globals.store_type_str, optarg, strlen(optarg) + 1);
            bs_globals.store_type =
                (strcmp(bs_globals.store_type_str, kStoreSUTStr) == 0) ? kStoreSUT :
                (strcmp(bs_globals.store_type_str, kStoreSquareMatrixStr) == 0) ? kStoreSquareMatrix :
                kStoreUndefined;
            break;
        case 'c':
            bs_globals.store_create_flag = kTrue;
            break;
        case 'q':
            bs_globals.store_query_flag = kTrue;
            break;
        case 'i':
            memcpy(bs_globals.store_query_str, optarg, strlen(optarg) + 1);
            break;
        case 'l':
            memcpy(bs_globals.lookup_fn, optarg, strlen(optarg) + 1);
            if (!bs_file_exists(bs_globals.lookup_fn)) {
                fprintf(stderr, "Error: Lookup file [%s] does not exist!\n", bs_globals.lookup_fn);
                bs_print_usage(stderr);
                exit(EXIT_FAILURE);
            }
            break;
        case 's':
            memcpy(bs_globals.store_fn, optarg, strlen(optarg) + 1);
            break;
        case 'd':
            bs_globals.rng_seed_flag = kTrue;
            bs_globals.rng_seed_value = (uint32_t) strtol(optarg, NULL, 10);
            break;
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

    if (bs_globals.store_create_flag && bs_globals.store_query_flag) {
        fprintf(stderr, "Error: Cannot both create and query data store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    if (!bs_globals.store_create_flag && !bs_globals.store_query_flag) {
        fprintf(stderr, "Error: Must either create or query a data store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    if (strlen(bs_globals.lookup_fn) == 0) {
        fprintf(stderr, "Error: Must specify lookup table filename!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    if (strlen(bs_globals.store_fn) == 0) {
        fprintf(stderr, "Error: Must specify store filename!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    if (bs_globals.store_type == kStoreUndefined) {
        fprintf(stderr, "Error: Must specify store type!\n");
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
            "\t Create data store:\n" \
            "\t\t %s --store-create --store-type [sut|sqr] --lookup=fn --store=fn\n\n" \
            "\t Query data store:\n" \
            "\t\t %s --store-query  --store-type [sut|sqr] --lookup=fn --store=fn --index-query=str\n\n" \
            " Notes:\n\n" \
            " - Lookup file is a sorted BED3 or BED4 file\n\n" \
            " - Query string is a numeric range specifing indices of interest from lookup\n" \
            "   table (e.g. \"17-83\" represents indices 17 through 83)\n" \
            " - Output is in BED7 format (BED3 + BED3 + floating point score)\n\n",
            bs_name,
            bs_name);
}

/**
 * @brief      bs_file_exists(fn)
 *
 * @details    Returns kTrue or kFalse depending on whether filename 
 *             refers to an existing file.
 *
 * @param      fn      (const char*) filename to test existence
 */ 

inline boolean 
bs_file_exists(const char* fn)
{
    struct stat buf;
    return (boolean) (stat(fn, &buf) == 0);
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
    memcpy(a->fn, bs_globals.store_fn, strlen(bs_globals.store_fn) + 1);

    s->attr = a;

    return s;
}

/**
 * @brief      bs_populate_sut_store_with_random_scores(s)
 *
 * @details    Write randomly-generated unsigned char bytes to a
 *             FILE* handle associated with the specified SUT 
 *             store filename.
 *
 * @param      s      (sut_store_t*) pointer to SUT struct
 */

void
bs_populate_sut_store_with_random_scores(sut_store_t* s)
{
    unsigned char score = 0;
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
            score = (unsigned char) (mt19937_generate_random_ulong() % 256);
        } while (score > 200); /* sample until a {0, ..., 200} bin is referenced */
        if (fputc(score, os) != score) {
            fprintf(stderr, "Error: Could not write score to output SUT store!\n");
            exit(EXIT_FAILURE);
        }
    }

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
 * @brief      bs_print_sut_store_to_bed7(l, os)
 *
 * @details    Queries SUT store for provided index range globals
 *             and prints BED7 (BED3 + BED3 + floating point) to 
 *             specified output stream. The two BED3 elements are 
 *             retrieved from the lookup table and represent a 
 *             score pairing.
 *
 * @param      l      (lookup_t*) pointer to lookup table
 *             os     (FILE*) pointer to output stream
 */

void
bs_print_sut_store_to_bed7(lookup_t* l, sut_store_t* s, FILE* os)
{
    if (!bs_file_exists(s->attr->fn)) {
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

    if (bs_globals.store_query_idx_start != bs_globals.store_query_idx_end) {
        /* swap indices, if row and column range are in lower triangle */
        if (bs_globals.store_query_idx_start > bs_globals.store_query_idx_end) {
            swap(bs_globals.store_query_idx_start, bs_globals.store_query_idx_end);
        }
        for (uint32_t row_idx = bs_globals.store_query_idx_start; row_idx < bs_globals.store_query_idx_end; row_idx++) {
            for (uint32_t col_idx = row_idx + 1; col_idx <= bs_globals.store_query_idx_end; col_idx++) {
                off_t is_offset = bs_sut_byte_offset_for_element_ij(l->nelems, row_idx, col_idx);
                fseek(is, is_offset, SEEK_SET);
                int uc = fgetc(is);
                fprintf(os, 
                        "%s\t%" PRIu64 "\t%" PRIu64"\t%s\t%" PRIu64 "\t%" PRIu64 "\t%3.2f\n",
                        l->elems[row_idx]->chr,
                        l->elems[row_idx]->start,
                        l->elems[row_idx]->stop,
                        l->elems[col_idx]->chr,
                        l->elems[col_idx]->start,
                        l->elems[col_idx]->stop,
                        bs_decode_unsigned_char_to_double((unsigned char) uc));
            }
        }
    } 
    else {
        fprintf(stderr, "Warning: Store offset NA for diagonal\n");
    }

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
    (*s)->attr->nbytes = 0;
    (*s)->attr->nelems = 0;
    free((*s)->attr);
    (*s)->attr = NULL;
    free(*s);
    *s = NULL;
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
    memcpy(a->fn, bs_globals.store_fn, strlen(bs_globals.store_fn) + 1);

    s->attr = a;

    return s;
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
    (*s)->attr->nbytes = 0;
    (*s)->attr->nelems = 0;
    free((*s)->attr);
    (*s)->attr = NULL;
    free(*s);
    *s = NULL;
}
