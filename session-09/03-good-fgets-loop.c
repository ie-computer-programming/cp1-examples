/* 03-good -- loop on what the read returns, not on feof().
 *
 * Every read function already tells you it failed:
 *   fgets  -> NULL
 *   fscanf -> fewer conversions than you asked for
 *   fread  -> fewer items than you asked for
 *
 * Use that. feof() and ferror() are for afterwards, to tell the two kinds of
 * failure apart once the loop has stopped.
 */
#include <stdio.h>

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

    while (fgets(line, sizeof line, f) != NULL) {
        printf("%2d: %s", ++n, line);
    }

    /* The loop ended. Which was it? */
    if (ferror(f)) {
        perror("fgets");
        fclose(f);
        return 1;
    }

    fclose(f);
    printf("clean end of file after %d lines\n", n);
    return 0;
}
