/* 02-bad -- a base case that is never reached.
 *
 * countdown(n) stops at exactly 0, but it steps by 2. Start from an odd
 * number and it jumps straight past 0 into the negatives, forever.
 *
 * Python stops you at depth 1000 with RecursionError and a traceback.
 * C has no limit: it recurses until the stack (8 MB by default on Linux)
 * runs out, and then the operating system kills the process.
 */
#include <stdio.h>
#include <stdlib.h>

static long depth = 0;

static void countdown(int n)
{
    char frame_ballast[64];             /* a realistic frame, not an empty one */
    frame_ballast[0] = (char)n;

    depth++;
    if (depth % 20000 == 0) {
        fprintf(stderr, "depth %ld, n = %d\n", depth, n);  /* stderr: survives the crash */
    }
    if (n == 0) {                       /* never true for an odd start */
        return;
    }
    countdown(n - 2);
    (void)frame_ballast;
}

int main(int argc, char **argv)
{
    int start = (argc > 1) ? atoi(argv[1]) : 7;
    countdown(start);
    printf("done -- never printed for an odd start\n");
    return 0;
}
