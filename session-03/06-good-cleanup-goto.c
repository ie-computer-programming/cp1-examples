/* GOOD: one cleanup tail, entered by jumping forward. Every path releases
   exactly what it acquired, in reverse order. */
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 4096

static int process(const char *path)
{
    int status = -1;

    FILE *f = fopen(path, "r");
    if (f == NULL) {
        perror("fopen");
        return -1;                  /* nothing acquired yet */
    }

    char *buf = malloc(BUF_SIZE);
    if (buf == NULL) {
        goto close_file;
    }

    size_t n = fread(buf, 1, BUF_SIZE, f);
    if (n == 0) {
        fprintf(stderr, "%s: empty or unreadable\n", path);
        goto free_buf;
    }

    printf("read %zu bytes from %s\n", n, path);
    status = 0;                     /* only reached on success */

free_buf:
    free(buf);
close_file:
    fclose(f);
    return status;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 2;
    }
    return process(argv[1]) == 0 ? 0 : 1;
}
