#ifndef PRINT_READ_INCLUDED
#define PRINT_READ_INCLUDED

#include "dct_quant.h"
#include "bitpack.h"

void print_binary(UArray2_T decode_arr);
void read_binary(FILE *input);

#endif