/**************************************************************
 *
 *                     print_read.h
 *
 *     Assignment: COMP40 Homework 4 - arith
 *     Authors:  Luella Sugiman (lsugim01), Yingyang Liang (yliang03)
 *     Date:     10/28/21
 *
 *     Interface of print_read - defines public functions of the 
 *     print_read module.
 *
 **************************************************************/

#ifndef PRINT_READ_INCLUDED
#define PRINT_READ_INCLUDED

#include "dct_quant.h"
#include "bitpack.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    UArray2 of Quant structs
 * Purpose: Prints a UArray2 of 32-bit codewords to standard out
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void print_binary(UArray2_T coded_arr);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    open file pointer to compressed image file.
 * Purpose: Reads a compressed binary file image into a UArray2 of 32-bit
    codewords.
 * Returns: UArray2 of Dct structs.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UArray2_T read_binary(FILE *input);

#endif