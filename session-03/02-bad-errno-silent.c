/* BAD: the failure is detected, but nothing useful is said about it.
   Missing file? No permission? Path too long? The message is the same. */
#include <stdio.h>

int main(void)
{
    const char *path = "does-not-exist.txt";

    FILE *f = fopen(path, "r");
    if (f == NULL) {
        fprintf(stderr, "error\n");
        return 1;
    }

    fclose(f);
    return 0;
}
