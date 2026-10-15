/* 04 -- Towers of Hanoi: where recursion is the only sane way to think.
 *
 * Move n disks from `from` to `to`, using `via`, never putting a larger
 * disk on a smaller one. The whole solution is three sentences:
 *
 *   1. move the top n-1 disks out of the way, onto `via`
 *   2. move the biggest disk to `to`
 *   3. move the n-1 disks from `via` on top of it
 *
 * Steps 1 and 3 are the same problem, smaller. Trust that they work --
 * that is the "leap of faith" every recursive function is built on.
 */
#include <stdio.h>
#include <stdlib.h>

static unsigned long long moves;
static int verbose;

static void hanoi(int n, char from, char to, char via)
{
    if (n == 0) {
        return;
    }
    hanoi(n - 1, from, via, to);
    moves++;
    if (verbose) {
        printf("  move disk %d: %c -> %c\n", n, from, to);
    }
    hanoi(n - 1, via, to, from);
}

int main(void)
{
    verbose = 1;
    hanoi(3, 'A', 'C', 'B');
    printf("3 disks: %llu moves\n\n", moves);

    verbose = 0;
    for (int n = 5; n <= 25; n += 5) {
        moves = 0;
        hanoi(n, 'A', 'C', 'B');
        printf("%2d disks: %10llu moves   (2^%d - 1 = %llu)\n",
               n, moves, n, (1ull << n) - 1);
    }
    return 0;
}
