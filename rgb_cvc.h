/**************************************************************
 *
 *                     rgb_cvc.h
 *
 *     Assignment: COMP40 Homework 4 - arith
 *     Authors:  Luella Sugiman (lsugim01), Yingyang Liang (yliang03)
 *     Date:     10/28/21
 *
 *     Interface of rgb_cvc - defines public functions of the 
 *     rgb_cvc module.
 *
 **************************************************************/

#ifndef RGB_CVC_INCLUDED
#define RGB_CVC_INCLUDED

#include <assert.h>
#include <stdlib.h>
#include "pnm.h"
#include "stdio.h"
#include "uarray2.h"
#include "a2methods.h"
#include "helper.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Contents:
    values of Y, Pb and Pr
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
typedef struct Cvc {
    float y;
    float pb;
    float pr;
} *Cvc;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    Pnm_ppm of an image file
 * Purpose: Compresses a Pnm_ppm image to a UArray2 of Cvc structs
 * Returns: UArray2 of Cvc structs.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UArray2_T convert_to_cvc(Pnm_ppm img);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    UArray2 of Cvc structs,
    A2Methods methods suite.
 * Purpose: Decompresses a UArray2 of Cvc structs into a Pnm_ppm image
 * Returns: Pnm_ppm of an image file
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Pnm_ppm cvc_to_ppm(UArray2_T arr, A2Methods_T methods);

#endif