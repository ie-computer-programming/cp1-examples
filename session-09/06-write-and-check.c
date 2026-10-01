/* 06 -- writing, buffering, and why fclose can fail.
 *
 * fprintf does not write to the disk. It writes into a buffer that the C
 * library flushes later -- when the buffer fills, when you call fflush, or
 * when you call fclose. That is why the LAST write error in a program is
 * usually reported by fclose, not by fprintf.
 *
 * Ignoring the result of fclose after writing means ignoring "the disk was
 * full" for every byte still sitting in the buffer.
 */
#include <stdio.h>

int main(void)
{
    const char *path = "06-output.txt";

    FILE *f = fopen(path, "w");        /* "w" TRUNCATES an existing file */
    if (f == NULL) {
        perror(path);
        return 1;
    }

    for (int i = 1; i <= 3; i++) {
        if (fprintf(f, "line %d\n", i) < 0) {   /* fprintf returns < 0 on error */
            perror("fprintf");
            fclose(f);
            return 1;
        }
    }

    printf("3 lines written -- but possibly still in the buffer\n");

    if (fclose(f) != 0) {              /* the flush happens HERE */
        perror("fclose");
        return 1;
    }
    printf("fclose ok -- now they are really on disk\n");

    FILE *r = fopen(path, "r");
    if (r == NULL) {
        perror(path);
        return 1;
    }
    char line[64];
    while (fgets(line, sizeof line, r) != NULL) {
        printf("  read back: %s", line);
    }
    fclose(r);

    remove(path);
    return 0;
}
