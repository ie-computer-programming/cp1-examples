/* 03-bad -- the classic: while (!feof(f))
 *
 * feof() does not predict the future. It reports whether a PREVIOUS read
 * already hit the end of the file. So the loop runs one extra time:
 *
 *   read "gamma"   -> ok, eof flag still clear
 *   !feof(f)       -> true, loop again
 *   fgets          -> FAILS, returns NULL, sets the eof flag
 *   print line     -> prints the buffer's PREVIOUS contents, again
 *
 * The last line appears twice, and the program exits 0.
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

    while (!feof(f)) {
        fgets(line, sizeof line, f);   /* return value ignored */
        printf("%2d: %s", ++n, line);
    }

    fclose(f);
    printf("exit code will be 0\n");
    return 0;
}
