/* 01 -- the idiomatic way to read a text file line by line.
 *
 * Python:                          C:
 *   with open(path) as f:            FILE *f = fopen(path, "r");
 *       for line in f:               char buf[256];
 *           print(line)              while (fgets(buf, sizeof buf, f)) { ... }
 *                                    fclose(f);
 *
 * Three things Python did for you and C does not:
 *   - it opened the file or raised; C returns NULL and you must check
 *   - it closed the file at the end of the block; C needs fclose()
 *   - it sized each line for you; C reads into a buffer YOU sized
 */
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    const char *path = (argc > 1) ? argv[1] : "sample.txt";

    FILE *f = fopen(path, "r");
    if (f == NULL) {
        perror(path);
        return 1;
    }

    char line[256];
    int n = 0;

    /* fgets returns the buffer, or NULL at end-of-file or on error.
     * Looping on the RETURN VALUE is the whole trick -- see 03. */
    while (fgets(line, sizeof line, f) != NULL) {
        n++;

        /* fgets KEEPS the '\n'. Almost always you want it gone. */
        line[strcspn(line, "\n")] = '\0';

        printf("%2d: [%s]\n", n, line);
    }

    if (ferror(f)) {              /* NULL meant error, not end-of-file */
        perror("fgets");
        fclose(f);
        return 1;
    }

    fclose(f);
    printf("%d lines\n", n);
    return 0;
}
