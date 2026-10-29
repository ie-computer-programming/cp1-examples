/* 02-bad -- a child nobody waits for becomes a zombie.
 *
 * When a process exits, the kernel keeps a small record of it -- its exit
 * status -- until the parent collects it with wait(). Until then the child
 * is a ZOMBIE: dead, using no CPU and no memory, but still occupying a slot
 * in the process table.
 *
 * One zombie is harmless. A server that forks per request and never waits
 * fills the process table, and then fork() itself starts failing.
 *
 * Python hides this: subprocess.Popen objects reap their children for you.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        exit(0);                       /* the child ends immediately */
    }

    sleep(1);                          /* ... and the parent never waits */

    char cmd[64];
    snprintf(cmd, sizeof cmd, "ps -o pid,stat,comm -p %d", (int)pid);
    printf("child %d has exited; the parent has NOT called wait():\n", (int)pid);
    fflush(stdout);
    system(cmd);                       /* STAT 'Z' means zombie */

    return 0;
}
