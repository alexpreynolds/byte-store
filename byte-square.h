#ifndef BYTE_SQUARE_H_
#define BYTE_SQUARE_H_

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "mt19937.h"

void bs_init_command_line_options(int argc, char** argv);
void bs_print_usage(FILE* output_stream);

static struct option bs_client_long_options[] = {
    { "help", no_argument, NULL, 'h' },
    { NULL,   no_argument, NULL,  0  }
}; 

static const char *bs_client_opt_string = "h?";

static const char *bs_name = "byte-square";

#endif // BYTE_SQUARE_H_
