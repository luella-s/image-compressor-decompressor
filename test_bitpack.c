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

    fprintf(stderr, "UINT64_MAX is %lu\n", UINT64_MAX);
    uint64_t max = ~0;
    fprintf(stderr, "bitpack max is %lu\n", max);
    fprintf(stderr, "value UINT64_MAX - 1 fits in 64 (unsigned): %d\n", Bitpack_fitsu(UINT64_MAX - 1, 64));

    fprintf(stderr, "value UINT64_MAX fits in 64 (unsigned): %d\n", Bitpack_fitsu(UINT64_MAX, 64));

    fprintf(stderr, "value UINT64_MAX + 1 fits in 64 (unsigned): %d\n", Bitpack_fitsu((UINT64_MAX + 1), 64));

    fprintf(stderr, "value UINT64_MAX + 2 fits in 64 (unsigned): %d\n", Bitpack_fitsu((UINT64_MAX + 2), 64));

    fprintf(stderr, "value UINT64_MAX + 3 fits in 64 (unsigned): %d\n", Bitpack_fitsu((UINT64_MAX + 3), 64));

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
    uint64_t a = Bitpack_getu(0x4787A267, 8, 24);
    uint64_t b = Bitpack_getu(0x4787A267, 8, 16);
    uint64_t c = Bitpack_getu(0x4787A267, 8, 8);
    uint64_t d = Bitpack_getu(0x4787A267, 8, 0);
    fprintf(stderr, "first 3: %lu\n", first_3);
    fprintf(stderr, "second 3: %lu\n", second_3);
    fprintf(stderr, "expected a: %d output: %lu\n", 0x47, a);
    fprintf(stderr, "expected b: %d output: %lu\n", 0x87, b);
    fprintf(stderr, "expected c: %d output: %lu\n", 0xA2, c);
    fprintf(stderr, "expected d: %d output: %lu\n", 0x67, d);
}

void test_gets() 
{
    int64_t neg = -25; //1111111111111111111111100111
    int64_t pos = 40; //00101000
    int64_t neg1 = Bitpack_gets(neg, 6, 0); //100111 //-25
    int64_t neg2 = Bitpack_gets(neg, 5, 40); //11111
    int64_t pos1 = Bitpack_gets(pos, 64, 0); //dec is 40
    int64_t pos2 = Bitpack_gets(pos, 63, 1); //00010100
    int64_t pos3 = Bitpack_gets(pos, 4, 2); //00010100
    fprintf(stderr, "neg1: %ld\n", neg1);
    fprintf(stderr, "neg2: %ld\n", neg2);
    fprintf(stderr, "pos1: %ld\n", pos1);
    fprintf(stderr, "pos2: %ld\n", pos2);
    fprintf(stderr, "pos3: %ld\n", pos3);

}

void test_newu()
{
    uint64_t one = Bitpack_newu(15, 2, 0, 2);
    fprintf(stderr, "Expected: 14 Output: %lu\n", one);
    uint64_t two = Bitpack_newu(100, 3, 2, 0);
    fprintf(stderr, "Expected: 96 Output: %lu\n", two);
    uint64_t three = Bitpack_newu(456789766165, 5, 5, 31);
    fprintf(stderr, "Expected: 456789767157 Output: %lu\n", three);
    
}

void test_news()
{
    //uint64_t word, unsigned width, unsigned lsb,  int64_t value
    uint64_t pos = 31;
    uint64_t neg = ~0;
    uint64_t news_pos1 = Bitpack_news(pos, 3, 5, 3); //should be 0111 1111
    fprintf(stderr, "Expected: 127 Output: %lu\n", news_pos1);
    uint64_t news_pos2 = Bitpack_news(pos, 3, 5, -4); //should be 1001 1111
    fprintf(stderr, "Expected: 159 Output: %lu\n", news_pos2);
    int64_t news_neg1 = Bitpack_news(neg, 4, 1, -8); //should be 111...10001
    fprintf(stderr, "Expected: -15 Output: %ld\n", news_neg1);
    uint64_t news_neg2 = Bitpack_news(neg, 4, 1, -1); //should be 111...11111
    fprintf(stderr, "Expected: %lu Output: %lu\n", UINT64_MAX, news_neg2);
}

void utol()
{
    uint64_t u = 584115552301;
    fprintf(stderr, "unsigned: %lu\n", u);
    int64_t s = u;
    fprintf(stderr, "signed: %ld\n", s);
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    test_fitsu();
    test_fitss();
    test_newu();
    test_news();
    test_getu();
    test_gets();
    utol();

    
    return 0;
}