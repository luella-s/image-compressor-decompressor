#include <assert.h>

#include "pnm.h"
#include "stdio.h"
#include "math.h"
#include "uarray2.h"
#include "a2methods.h"

// UArray2_T convert_to_scaled(Pnm_ppm img);
UArray2_T convert_to_scaled_float(Pnm_ppm img);
void print_compressed(UArray2_T arr);
void apply_print(int i, int j, UArray2_T array2, 
                                      void *elem, void *cl);