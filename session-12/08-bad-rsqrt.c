/* 08-bad -- the Quake III fast inverse square root, exactly as shipped (1999).
 *
 * The idea is brilliant: a float's bit pattern, read as an integer, is
 * roughly a scaled logarithm of the float. Shift right = halve the log =
 * square root; subtract from a magic constant = negate = reciprocal.
 *
 * The CODE is undefined behaviour twice over on a 2026 machine:
 *   1. *(long *)&y reads a float through a long pointer: strict-aliasing UB.
 *      The optimiser is allowed to assume that never happens.
 *   2. long was 4 bytes in 1999. On 64-bit Linux and macOS it is 8, so this
 *      reads 4 bytes of whatever sits next to `y` on the stack.
 */
#include <math.h>
#include <stdio.h>

static float Q_rsqrt(float number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = *(long *)&y;                       // evil floating point bit level hacking
    i  = 0x5f3759df - (i >> 1);             // what the f***?
    y  = *(float *)&i;
    y  = y * (threehalfs - (x2 * y * y));   // 1st iteration of Newton's method

    return y;
}

int main(void)
{
    printf("sizeof(long) = %zu, sizeof(float) = %zu\n\n", sizeof(long), sizeof(float));

    float inputs[] = {0.25f, 1.0f, 2.0f, 10.0f, 100.0f};
    for (size_t k = 0; k < sizeof inputs / sizeof inputs[0]; k++) {
        float v = inputs[k];
        printf("x = %7.2f   Q_rsqrt = %-12g   1/sqrtf = %g\n",
               (double)v, (double)Q_rsqrt(v), (double)(1.0f / sqrtf(v)));
    }
    return 0;
}
