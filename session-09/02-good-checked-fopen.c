/* 02-good -- check the return value, then say what went wrong.
 *
 * This is Session 3's sentinel-return rule applied to files: fopen returns
 * NULL on failure and sets errno to say why. perror turns errno into the
 * same sentence the shell would print.
 */
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    const char *path = "does-not-exist.txt";

    FILE *f = fopen(path, "r");
    if (f == NULL) {
        /* Read errno IMMEDIATELY -- the next library call may overwrite it. */
        fprintf(stderr, "could not open %s: %s\n", path, strerror(errno));
        perror(path);
        return 1;
    }

    char line[128];
    if (fgets(line, sizeof line, f) != NULL) {
        printf("read: %s", line);
    }

    fclose(f);
    return 0;
}
