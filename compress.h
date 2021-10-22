#include <assert.h>

#include "pnm.h"
#include "stdio.h"
#include "math.h"
#include "uarray2.h"
#include "a2methods.h"
#include <stdlib.h>

void trim_ppm(Pnm_ppm img);
UArray2_T convert_to_scaled_float(Pnm_ppm img);
void print_compressed(UArray2_T arr);
void apply_print(int i, int j, UArray2_T array2, void *elem, void *cl);
void unsigned_to_float(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl);