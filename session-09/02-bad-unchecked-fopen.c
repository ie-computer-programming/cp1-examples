/* 02-bad -- fopen can fail, and returns NULL when it does.
 *
 * Python raises FileNotFoundError and your program stops at the line that
 * caused it. C hands you NULL and carries on. The crash happens later, in
 * fgets, with no indication of which file was missing.
 */
#include <stdio.h>

int main(void)
{
    FILE *f = fopen("does-not-exist.txt", "r");
    /* no check */

    char line[128];
    fgets(line, sizeof line, f);     /* undefined behaviour: f is NULL */

    printf("read: %s", line);
    return 0;
}
