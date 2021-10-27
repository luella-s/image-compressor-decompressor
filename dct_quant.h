#ifndef DCT_QUANT_INCLUDED
#define DCT_QUANT_INCLUDED

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "uarray2.h"
#include "arith40.h"
#include "math.h"
#include "cvc_dct.h"
#include "helper.h"

typedef struct Quant {
    unsigned scaled_a;
    signed scaled_b;
    signed scaled_c;
    signed scaled_d;
    unsigned ind_pb;
    unsigned ind_pr;
} *Quant;

/* Main function */
UArray2_T quantize(UArray2_T dct_arr);
UArray2_T dequantize(UArray2_T quant_arr);

#endif