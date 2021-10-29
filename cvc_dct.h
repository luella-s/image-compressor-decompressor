/**************************************************************
 *
 *                     cvc_dct.h
 *
 *     Assignment: COMP40 Homework 4 - arith
 *     Authors:  Luella Sugiman (lsugim01), Yingyang Liang (yliang03)
 *     Date:     10/28/21
 *
 *     Interface of cvc_dct - defines public functions of the 
 *     cvc_dct module.
 *
 **************************************************************/

#ifndef CVC_DCT_INCLUDED
#define CVC_DCT_INCLUDED

#include <assert.h>
#include <stdlib.h>
#include "pnm.h"
#include "stdio.h"
#include "uarray2.h"
#include "a2methods.h"
#include "rgb_cvc.h"
#include "helper.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Contents:
    unscaled values of a, b, c, d
    average values of Pb and Pr
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
typedef struct Dct_float {
    float a;
    float b;
    float c;
    float d;
    float avg_pb;
    float avg_pr;
} *Dct_float;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
   UArray2 of Cvc structs
 * Purpose: Compresses a UArray2 of Cvc structs into a UArray2 of Dct structs
 * Returns: UArray2 of Dct structs.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UArray2_T dct(UArray2_T cvc_arr);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
   UArray2 of Dct structs
 * Purpose: deompresses a UArray2 of Dct structs into a UArray2 of Cvc structs
 * Returns: UArray2 of Cvc structs.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UArray2_T inverse_dct(UArray2_T dct_arr);

#endif