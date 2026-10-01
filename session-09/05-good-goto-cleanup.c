/* 05-good -- Session 3's goto cleanup pattern, now with files in it.
 *
 * One exit path. Each label releases exactly one thing, in the reverse order
 * it was acquired. Adding a third resource means adding one label, not
 * auditing every return in the function.
 */
#include <stdio.h>

#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
/* Same cap as 05-bad, so the two runs are directly comparable. */
static void limit_open_files(int n)
{
    struct rlimit rl = {.rlim_cur = (rlim_t)n, .rlim_max = (rlim_t)n};
    setrlimit(RLIMIT_NOFILE, &rl);
}
#else
static void limit_open_files(int n) { (void)n; }
#endif

static int process(const char *path, const char *aux_path)
{
    int status = -1;

    FILE *f = fopen(path, "r");
    if (f == NULL) {
        perror(path);
        return -2;                 /* nothing acquired yet */
    }

    FILE *aux = fopen(aux_path, "r");
    if (aux == NULL) {
        goto close_f;
    }

    char line[128];
    if (fgets(line, sizeof line, f) != NULL) {
        printf("first line: %s", line);
    }
    status = 0;

    fclose(aux);
close_f:
    if (fclose(f) != 0) {          /* fclose can fail -- see 06 */
        perror("fclose");
        status = -1;
    }
    return status;
}

int main(int argc, char **argv)
{
    const char *path = (argc > 1) ? argv[1] : "sample.txt";
    const char *aux  = (argc > 2) ? argv[2] : "no-such-config.txt";

    limit_open_files(64);

    /* Exactly the same loop as 05-bad. */
    for (int i = 1; i <= 2000; i++) {
        if (process(path, aux) == -2) {
            printf("iteration %d: could not open %s any more\n", i, path);
            return 1;
        }
    }

    printf("2000 iterations, every file closed on every path\n");
    return 0;
}
