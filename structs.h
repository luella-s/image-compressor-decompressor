#ifndef STRUCTS_INCLUDED
#define STRUCTS_INCLUDED

typedef struct Cvc {
    float y;
    float pb;
    float pr;
} *Cvc;

typedef struct Dct_float {
    float a;
    float b;
    float c;
    float d;
    float avg_pb;
    float avg_pr;
} *Dct_float;

typedef struct Quant {
    unsigned scaled_a;
    signed scaled_b;
    signed scaled_c;
    signed scaled_d;
    unsigned ind_pb;
    unsigned ind_pr;
} *Quant;

#endif