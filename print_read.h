#ifndef PRINT_READ_INCLUDED
#define PRINT_READ_INCLUDED

#include "dct_quant.h"
#include "bitpack.h"

void print_binary(UArray2_T coded_arr);
UArray2_T read_binary(FILE *input);

#endif