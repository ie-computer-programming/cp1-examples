/* 08-good -- the same algorithm, written legally.
 *
 * memcpy between a float and a uint32_t is the defined way to reinterpret
 * bits. It looks like a function call; every compiler turns it into a single
 * register move. uint32_t is exactly 32 bits on every platform, unlike long.
 */
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static float fast_rsqrt(float number)
{
    uint32_t i;
    float y = number;

    memcpy(&i, &y, sizeof i);
    i = 0x5f3759dfu - (i >> 1);
    memcpy(&y, &i, sizeof y);

    return y * (1.5f - 0.5f * number * y * y);   /* one Newton step */
}

int main(void)
{
    _Static_assert(sizeof(float) == sizeof(uint32_t), "float must be 32 bits");

    float inputs[] = {0.25f, 1.0f, 2.0f, 10.0f, 100.0f};
    for (size_t k = 0; k < sizeof inputs / sizeof inputs[0]; k++) {
        float v = inputs[k];
        printf("x = %7.2f   fast = %-10g   exact = %g\n",
               (double)v, (double)fast_rsqrt(v), (double)(1.0f / sqrtf(v)));
    }

    /* Worst relative error over a wide range. */
    double worst = 0.0;
    float  at = 0.0f;
    for (float v = 0.001f; v < 1000.0f; v *= 1.001f) {
        double exact = 1.0 / sqrt((double)v);
        double err = fabs((double)fast_rsqrt(v) - exact) / exact;
        if (err > worst) {
            worst = err;
            at = v;
        }
    }
    printf("\nworst relative error over [0.001, 1000): %.4f%% (at x = %g)\n",
           worst * 100.0, (double)at);
    return 0;
}
