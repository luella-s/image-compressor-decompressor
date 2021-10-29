/**************************************************************
 *
 *                     compress40.c
 *
 *     Assignment: COMP40 Homework 4 - arith
 *     Authors:  Luella Sugiman (lsugim01), Yingyang Liang (yliang03)
 *     Date:     10/28/21
 *
 *     Executes compress or decompress on a given image file, represented
 *     by an open file pointer.
 *
 **************************************************************/

#include "compress40.h"
#include "pnm.h"
#include "rgb_cvc.h"
#include "cvc_dct.h"
#include "dct_quant.h"
#include "codeword.h"
#include "print_read.h"
#include "a2methods.h"
#include "a2plain.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    open file pointer to a PPM image.
 * Purpose: compresses a PPM image, given through a file pointer,
    and prints the compressed binary file to standard output.
 * Error cases:
    if any (pointers to) objects returned from a function are NULL,
    we exit with CRE.  
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void compress40(FILE *input)
{
    assert(input != NULL);

    /* create A2Methods suite */
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods != NULL);

    /* read PPM image */
    assert(input != NULL);
    Pnm_ppm image = Pnm_ppmread(input, methods);

    /* convert to scaled float then into component video color space format */
    assert(image != NULL);
    UArray2_T cvc_arr = convert_to_cvc(image);

    /* convert chrome values & cosine coeffecients */
    assert(cvc_arr != NULL);
    UArray2_T dct_arr = dct(cvc_arr);

    /* quantize values */
    assert(dct_arr != NULL);
    UArray2_T quant_arr = quantize(dct_arr);

    /* bitpack */
    assert(quant_arr != NULL);
    UArray2_T codeword_arr = encode(quant_arr);

    /* print to stdout */
    assert(codeword_arr != NULL);
    print_binary(codeword_arr);

    /* free dynamically allocated memory */
    Pnm_ppmfree(&image);
    UArray2_free(&cvc_arr);
    UArray2_free(&dct_arr);
    UArray2_free(&quant_arr);
    UArray2_free(&codeword_arr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    open file pointer to a binary file of the compressed image.
 * Purpose: decompresses an image represented as a binary file, given 
    through a file pointer, and prints the resulting PPM image
    to standard output.
 * Error cases:
    if any (pointers to) objects returned from a function are NULL,
    we exit with CRE.  
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void decompress40(FILE *input)
{
    /* read binary file */
    assert(input != NULL);
    UArray2_T codeword_arr = read_binary(input);

    /* unpack 32-bit codeword */
    assert(codeword_arr != NULL);
    UArray2_T inverse_quant_arr = decode(codeword_arr);

    /* revert from quantized values to dct values */
    assert(inverse_quant_arr != NULL);
    UArray2_T inverse_dct_arr = dequantize(inverse_quant_arr);

    /* perform inverse discrete cosine transformation */
    assert(inverse_dct_arr != NULL);
    UArray2_T inverse_cvc_arr = inverse_dct(inverse_dct_arr);

    /* create A2Methods suite */
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods != NULL);

    /* revert from component-video to RGB representation (stored in PPM) */
    assert(inverse_cvc_arr != NULL);
    Pnm_ppm decomp = cvc_to_ppm(inverse_cvc_arr, methods);

    /* print PPM image to standard out */
    assert(decomp != NULL);
    Pnm_ppmwrite(stdout, decomp);

    /* free dynamically allocated memory */
    Pnm_ppmfree(&decomp);
    UArray2_free(&inverse_dct_arr);
    UArray2_free(&inverse_cvc_arr);
    UArray2_free(&inverse_quant_arr);
    UArray2_free(&codeword_arr);
}