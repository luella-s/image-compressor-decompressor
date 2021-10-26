#include "bitpack.h"
#include <stdio.h>

void test_fitsu()
{
    fprintf(stderr, "value 8 fits in 2 (unsigned): %d\n", Bitpack_fitsu(8, 2));

    fprintf(stderr, "value 8 fits in 3 (unsigned): %d\n", Bitpack_fitsu(8, 3));

    fprintf(stderr, "value 8 fits in 4 (unsigned): %d\n", Bitpack_fitsu(8, 4));

    fprintf(stderr, "value 7 fits in 3 (unsigned): %d\n", Bitpack_fitsu(7, 3));

    fprintf(stderr, "value -3 fits in 3 (unsigned): %d\n", Bitpack_fitsu(-3, 3));
    fprintf(stderr, "value 100 fits in 0 (unsigned): %d\n", Bitpack_fitsu(-3, 3));
    fprintf(stderr, "value 0 fits in 0 (unsigned): %d\n", Bitpack_fitsu(-3, 3));
    fprintf(stderr, "value -3 fits in -23 (unsigned): %d\n", Bitpack_fitsu(-3, 3));

    fprintf(stderr, "value UINT64_MAX fits in 64 (unsigned): %d\n", Bitpack_fitsu(UINT64_MAX, 64));
    fprintf(stderr, "value UINT64_MAX fits in 63 (unsigned): %d\n", Bitpack_fitsu(UINT64_MAX, 63));
    fprintf(stderr, "value UINT64_MAX fits in 64.4 (unsigned): %d\n", Bitpack_fitsu(UINT64_MAX, 64.4));
}

void test_fitss()
{
    fprintf(stderr, "value 4 fits in 3 (signed): %d\n", Bitpack_fitss(4, 3));

    fprintf(stderr, "value -4 fits in 3 (signed): %d\n", Bitpack_fitss(-4, 3));

    fprintf(stderr, "value 8 fits in 4 (signed): %d\n", Bitpack_fitss(8, 4));

    fprintf(stderr, "value 7 fits in 3 (signed): %d\n", Bitpack_fitss(7, 3));
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    /* test bitpack_fits */
    test_fitsu();
    test_fitss();
    
    return 0;
}