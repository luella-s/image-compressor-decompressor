/**************************************************************
 *
 *                     dct_quant.h
 *
 *     Assignment: COMP40 Homework 4 - arith
 *     Authors:  Luella Sugiman (lsugim01), Yingyang Liang (yliang03)
 *     Date:     10/28/21
 *
 *     Interface of dct_quant - defines public functions of the 
 *     dct_quant module.
 *
 **************************************************************/

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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Purpose: stores quantized values of a block of 2x2 pixels.
 * Contents: 
    scaled integers of a, b, c and d
    index of average Pb and Pr
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
typedef struct Quant {
    unsigned scaled_a;
    signed scaled_b;
    signed scaled_c;
    signed scaled_d;
    unsigned ind_pb;
    unsigned ind_pr;
} *Quant;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    UArray2 of Dct structs
 * Purpose: Compresses a UArray2 of Dct structs into a UArray2 of Quant structs
 * Returns: UArray2 of Quant structs.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UArray2_T quantize(UArray2_T dct_arr);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    UArray2 of Quant structs
 * Purpose: Decompresses a UArray2 of Quant structs into a UArray2 of Dct
    structs
 * Returns: UArray2 of Dct structs.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UArray2_T dequantize(UArray2_T quant_arr);

#endif