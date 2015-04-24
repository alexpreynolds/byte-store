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

void
bs_test_score_encoding()
{
    double d;
    double epsilon = 0.00001;
    unsigned char uc;

    for (d = -1.0f, uc = 0x00; d <= 1.0f; d += 0.01f, uc++) {
        fprintf(stderr, "Testing [ %3.2f | 0x%02x ] -> [ 0x%02x | %3.2f ]\n", d, uc, bs_encode_double_to_unsigned_char(d), bs_encode_unsigned_char_to_double(uc));
        assert(bs_encode_double_to_unsigned_char(d) == uc);
        assert(fabs(bs_encode_unsigned_char_to_double(uc) - d) < epsilon);
    }
}

inline double
bs_encode_unsigned_char_to_double(unsigned char uc)
{
    return bs_encode_unsigned_char_to_double_table[uc];
}

inline unsigned char
bs_encode_double_to_unsigned_char(double d) 
{
    int d_lookup = (int) (((d < 0) ? ceil(d * 100.0f) : floor((d + 0.01f) * 100.0f)) + 100.0f);
    return bs_encode_double_to_unsigned_char_table[d_lookup];
}

off_t
bs_sut_byte_offset_for_element_ij(uint32_t n, uint32_t i, uint32_t j)
{
    return (n*(n-1)/2) - (n-i)*((n-i)-1)/2 + j - i - 1; /* cf. http://stackoverflow.com/a/27088560/19410 */
}

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

void
bs_print_lookup(lookup_t* l)
{
    fprintf(stderr, "----------------------\n");
    fprintf(stderr, "Lookup\n");
    fprintf(stderr, "----------------------\n");
    for (uint32_t idx = 0; idx < l->nelems; idx++) {
        fprintf(stderr, 
                "Element [%09d] [%s | %" PRIu64 " | %" PRIu64 " | %s]\n", 
                idx, 
                l->elems[idx]->chr,
                l->elems[idx]->start,
                l->elems[idx]->stop,
                l->elems[idx]->id);
    }
    fprintf(stderr, "----------------------\n");
}

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
        } while (score > 201);
        if (fputc(score, os) != score) {
            fprintf(stderr, "Error: Could not write score to output SUT store!\n");
            exit(EXIT_FAILURE);
        }
        /* fprintf(stderr, "writing score [%c | %3.2f]\n", score, bs_encode_unsigned_char_to_double[score]); */
    }

    fclose(os);
}

void
bs_delete_sut_store(sut_store_t** s)
{
    (*s)->nbytes = 0;
    (*s)->nelems = 0;
    free(*s);
    *s = NULL;
}

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
