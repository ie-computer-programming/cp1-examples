/* 02-good -- waitpid() collects the child AND tells you how it ended.
 *
 * The status word packs two different stories. Ask which one it is first:
 *
 *   WIFEXITED(status)    the child called exit() / returned from main
 *       WEXITSTATUS        ... with this code        (Session 9: 0 = success)
 *   WIFSIGNALED(status)  the OS killed it with a signal
 *       WTERMSIG           ... this one
 *
 * That is where the shell's "exit code 134" from Session 3 came from:
 * 128 + 6, SIGABRT. The shell is just a parent calling waitpid().
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void report(pid_t pid)
{
    int status = 0;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return;
    }
    if (WIFEXITED(status)) {
        printf("child %d exited normally with code %d\n", (int)pid, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("child %d was killed by signal %d  (a shell would report %d)\n",
               (int)pid, WTERMSIG(status), 128 + WTERMSIG(status));
    }
}

int main(void)
{
    pid_t a = fork();
    if (a == 0) {
        exit(7);                       /* a deliberate, ordinary failure code */
    }

    pid_t b = fork();
    if (b == 0) {
        abort();                       /* dies with SIGABRT, like a failed assert() */
    }

    report(a);
    report(b);
    return 0;
}
