/* 07 -- Morton codes (Z-order): turning a 2D point into one number such that
 * points close together in space usually get numbers close together.
 *
 * Interleave the bits:  x = x3 x2 x1 x0,  y = y3 y2 y1 y0
 *                code = y3 x3 y2 x2 y1 x1 y0 x0
 *
 * Sort points by code and nearby points land near each other in memory --
 * the basis of quadtrees, spatial hashing in game physics, and the tile order
 * GPUs use for textures. No multiplication, no division: shifts and masks.
 */
#include <stdint.h>
#include <stdio.h>

#include "bits.h"

/* Spread the low 16 bits of v so there is a zero between every pair:
 * ...dcba  ->  ...0d0c0b0a. Each step halves the gap between groups. */
static uint32_t spread_bits(uint32_t v)
{
    v &= 0x0000FFFFu;
    v = (v | (v << 8)) & 0x00FF00FFu;
    v = (v | (v << 4)) & 0x0F0F0F0Fu;
    v = (v | (v << 2)) & 0x33333333u;
    v = (v | (v << 1)) & 0x55555555u;
    return v;
}

static uint32_t morton(uint32_t x, uint32_t y)
{
    return spread_bits(x) | (spread_bits(y) << 1);
}

int main(void)
{
    printf("x = 5 -> ");  print_bits(5, 4);  printf("\n");
    printf("y = 3 -> ");  print_bits(3, 4);  printf("\n");
    printf("code  -> ");  print_bits(morton(5, 3), 8);
    printf("  (%u)\n\n", morton(5, 3));

    /* The Z: follow the numbers 0,1,2,3,... across the grid. */
    printf("Morton codes on a 4x4 grid (y down, x across):\n\n");
    for (uint32_t y = 0; y < 4; y++) {
        for (uint32_t x = 0; x < 4; x++) {
            printf("%4u", morton(x, y));
        }
        printf("\n");
    }
    return 0;
}
