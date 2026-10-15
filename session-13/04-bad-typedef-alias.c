/* 04-bad -- typedef makes a new NAME, not a new TYPE.
 *
 * Meters and Seconds are both just double. The compiler sees no difference,
 * so adding a distance to a time compiles cleanly. This is the bug class
 * that destroyed the Mars Climate Orbiter in 1999 (pound-seconds handed to
 * code expecting newton-seconds).
 */
#include <stdio.h>

typedef double Meters;
typedef double Seconds;

static Meters walk(Meters start, Meters step)
{
    return start + step;
}

int main(void)
{
    Meters  position = 100.0;
    Seconds elapsed  = 30.0;

    position = walk(position, elapsed);     /* a time, passed as a distance */

    printf("position = %.1f m   <-- 130 \"meters\", after adding 30 seconds\n",
           position);
    return 0;
}
