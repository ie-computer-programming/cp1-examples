/* Shared helper: print the low `bits` bits of v, most significant first. */
#ifndef CP1_BITS_H
#define CP1_BITS_H

#include <stdint.h>
#include <stdio.h>

static inline void print_bits(uint32_t v, int bits)
{
    for (int i = bits - 1; i >= 0; i--) {
        putchar((v >> i) & 1u ? '1' : '0');
        if (i % 4 == 0 && i != 0) {
            putchar('_');
        }
    }
}

#endif /* CP1_BITS_H */
