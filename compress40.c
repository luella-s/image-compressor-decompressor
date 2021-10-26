#include "compress40.h"
#include "pnm.h"
#include "rgb_cvc.h"
#include "cvc_dct.h"
#include "dct_quant.h"
#include "a2methods.h"
#include "a2plain.h"

/*

TO ASK TA:

*/

/*
 * The two functions below are functions you should implement.
 * They should take their input from the parameter and should
 *  write their output to stdout
 */

/* reads PPM, writes compressed image */
void compress40(FILE *input)
{
    //TODO
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
    UArray2_T dct_arr = dct(cvc_arr);

    /* quantize values */
    UArray2_T quant_arr = quantize(dct_arr);

    /* bitpack */

    //print to stdout

    /* decompress */
    /* revert from quantized values to dct values */
    UArray2_T inverse_dct_arr = dequantize(quant_arr);

    /* revert from dct to CVC */
    assert(inverse_dct_arr != NULL);
    UArray2_T inverse_cvc_arr = inverse_dct(inverse_dct_arr);

    /* revert from CVC to PPM */
    assert(inverse_cvc_arr != NULL);
    Pnm_ppm decomp = cvc_to_ppm(inverse_cvc_arr, methods);

    assert(decomp != NULL);
    Pnm_ppmwrite(stdout, decomp);

    /* FREE allocated memory*/
    Pnm_ppmfree(&image);
    Pnm_ppmfree(&decomp);
    UArray2_free(&cvc_arr);
    UArray2_free(&dct_arr);
    UArray2_free(&quant_arr);
    UArray2_free(&inverse_dct_arr);
    UArray2_free(&inverse_cvc_arr);
}

/* reads compressed image, writes PPM */
void decompress40(FILE *input)
{
    //TODO 
    //read compressed output
    assert(input != NULL);
    // read_compress(input);
    //convert to RGB
    //convert to unscaled
}