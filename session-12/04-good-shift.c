/* 04-good -- shift UNSIGNED values, by LESS than their width.
 *
 * Use a fixed-width unsigned type, and check the shift amount when it comes
 * from outside. For bit 31 and beyond, start from a wider type.
 */
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int last = (argc > 1) ? atoi(argv[1]) : 32;

    for (int n = 28; n <= last; n++) {
        if (n < 0 || n >= 32) {
            printf("n = %2d   refused: a 32-bit value has bits 0..31\n", n);
            continue;
        }
        uint32_t mask = UINT32_C(1) << n;
        printf("n = %2d   mask = 0x%08" PRIx32 "\n", n, mask);
    }

    uint64_t wide = UINT64_C(1) << 32;     /* fine: 64-bit type, shift < 64 */
    printf("UINT64_C(1) << 32 = 0x%016" PRIx64 "\n", wide);
    return 0;
}
