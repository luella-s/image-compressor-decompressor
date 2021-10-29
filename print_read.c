/**************************************************************
 *
 *                     print_read.c
 *
 *     Assignment: COMP40 Homework 4 - arith
 *     Authors:  Luella Sugiman (lsugim01), Yingyang Liang (yliang03)
 *     Date:     10/28/21
 *
 *     Implementation of print_read module - implements functions
 *     that prints a UArray2 of codewords to stdout and reads a binary
 *     file of codewords into a UArray2
 *
 **************************************************************/

#include "print_read.h"

#define WORD_SIZE 32

/* * * * * Print UArray2 of codewords to standard output * * * * */

void apply_print(int col, int row, UArray2_T array2, void *elem, void *cl);
void index_convert(UArray2_T array2, unsigned index, int *col, int *row);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    UArray2 of 32-bit codewords.
 * Purpose: Prints a UArray2 of 32-bit codewords to standard out
 * Error cases:
    if 'coded_arr' passed in is a NULL pointer, we exit with CRE.
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void print_binary(UArray2_T coded_arr)
{
    assert(coded_arr != NULL);

    /* print header of image */
    unsigned width = UArray2_width(coded_arr);
    unsigned height = UArray2_height(coded_arr);
    printf("COMP40 Compressed image format 2\n%u %u\n", width, height);

    /* print raster of image */
    UArray2_map_row_major(coded_arr, apply_print, NULL);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    column index,
    row index,
    UArray2 being mapped through,
    element in array2[col][row],
    closure argument (NULL).
 * Purpose: prints codeword in big-Endian.
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void apply_print(int col, int row, UArray2_T array2, void *elem, void *cl)
{
    (void) col;
    (void) row;
    (void) array2;
    (void) cl;

    uint32_t word = *(uint32_t *)elem;
    unsigned lsb;

    /* print codeword in big-Endian */
    for (int i = (WORD_SIZE / 8 - 1); i >= 0; i--) {
        lsb = i * 8;
        uint64_t field = Bitpack_getu(word, 8, lsb);
        putchar(field);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    UArray2 being accessed,
    index to convert,
    column index to return,
    row index to return.
 * Purpose: converts 1D array index to a 2D array index.
 * Error cases:
    if 'array2' passed in is a NULL pointer, we exit with CRE.
 * Returns: nothing (changes passed by reference).
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void index_convert(UArray2_T array2, unsigned index, int *col, int *row)
{
    assert(array2 != NULL);

    int width = UArray2_width(array2);
    int height = UArray2_height(array2);
    int i = index % width;
    int j = index / width;

    /* Assert that index given is within 2D array dimensions */
    assert((i < width) && (i >= 0) && (j < height) && (j >= 0));

    /* update col and row values */
    *col = i;
    *row = j;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    open file pointer to compressed binary image file.
 * Purpose: reads image file codewords into a UArray2.
 * Error cases:
    if file pointer passed in is a NULL pointer, we exit with CRE.
 * Returns: UArray2 of 32-bit codewords.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UArray2_T read_binary(FILE *fp)
{
    assert(fp != NULL);

    /* retrieve height & width from file header */
    unsigned height, width;
    int read = fscanf(fp, "COMP40 Compressed image format 2\n%u %u", 
                      &width, &height);
    assert(read == 2);
    int c = getc(fp);
    assert(c == '\n');

    int col = 0;
    int row = 0;
    unsigned index = 0;
    uint64_t word = 0;

    /* make UArray2 of 32-bit codewords */
    UArray2_T coded_arr = UArray2_new(width, height, sizeof(uint32_t));
    assert(coded_arr != NULL);

    c = getc(fp);
    while (c != EOF && index < (width * height)) {
        /* read 32-bit word into big-Endian format */
        for (int i = (WORD_SIZE / 8 - 1); i >= 0; i--) {
            word = Bitpack_newu(word, 8, (i * 8), (uint64_t)c);
            c = getc(fp);
        }
        /* store codeword into UArray2 */
        index_convert(coded_arr, index, &col, &row);
        *((uint32_t *)UArray2_at(coded_arr, col, row)) = word;
        index++;
    }

    /* ensure raster provides enough pixels */
    assert(index >= (width * height));
    return coded_arr;
}