/* 05-bad -- an early return that forgets the file.
 *
 * C has no `with`, no `finally`, no destructor. Every return between fopen
 * and fclose is a path where the file stays open. Here a second file fails
 * to open and the function returns -- leaking the first one.
 *
 * One leak does not matter. The same function called in a loop, in a server,
 * runs the process out of file descriptors and then fails to open anything
 * at all, somewhere completely unrelated.
 */
#include <stdio.h>

#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
/* Cap our own open-file limit so the demo fails quickly and identically on
 * every machine. POSIX, not standard C -- it is scaffolding, not the lesson. */
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
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        perror(path);
        return -2;                 /* could not even start */
    }

    FILE *aux = fopen(aux_path, "r");
    if (aux == NULL) {
        return -1;                 /* f is STILL OPEN */
    }

    char line[128];
    if (fgets(line, sizeof line, f) != NULL) {
        printf("first line: %s", line);
    }

    fclose(aux);
    fclose(f);
    return 0;
}

int main(int argc, char **argv)
{
    const char *path = (argc > 1) ? argv[1] : "sample.txt";
    const char *aux  = (argc > 2) ? argv[2] : "no-such-config.txt";

    limit_open_files(64);

    /* Call it the way a server would: over and over. */
    for (int i = 1; i <= 2000; i++) {
        if (process(path, aux) == -2) {
            printf("iteration %d: could not open %s any more\n", i, path);
            printf("every previous call leaked one open file\n");
            return 0;
        }
    }

    printf("2000 iterations, no failure\n");
    return 0;
}
