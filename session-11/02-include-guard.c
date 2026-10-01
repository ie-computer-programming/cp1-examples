/* 02 -- why every header needs an include guard.
 *
 * #include is literally "paste this file here". Include a header twice and
 * you declare everything in it twice -- and `struct point` cannot be defined
 * twice in the same translation unit.
 *
 * In a real project you rarely include a header twice ON PURPOSE. It happens
 * because a.h includes c.h, b.h includes c.h, and you include both.
 *
 * The guard makes the second and later copies expand to nothing.
 * `#pragma once` does the same thing in one line -- non-standard, but
 * supported by every compiler you will meet.
 */
#include <stdio.h>

#include "point.h"
#include "point.h"      /* harmless: the guard swallows this one */

int main(void)
{
    struct point p = {.x = 3, .y = 4};

    printf("point_sum = %d\n", point_sum(p));
    printf("included point.h twice, compiled anyway\n");

    return 0;
}
