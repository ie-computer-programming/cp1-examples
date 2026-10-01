/* 07 -- command-line arguments and environment variables.
 *
 * Python:                        C:
 *   sys.argv                       int main(int argc, char **argv)
 *   sys.argv[0]  script name       argv[0]  program name
 *   len(sys.argv)                  argc
 *   os.environ.get("HOME")         getenv("HOME")   -- NULL if unset
 *
 * argv is an array of argc pointers to C strings, and argv[argc] is NULL.
 * The shell has already split the line and expanded globs and $VARIABLES
 * before your program starts -- you receive the result, not the typing.
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    printf("argc = %d\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("  argv[%d] = \"%s\"\n", i, argv[i]);
    }
    printf("  argv[%d] = %p   (always NULL)\n", argc, (void *)argv[argc]);

    /* Arguments arrive as TEXT. "42" is not 42 -- convert, and check. */
    if (argc > 1) {
        char *end = NULL;
        long value = strtol(argv[1], &end, 10);

        if (end == argv[1] || *end != '\0') {
            fprintf(stderr, "  \"%s\" is not a number\n", argv[1]);
        } else {
            printf("  as a number: %ld\n", value);
        }
    }

    /* getenv returns NULL when the variable is not set -- check it. */
    const char *home = getenv("HOME");
    const char *nope = getenv("CP1_DEFINITELY_NOT_SET");

    printf("HOME                    = %s\n", home != NULL ? home : "(unset)");
    printf("CP1_DEFINITELY_NOT_SET  = %s\n", nope != NULL ? nope : "(unset)");

    return 0;
}
