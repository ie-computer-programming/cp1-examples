/* GOOD: snprintf always takes the size, and always terminates.
 *
 * It returns the length it WANTED to write, so comparing that against the
 * buffer size tells you whether the result was truncated -- a check strncpy
 * cannot give you (and strncpy does not even guarantee a terminator).
 */
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    char small[8];
    char after[8] = "SAFE";

    /* Read from the command line, so try your own:  ./07-good-snprintf hello
     * (argv[0], the program path, is the default -- and it keeps the length
     * unknown at compile time, which is the honest case this code is for.) */
    const char *name = argv[(argc > 1) ? 1 : 0];

    int wanted = snprintf(small, sizeof small, "%s", name);

    printf("small  = %s\n", small);
    printf("after  = %s  <-- untouched\n", after);

    if (wanted < 0) {
        fprintf(stderr, "snprintf failed\n");
        return 1;
    }
    if ((size_t)wanted >= sizeof small) {
        printf("truncated: needed %d bytes, had %zu\n", wanted + 1, sizeof small);
    } else {
        printf("fitted with room to spare\n");
    }
    return 0;
}
