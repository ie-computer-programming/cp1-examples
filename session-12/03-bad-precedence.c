/* 03-bad -- == binds tighter than &.
 *
 *     if (flags & MASK == 0)     is parsed as     if (flags & (MASK == 0))
 *
 * MASK == 0 is 0, flags & 0 is 0, so the test is ALWAYS false -- for every
 * value of flags. It compiles, and -Wall does warn.
 */
#include <stdio.h>

#define READY 0x1u

int main(void)
{
    unsigned flags = 0x6;   /* 0110: the READY bit is clear */

    if (flags & READY == 0) {
        printf("not ready -- waiting\n");
    } else {
        printf("ready -- starting   <-- wrong, the bit is clear\n");
    }
    return 0;
}
