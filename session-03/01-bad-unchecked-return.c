/* BAD: nothing is checked. fopen fails, f is NULL, and fgets walks into it. */
#include <stdio.h>

int main(void)
{
    FILE *f = fopen("does-not-exist.txt", "r");

    char line[128];
    fgets(line, sizeof line, f);   /* f is NULL -> undefined behaviour */
    printf("read: %s", line);

    return 0;
}
