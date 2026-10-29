/* 01-bad -- fork() without flushing stdout first.
 *
 * Same program as 01-fork-basics, minus one line: the fflush(stdout).
 *
 *     ./01-bad-no-flush              on a terminal: stdout is line-buffered,
 *                                    "before fork" is already out. Looks fine.
 *     ./01-bad-no-flush | cat        piped: stdout is BLOCK-buffered (Session 9),
 *                                    so "before fork" is still in the buffer when
 *                                    fork() copies the process. BOTH flush it.
 *
 * Run it both ways and compare. The bug only shows when the output is not a
 * terminal -- the case that matters for logs, pipelines and CI.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int x = 100;

    printf("before fork: pid %d, x = %d\n", (int)getpid(), x);
    /* BUG: no fflush(stdout) here */

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        x += 1;
        printf("child : pid %d, parent %d, x = %d\n",
               (int)getpid(), (int)getppid(), x);
        exit(0);
    }

    waitpid(pid, NULL, 0);
    printf("parent: pid %d, child was %d, x = %d\n", (int)getpid(), (int)pid, x);
    return 0;
}
