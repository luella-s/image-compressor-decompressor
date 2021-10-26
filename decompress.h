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

UArray2_T inverse_dct(UArray2_T dct_arr);

/* helper functions */
void check_bound(int col, int row, UArray2_T cvc_arr);
float clamps(float n, float lower, float higher);


/* mapping functions */


void apply_inverse_dct(int i, int j, UArray2_T array2, void *elem, 
                            void *cl);



/* test functions */
void apply_print_decomp(int i, int j, UArray2_T array2, void *elem, void *cl);

#endif