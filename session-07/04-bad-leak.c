/* BAD: a leak. Every loop iteration asks for memory and never gives it back.
 *
 * Nothing crashes. The program prints the right answer and exits 0. That is
 * the whole problem: a leak is invisible until the process is long-lived
 * enough to run out of memory -- a server, not a demo.
 *
 * Python's garbage collector made this impossible. C has no collector.
 *
 * Watch it get caught:
 *     ./build/04-bad-leak-asan
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    long total = 0;

    for (int i = 0; i < 100; i++) {
        int *block = malloc(64 * sizeof *block);
        if (block == NULL) {
            perror("malloc");
            return 1;
        }
        block[0] = i;
        total += block[0];
        /* <-- free(block) missing: 100 blocks, 25.6 KB, gone for good */
    }

    printf("total = %ld\n", total);
    printf("exit code will be 0 -- the leak is silent\n");
    return 0;
}
