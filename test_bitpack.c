#include "bitpack.h"
#include <stdio.h>

void test_fitsu()
{
    fprintf(stderr, "value 8 fits in 2 (unsigned): %d\n", Bitpack_fitsu(8, 2));

    fprintf(stderr, "value 8 fits in 3 (unsigned): %d\n", Bitpack_fitsu(8, 3));

    fprintf(stderr, "value 8 fits in 4 (unsigned): %d\n", Bitpack_fitsu(8, 4));

    fprintf(stderr, "value 7 fits in 3 (unsigned): %d\n", Bitpack_fitsu(7, 3));

    fprintf(stderr, "value -3 fits in 3 (unsigned): %d\n", Bitpack_fitsu(-3, 3));
    fprintf(stderr, "value 100 fits in 0 (unsigned): %d\n", Bitpack_fitsu(100, 0));
    //returns true
    fprintf(stderr, "value 0 fits in 0 (unsigned): %d\n", Bitpack_fitsu(0, 0));

    //throws uncaught exception
    // fprintf(stderr, "value -3 fits in -23 (unsigned): %d\n", Bitpack_fitsu(-3, -23));

    fprintf(stderr, "value UINT64_MAX fits in 64 (unsigned): %d\n", Bitpack_fitsu(UINT64_MAX, 64));

    fprintf(stderr, "value UINT64_MAX fits in 63 (unsigned): %d\n", Bitpack_fitsu(UINT64_MAX, 63));

    fprintf(stderr, "value UINT64_MAX fits in 64.4 (unsigned): %d\n", Bitpack_fitsu(UINT64_MAX, 64.4));
}

void test_fitss()
{
    fprintf(stderr, "value 4 fits in 3 (signed): %d\n", Bitpack_fitss(4, 3));

    fprintf(stderr, "value 3 fits in 3 (signed): %d\n", Bitpack_fitss(3, 3));

    fprintf(stderr, "value -4 fits in 3 (signed): %d\n", Bitpack_fitss(-4, 3));

    fprintf(stderr, "value 8 fits in 4 (signed): %d\n", Bitpack_fitss(8, 4));

    fprintf(stderr, "value 7 fits in 4 (signed): %d\n", Bitpack_fitss(7, 4));

    fprintf(stderr, "value -8 fits in 4 (signed): %d\n", Bitpack_fitss(-8, 4));

    fprintf(stderr, "value 7 fits in 3 (signed): %d\n", Bitpack_fitss(7, 3));

    fprintf(stderr, "value INT64_MAX fits in 64 (signed): %d\n", Bitpack_fitss(INT64_MAX, 64));

    fprintf(stderr, "value INT64_MIN fits in 64 (signed): %d\n", Bitpack_fitss(INT64_MIN, 64));
}

void test_getu()
{
    uint64_t num = 37; //100101
    uint64_t first_3 = Bitpack_getu(num, 3, 3); //4
    uint64_t second_3 = Bitpack_getu(num, 3, 0); //5
    fprintf(stderr, "first 3: %lu\n", first_3);
    fprintf(stderr, "second 3: %lu\n", second_3);
}

void test_gets() 
{
    // int64_t neg = -25; //1111111111111111111111100111
    int64_t pos = 40; //00101000
    // int64_t neg1 = Bitpack_gets(neg, 6, 0); //100111 //-25
    // int64_t neg2 = Bitpack_gets(neg, 5, 40); //11111
    // int64_t pos1 = Bitpack_gets(pos, 64, 0); //dec is 40
    // int64_t pos2 = Bitpack_gets(pos, 63, 1); //00010100
    // int64_t pos3 = Bitpack_gets(pos, 4, 2); //00010100
    // fprintf(stderr, "neg1: %ld\n", neg1);
    // fprintf(stderr, "neg2: %ld\n", neg2);
    // fprintf(stderr, "pos1: %ld\n", pos1);
    // fprintf(stderr, "pos2: %ld\n", pos2);
    // fprintf(stderr, "pos3: %ld\n", pos3);

}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    /* test bitpack_fits */
    // test_fitsu();
    // test_fitss();
    // test_getu();
    test_gets();
    
    return 0;
}