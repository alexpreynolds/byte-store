#include "byte-square.h"

int
main(int argc, char** argv) 
{
    bs_init_command_line_options(argc, argv);
    return EXIT_SUCCESS;
}

void 
bs_init_command_line_options(int argc, char** argv)
{
    fprintf(stderr, "argc:\t\t[%d]\n", argc);
    for (int argc_idx = 0; argc_idx < argc; argc_idx++) { 
        fprintf(stderr, "argv[%02d]:\t[%s]\n", argc_idx, argv[argc_idx]); 
    }

    int bs_client_long_index;
    int bs_client_opt = getopt_long(argc,
                                    argv,
                                    bs_client_opt_string,
                                    bs_client_long_options,
                                    &bs_client_long_index);

    opterr = 0;

    while (bs_client_opt != -1) {
        switch (bs_client_opt) {
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
}

void 
bs_print_usage(FILE* output_stream) 
{
    fprintf(output_stream,
            "Usage: %s (--store-make|--store-query) --lookup-file=fn --store-file=fn --query-string=str\n",
            bs_name);
}
