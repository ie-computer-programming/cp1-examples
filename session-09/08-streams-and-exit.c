/* 08 -- the three standard streams, and what your exit code means.
 *
 * stdout  normal output.  Block-buffered when redirected to a file.
 * stderr  diagnostics.    UNBUFFERED -- appears immediately, even before a crash.
 * stdin   input.          Read it with fgets, exactly like any other FILE *.
 *
 * They are ordinary FILE * values, so every function from this session works
 * on them: fprintf(stdout, ...) is printf(...).
 *
 * Try it:
 *   ./08-streams-and-exit > out.txt     stdout redirected, stderr still on screen
 *   ./08-streams-and-exit 2> err.txt    the other way round
 *   echo hello | ./08-streams-and-exit  stdin from a pipe
 *   echo $?                             the exit code
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("this goes to stdout\n");
    fprintf(stdout, "so does this -- printf IS fprintf(stdout, ...)\n");
    fprintf(stderr, "this goes to stderr\n");

    char line[128];
    if (fgets(line, sizeof line, stdin) != NULL) {
        printf("stdin gave me: %s", line);
    } else {
        printf("stdin was empty (end of file)\n");
    }

    /* Exit code: 0 means success, non-zero means failure. The shell reads it
     * with `echo $?`, and `&&` / `||` branch on it.
     *
     *   return 1;   from main   -- identical to ...
     *   exit(1);               -- ... except exit() works from ANY function
     *
     * EXIT_SUCCESS and EXIT_FAILURE are the portable spellings of 0 and 1.
     */
    fprintf(stderr, "exiting with EXIT_SUCCESS (0)\n");
    return EXIT_SUCCESS;
}
