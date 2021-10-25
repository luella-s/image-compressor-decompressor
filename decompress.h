#ifndef DECOMPRESS_INCLUDED
#define DECOMPRESS_INCLUDED

#include <assert.h>
#include <stdlib.h>
#include <mem.h>
#include "pnm.h"
#include "stdio.h"
#include "uarray2.h"
#include "arith40.h"
#include "structs.h"
#include "math.h"

/* modules */
Pnm_ppm cvc_to_ppm(UArray2_T arr, A2Methods_T methods);
UArray2_T dequantize(UArray2_T quant_arr);
UArray2_T inverse_dct(UArray2_T dct_arr);

/* helper functions */
void check_bound(int col, int row, UArray2_T cvc_arr);
float clamps(float n, float lower, float higher);
float dequantize_a(unsigned a);
float dequantize_bcd(signed n);

/* mapping functions */
void apply_cvc_to_unsigned(int i, int j, UArray2_T array2, 
                            void *elem, void *cl);
void apply_dequant_chroma(int i, int j, UArray2_T array2, void *elem, 
                            void *cl);
void apply_dequant_dct(int i, int j, UArray2_T array2, void *elem, 
                            void *cl);
void apply_inverse_dct(int i, int j, UArray2_T array2, void *elem, 
                            void *cl);
void apply_dequant(int col, int row, UArray2_T quant_arr, void *elem, 
                            void *cl);


/* test functions */
void apply_print_decomp(int i, int j, UArray2_T array2, void *elem, void *cl);

#endif