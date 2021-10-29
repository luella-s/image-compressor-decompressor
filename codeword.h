/**************************************************************
 *
 *                     codeword.h
 *
 *     Assignment: COMP40 Homework 4 - arith
 *     Authors:  Luella Sugiman (lsugim01), Yingyang Liang (yliang03)
 *     Date:     10/28/21
 *
 *     Interface of codeword module - defines public functions of the 
 *     codeword module.
 *
 **************************************************************/

#ifndef CODEWORD_INCLUDED
#define CODEWORD_INCLUDED

#include "dct_quant.h"
#include "bitpack.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    UArray2 of Quant structs
 * Purpose: Compresses a UArray2 of Quant structs into a UArray2 of 32-bit
    codewords.
 * Returns: UArray2 of 32-bit codewords.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UArray2_T encode(UArray2_T quant_arr);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    UArray2 of 32-bit codewords
 * Purpose: Decompresses a UArray2 of Quant structs into a UArray2 of 
    32-bit codewrods
 * Returns: UArray2 of Quant structs.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UArray2_T decode(UArray2_T codeword_arr);
 
#endif