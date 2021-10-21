#include "compress40.h"
#include "pnm.h"
#include "compress.h"
#include "decompress.h"
#include "a2methods.h"
#include "a2plain.h"

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
    assert(methods);
    //read PPM image
    assert(input != NULL);
    Pnm_ppm image = Pnm_ppmread(input, methods);
    //convert to scaled float - call compress.h function
    UArray2_T arr = convert_to_scaled_float(image);
    //print to stdout
    print_compressed(arr);
}

/* reads compressed image, writes PPM */
void decompress40(FILE *input)
{
    //TODO 
    //read compressed output
    assert(input != NULL);
    read_compress(input);
    //convert to RGB
    //convert to unscaled
}