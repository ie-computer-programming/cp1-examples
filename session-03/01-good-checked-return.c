/* GOOD: every call that can fail is checked, and failure is reported. */
#include <stdio.h>

int main(void)
{
    const char *path = "does-not-exist.txt";

    FILE *f = fopen(path, "r");
    if (f == NULL) {
        fprintf(stderr, "could not open %s\n", path);
        return 1;
    }

    char line[128];
    if (fgets(line, sizeof line, f) == NULL) {
        fprintf(stderr, "could not read from %s\n", path);
        fclose(f);
        return 1;
    }

    printf("read: %s", line);
    fclose(f);
    return 0;
}
