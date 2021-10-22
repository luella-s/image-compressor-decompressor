#include <assert.h>
#include <stdlib.h>
#include <mem.h>
#include "pnm.h"
#include "stdio.h"
#include "uarray2.h"

UArray2_T read_compress(FILE *input);
Pnm_ppm convert_to_ppm(UArray2_T arr, A2Methods_T methods);
void float_to_unsigned(int i, int j, UArray2_T array2, void *elem, void *cl);
void apply_print2(int i, int j, UArray2_T array2, void *elem, void *cl);