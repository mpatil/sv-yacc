#include "defs.h"

void transitive_closure(unsigned *R, int n)
{
    int rowsize;
    unsigned mask;
    unsigned *rowj;
    unsigned *rp;
    unsigned *rend;
    unsigned *ccol;
    unsigned *relend;
    unsigned *cword;
    unsigned *rowi;

    rowsize = WORDSIZE(n);
    relend = R + n * rowsize;

    cword = R;
    mask = 1;
    rowi = R;
    while (rowi < relend) {
        ccol = cword;
        rowj = R;

        while (rowj < relend) {
            if (*ccol & mask) {
                rp = rowi;
                rend = rowj + rowsize;
                while (rowj < rend)
                    *rowj++ |= *rp++;
            } else {
                rowj += rowsize;
            }

            ccol += rowsize;
        }

        mask <<= 1;
        if (mask == 0) {
            mask = 1;
            cword++;
        }

        rowi += rowsize;
    }
}

void reflexive_transitive_closure(unsigned *R, int n)
{
    int rowsize;
    unsigned mask;
    unsigned *rp;
    unsigned *relend;

    transitive_closure(R, n);

    rowsize = WORDSIZE(n);
    relend = R + n * rowsize;

    mask = 1;
    rp = R;
    while (rp < relend) {
        *rp |= mask;
        mask <<= 1;
        if (mask == 0) {
            mask = 1;
            rp++;
        }

        rp += rowsize;
    }
}
