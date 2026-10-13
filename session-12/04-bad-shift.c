/* 04-bad -- two shifts that are undefined behaviour, and look fine.
 *
 * 1. Shifting a 1 into the sign bit of a signed int:   1 << 31
 * 2. Shifting by >= the width of the type:            1u << 32
 *
 * The shift amount comes from the command line, so the compiler cannot see
 * it and does not warn. On x86 the CPU masks the shift count to 5 bits, so
 * 1u << 32 quietly becomes 1u << 0.
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int last = (argc > 1) ? atoi(argv[1]) : 32;

    for (int n = 28; n <= last; n++) {             /* <= : one too many */
        unsigned mask = 1u << n;
        int      sbit = 1 << n;
        printf("n = %2d   1u << n = 0x%08x   1 << n = %d\n", n, mask, sbit);
    }
    return 0;
}
