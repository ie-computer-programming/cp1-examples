/* BAD: each error path returns directly, forgetting what it already acquired.
   The file is never closed and the buffer is never freed. */
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 4096

static int process(const char *path)
{
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        perror("fopen");
        return -1;
    }

    char *buf = malloc(BUF_SIZE);
    if (buf == NULL) {
        return -1;              /* leaked: f is still open */
    }

    size_t n = fread(buf, 1, BUF_SIZE, f);
    if (n == 0) {
        fprintf(stderr, "%s: empty or unreadable\n", path);
        return -1;              /* leaked: f still open AND buf never freed */
    }

    printf("read %zu bytes from %s\n", n, path);

    free(buf);
    fclose(f);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 2;
    }
    return process(argv[1]) == 0 ? 0 : 1;
}
