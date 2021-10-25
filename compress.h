#ifndef COMPRESS_INCLUDED
#define COMPRESS_INCLUDED

#include <assert.h>
#include "pnm.h"
#include "stdio.h"
#include "math.h"
#include "uarray2.h"
#include "a2methods.h"
#include "arith40.h"
#include "structs.h"
#include <stdlib.h>

/* modules */
UArray2_T convert_to_cvc(Pnm_ppm img);
UArray2_T dct(UArray2_T cvc_arr);
UArray2_T quantize(UArray2_T dct_arr);

/* helper functions */
void trim_ppm(Pnm_ppm img);
void check_bounds(int col, int row, UArray2_T cvc_arr);
float clamp(float num, float lower, float upper);
float avg_pb(int col, int row, UArray2_T cvc_arr);
float avg_pr(int col, int row, UArray2_T cvc_arr);
signed quantize_bcd(unsigned n);

/* mapping functions */
void unsigned_to_cvc(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl);
void apply_dct(int i, int j, UArray2_T array2, void *elem, void *cl);
void apply_quantize(int i, int j, UArray2_T array2, void *elem, void *cl);

/* test functions */
void print_compressed(UArray2_T arr);
void apply_print(int i, int j, UArray2_T array2, void *elem, void *cl);
void print_chroma(int col, int row, UArray2_T array2, void *elem, void *cl);

#endif