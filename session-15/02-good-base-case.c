/* 02-good -- the base case must catch EVERY way the input can end.
 *
 * `n <= 0` instead of `n == 0`. One character, and every start value
 * terminates.
 */
#include <stdio.h>
#include <stdlib.h>

static long depth = 0;

static void countdown(int n)
{
    depth++;
    if (n <= 0) {
        return;
    }
    countdown(n - 2);
}

int main(int argc, char **argv)
{
    int start = (argc > 1) ? atoi(argv[1]) : 7;
    countdown(start);
    printf("countdown(%d) finished after %ld calls\n", start, depth);
    return 0;
}
