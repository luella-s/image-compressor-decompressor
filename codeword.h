#ifndef CODEWORD_INCLUDED
#define CODEWORD_INCLUDED

#include "dct_quant.h"
#include "bitpack.h"

/* Encodes Quant structs into 64-bit words in row-major order */
UArray_T encode(UArray2_T quant_arr);

UArray2_T decode(UArray_T codeword_arr);
 
#endif