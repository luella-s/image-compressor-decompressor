#ifndef DCT_QUANT_INCLUDED
#define DCT_QUANT_INCLUDED

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "uarray2.h"
#include "structs.h"
#include "arith40.h"
#include "math.h"
#include "helper.h"

/* Main function */
UArray2_T quantize(UArray2_T dct_arr);
UArray2_T dequantize(UArray2_T quant_arr);

/* Mapping function */
void apply_quantize(int i, int j, UArray2_T array2, void *elem, void *cl);
void apply_dequant(int col, int row, UArray2_T quant_arr, void *elem, 
    void *cl);

/* Helper Functions */
signed quantize_bcd(float n);
uint64_t power_2(int shift);
float dequantize_a(unsigned a);
float dequantize_bcd(signed n);

#endif