/* 04-good -- wrap the value in a struct, and you get a REAL new type.
 *
 * Two struct types are never compatible, even with identical members. The
 * mistake from 04-bad becomes a compile error. Cost at runtime: nothing --
 * a one-member struct is passed in the same register as the double.
 */
#include <stdio.h>

typedef struct { double value; } Meters;
typedef struct { double value; } Seconds;

static Meters walk(Meters start, Meters step)
{
    return (Meters){ start.value + step.value };
}

int main(void)
{
    Meters  position = { 100.0 };
    Meters  step     = { 2.5 };
    Seconds elapsed  = { 30.0 };

    position = walk(position, step);
    /* position = walk(position, elapsed);
     *   error: incompatible type for argument 2 of 'walk'
     *   note: expected 'Meters' but argument is of type 'Seconds' */

    printf("position = %.1f m after %.0f s\n", position.value, elapsed.value);
    return 0;
}
