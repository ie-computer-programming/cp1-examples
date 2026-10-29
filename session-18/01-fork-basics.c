/* 01 -- fork() returns TWICE: once in the parent, once in the child.
 *
 * fork() clones the running process. From the very next instruction there
 * are two programs, identical except for one thing: what fork() returned.
 *
 *     pid  < 0   fork failed (no child exists)
 *     pid == 0   "I am the child"
 *     pid  > 0   "I am the parent; the number is my child's pid"
 *
 * The child gets a COPY of the parent's memory (Session 7: stack, heap,
 * globals). Changing x in the child changes the child's x. That is the whole
 * difference between processes and the threads of Sessions 16-17.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int x = 100;

    printf("before fork: pid %d, x = %d\n", (int)getpid(), x);
    fflush(stdout);          /* see the note at the bottom */

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

    waitpid(pid, NULL, 0);   /* always collect your child -- see 02 */
    printf("parent: pid %d, child was %d, x = %d   <-- unchanged: the child had a COPY\n",
           (int)getpid(), (int)pid, x);
    return 0;
}

/* Why the fflush: stdout is block-buffered when it is not a terminal
 * (Session 9). Anything still sitting in the buffer at fork() is copied into
 * the child, and BOTH processes flush it. Try removing the fflush and run
 *     ./01-fork-basics | cat
 * "before fork" appears twice. Flush before you fork. */
