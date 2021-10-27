#ifndef RGB_CVC_INCLUDED
#define RGB_CVC_INCLUDED

#include <assert.h>
#include <stdlib.h>
#include "pnm.h"
#include "stdio.h"
#include "uarray2.h"
#include "a2methods.h"
#include "structs.h"
#include "helper.h"



UArray2_T convert_to_cvc(Pnm_ppm img);
void trim_ppm(Pnm_ppm img);
void unsigned_to_cvc(int i, int j, A2Methods_UArray2 array2, 
                    void *elem, void *cl);
Pnm_ppm cvc_to_ppm(UArray2_T arr, A2Methods_T methods);
void apply_cvc_to_unsigned(int i, int j, UArray2_T array2, 
                            void *elem, void *cl);

#endif