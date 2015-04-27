#include "byte-store.h"

int
main(int argc, char** argv) 
{
    lookup_t *lookup = NULL;
    sut_store_t* sut_store = NULL;

    bs_init_globals();
    bs_init_command_line_options(argc, argv);

    lookup = bs_init_lookup(bs_global_args.lookup_fn);
    sut_store = bs_init_sut_store(lookup->nelems);
        
    if (bs_global_args.sut_store_create_flag) {
        bs_populate_sut_store_with_random_scores(sut_store);
    }
    else if (bs_global_args.sut_store_query_flag) {
    }

    bs_delete_sut_store(&sut_store);
    bs_delete_lookup(&lookup);

    return EXIT_SUCCESS;
}

/**
 * @brief      bs_test_score_encoding()
 *
 * @details    Encodes scores in the interval [-1.0, +1.0]
 *             at 1e-6 increments. Encoded scores are 
 *             unsigned char byte values equivalent to 
 *             the provided score value. Decoded scores are
 *             the double-type "bin" with which the original
 *             score associates.
 */

void
bs_test_score_encoding()
{
    double epsilon = 0.000001f;
    double d;
    int count;

    for (d = -1.0f, count = 0; d <= 1.0f; d += epsilon, ++count) {
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
    return (d < 0) ? ceil(d * factor)/factor : floor((d + 0.000001) * factor)/factor;
}

/**
 * @brief      bs_encode_double_to_unsigned_char(d)
 *
 * @details    Encodes double-type value to unsigned char-type
 *             byte bin.
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
    double epsilon = 0.0000001f;
    d += (d < 0) ? -epsilon : epsilon; /* jitter is used to deal with interval edges */
    d = bs_truncate_double_to_precision(d, 2);
    int encode_d = (int) ((d < 0) ? (ceil(d * 1000.0f)/10.0f + 100) : (floor(d * 1000.0f)/10.0f + 100)) + signbit(-d);
    return (unsigned char) encode_d;
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
 * @brief      bs_sut_byte_offset_for_element_ij(n, i, j)
 *
 * @details    Returns the byte offset (off_t) value from a linear
 *             representation of bytes in a strictly upper-triangular (SUT)
 *             matrix of order n, for given row i and column j.
 *
 * @param      n      (uint32_t) order of square matrix
 *             i      (uint32_t) i-th row of matrix
 *             j      (uint32_t) j-th column of matrix
 *
 * @return     (off_t) byte offset into SUT byte array
 *
 * @todo       Test condition where i == j.
 */

off_t
bs_sut_byte_offset_for_element_ij(uint32_t n, uint32_t i, uint32_t j)
{
    /* cf. http://stackoverflow.com/a/27088560/19410 */
    return (n * (n - 1)/2) - (n - i)*((n - i) - 1)/2 + j - i - 1; 
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
    if (lf) {
        while (fgets(buf, BUF_MAX_LEN, lf)) {
            sscanf(buf, "%s\t%s\t%s\t%s\n", chr_str, start_str, stop_str, id_str);
            sscanf(start_str, "%" SCNu64, &start_val);
            sscanf(stop_str, "%" SCNu64, &stop_val);
            element_t* e = bs_init_element(chr_str, start_val, stop_val, id_str);
            bs_push_elem_to_lookup(e, &l);
        }
    }
    else {
        fprintf(stderr, "Error: Could not open or read from [%s]\n", fn);
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
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

    s = malloc(sizeof(sut_store_t));
    if (!s) {
        fprintf(stderr, "Error: Could not allocate space for SUT store!\n");
        exit(EXIT_FAILURE);
    }
    s->nelems = n;
    s->nbytes = n * (n - 1) / 2; /* strictly upper triangular (SUT) matrix */

    return s;
}

/**
 * @brief      bs_populate_sut_store_with_random_scores(s)
 *
 * @details    Write randomly-generated unsigned char bytes to a
 *             FILE* handle associated with the specified SUT 
 *             store filename.
 *
 * @param      s      (sut_store_t*) pointer to SUT attribute struct
 */

void
bs_populate_sut_store_with_random_scores(sut_store_t* s)
{
    unsigned char score = 0;
    FILE* os = NULL;
    
    /* seed RNG */
    if (bs_global_args.rng_seed_flag)
        mt19937_seed_rng(bs_global_args.rng_seed_value);
    else
        mt19937_seed_rng(time(NULL));

    os = fopen(bs_global_args.sut_store_fn, "wb");
    if (ferror(os)) {
        fprintf(stderr, "Error: Could not open handle to output SUT store!\n");
        exit(EXIT_FAILURE);
    }

    /* write stream of random scores out to os ptr */
    for (uint32_t idx = 0; idx < s->nbytes; idx++) {
        do {
            score = (unsigned char) (mt19937_generate_random_ulong() % 256);
        } while (score > 200);
        if (fputc(score, os) != score) {
            fprintf(stderr, "Error: Could not write score to output SUT store!\n");
            exit(EXIT_FAILURE);
        }
        /* fprintf(stderr, "writing score [%c | %3.2f]\n", score, bs_encode_unsigned_char_to_double[score]); */
    }

    fclose(os);
}

/**
 * @brief      bs_delete_sut_store(s)
 *
 * @details    Release memory associated with SUT store pointer.
 *
 * @param      s      (sut_store_t**) pointer to SUT attribute struct pointer
 */

void
bs_delete_sut_store(sut_store_t** s)
{
    (*s)->nbytes = 0;
    (*s)->nelems = 0;
    free(*s);
    *s = NULL;
}

/**
 * @brief      bs_init_globals()
 *
 * @details    Initialize application global variables.
 */

void
bs_init_globals()
{
    bs_global_args.sut_store_create_flag = kFalse;
    bs_global_args.sut_store_query_flag = kFalse;
    bs_global_args.rng_seed_flag = kFalse;
    bs_global_args.rng_seed_value = 0;
    bs_global_args.lookup_fn[0] = '\0';
    bs_global_args.sut_store_fn[0] = '\0';
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
        case 'c':
            bs_global_args.sut_store_create_flag = kTrue;
            break;
        case 'q':
            bs_global_args.sut_store_query_flag = kTrue;
            break;
        case 'l':
            memcpy(bs_global_args.lookup_fn, optarg, strlen(optarg) + 1);
            break;
        case 's':
            memcpy(bs_global_args.sut_store_fn, optarg, strlen(optarg) + 1);
            break;
        case 'd':
            bs_global_args.rng_seed_flag = kTrue;
            bs_global_args.rng_seed_value = atoi(optarg);
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

    if (bs_global_args.sut_store_create_flag && bs_global_args.sut_store_query_flag) {
        fprintf(stderr, "Error: Cannot both create and query SUT data store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    if (!bs_global_args.sut_store_create_flag && !bs_global_args.sut_store_query_flag) {
        fprintf(stderr, "Error: Must either create or query a SUT data store!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    if (strlen(bs_global_args.lookup_fn) == 0) {
        fprintf(stderr, "Error: Must specify lookup table filename!\n");
        bs_print_usage(stderr);
        exit(EXIT_FAILURE);
    }

    if (strlen(bs_global_args.sut_store_fn) == 0) {
        fprintf(stderr, "Error: Must specify SUT store filename!\n");
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
            "\t Create SUT data store:\n" \
            "\t\t %s --store-create --lookup=fn --store=fn\n\n" \
            "\t Query SUT data store:\n" \
            "\t\t %s --store-query  --lookup=fn --store=fn --query=str\n\n" \
            " Notes:\n\n" \
            " - Lookup file is a sorted BED3 or BED4 file\n\n" \
            " - Query string is a numeric range specifing indices of interest from lookup\n" \
            "   table (e.g. \"17-83\" represents indices 17 through 83)\n\n",
            bs_name,
            bs_name);
}
