/* GOOD: errno says exactly why the call failed. */
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    const char *path = "does-not-exist.txt";

    FILE *f = fopen(path, "r");
    if (f == NULL) {
        /* Saved immediately: perror and fprintf are themselves library calls
           and are allowed to overwrite errno. */
        int saved = errno;

        perror("fopen");
        fprintf(stderr, "could not open %s: %s (errno %d)\n",
                path, strerror(saved), saved);
        return 1;
    }

    fclose(f);
    return 0;
}
