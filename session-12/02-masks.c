/* 02 -- the four mask idioms. Learn these four lines and you can read any
 * driver, protocol parser or file-format decoder ever written in C.
 *
 *   set     x |=  (1u << n)
 *   clear   x &= ~(1u << n)
 *   toggle  x ^=  (1u << n)
 *   test    (x >> n) & 1u      or     (x & (1u << n)) != 0
 */
#include <stdint.h>
#include <stdio.h>

#include "bits.h"

static void show(const char *label, uint32_t v)
{
    printf("%-22s ", label);
    print_bits(v, 8);
    printf("\n");
}

int main(void)
{
    uint32_t x = 0;

    show("start", x);
    x |= (1u << 3);   show("set bit 3", x);
    x |= (1u << 0);   show("set bit 0", x);
    x ^= (1u << 7);   show("toggle bit 7", x);
    x &= ~(1u << 3);  show("clear bit 3", x);
    x ^= (1u << 7);   show("toggle bit 7 again", x);

    printf("\nbit 0 is %s\n", ((x >> 0) & 1u) ? "set" : "clear");
    printf("bit 3 is %s\n", ((x >> 3) & 1u) ? "set" : "clear");

    /* A mask of the low n bits: (1u << n) - 1.  n = 4 -> 0000_1111 */
    show("\nlow-4-bits mask", (1u << 4) - 1);
    return 0;
}
