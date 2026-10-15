/* 05 -- backtracking: choose, explore, UN-choose.
 *
 * Generate every ordering of some characters. At each position, try every
 * character not used yet: mark it used (choose), fill the rest of the
 * positions (explore), then unmark it (un-choose) so the next attempt
 * starts from a clean slate.
 *
 * The un-choose step is what makes it BACKtracking. Forget it and the
 * second branch inherits the first branch's leftovers.
 */
#include <stdio.h>
#include <string.h>

#define MAX 8

static int count;

static void permute(const char *items, int n, char *current, int depth, int *used)
{
    if (depth == n) {                       /* a complete arrangement */
        current[depth] = '\0';
        printf("  %s\n", current);
        count++;
        return;
    }
    for (int i = 0; i < n; i++) {
        if (used[i]) {
            continue;
        }
        used[i] = 1;                        /* choose  */
        current[depth] = items[i];
        permute(items, n, current, depth + 1, used);   /* explore */
        used[i] = 0;                        /* un-choose */
    }
}

int main(void)
{
    const char *items = "ABCD";
    int  n = (int)strlen(items);
    char current[MAX + 1];
    int  used[MAX] = {0};

    permute(items, n, current, 0, used);
    printf("%d permutations of \"%s\" (4! = 24)\n", count, items);
    return 0;
}
