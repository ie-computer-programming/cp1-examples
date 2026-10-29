/* 03-bad -- exec only returns when it FAILS, and the child carries on.
 *
 * exec replaces the running program with a different one. If it works, it
 * never returns -- the old code is gone. So any line after exec runs only
 * when exec failed.
 *
 * Forget to stop there, and the child -- a full copy of the parent -- runs
 * on through the rest of the parent's code. Here that means two processes
 * both executing "the rest of main".
 */
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        execlp("no-such-program", "no-such-program", (char *)NULL);
        /* BUG: only reached if exec failed -- and nothing stops the child here */
    }

    printf("pid %d is running the rest of main\n", (int)getpid());

    if (pid > 0) {
        waitpid(pid, NULL, 0);
    }
    return 0;
}
