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

typedef struct Dct_float {
    float a;
    float b;
    float c;
    float d;
    float avg_pb;
    float avg_pr;
} *Dct_float;

UArray2_T dct(UArray2_T cvc_arr);
void apply_dct(int i, int j, UArray2_T array2, void *elem, void *cl);
float avg_pb(int col, int row, UArray2_T cvc_arr);
float avg_pr(int col, int row, UArray2_T cvc_arr);

UArray2_T inverse_dct(UArray2_T dct_arr);
void apply_inverse_dct(int i, int j, UArray2_T array2, void *elem, 
                            void *cl);

#endif