/* ASan: writing past the end of a buffer.
 * This is the HW1 `cp1_copy_bounded` lesson -- strcpy does not know how big
 * the destination is, and C will not stop it.
 *
 *   cc -g -fsanitize=address -o demo 01-asan-buffer-overflow.c && ./demo
 */
#include <stdio.h>
#include <string.h>

int main(void)
{
    char small[8];

    /* 20 characters plus a '\0' into 8 bytes of storage. */
    strcpy(small, "far too long for this");

    printf("%s\n", small);
    return 0;
}
