/* 01 -- the six bitwise operators, shown in binary.
 *
 * Python has the same operators with the same spelling. The difference is
 * the WIDTH: a Python int has as many bits as it needs, a C uint8_t has 8.
 * Every result below is cut to 8 bits, and ~ and << show it.
 */
#include <stdint.h>
#include <stdio.h>

#include "bits.h"

static void show(const char *label, uint8_t v)
{
    printf("%-8s = ", label);
    print_bits(v, 8);
    printf("  (%3u)\n", v);
}

int main(void)
{
    uint8_t a = 0xCA;   /* 1100_1010 */
    uint8_t b = 0x0F;   /* 0000_1111 */

    show("a", a);
    show("b", b);
    printf("\n");
    show("a & b", a & b);     /* both 1  -> keep only the low nibble */
    show("a | b", a | b);     /* either 1 */
    show("a ^ b", a ^ b);     /* exactly one 1 -> flips the low nibble */
    show("~a", (uint8_t)~a);  /* every bit flipped */
    show("a << 1", (uint8_t)(a << 1));   /* the top bit falls off the end */
    show("a >> 2", a >> 2);   /* zeros come in from the left (unsigned) */

    /* Python: ~0xCA == -203, and 0xCA << 1 == 404. No bits ever fall off. */
    return 0;
}
