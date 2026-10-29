/* 05-bad -- the pipe that never reports end-of-data.
 *
 * read() on a pipe returns 0 ("end of file") only when EVERY copy of the
 * write end is closed. The parent closed its copy -- but the CHILD still
 * holds one, inherited from fork(), and it is waiting in read() on the other
 * end of that very pipe. Nobody can ever write again, and nobody knows it.
 *
 * The program does not crash. It stops, forever -- like Session 17's
 * deadlock. An alarm ends this demo after 2 seconds.
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static void timeout(int sig)
{
    (void)sig;
    static const char msg[] = "HUNG: the child is still waiting for data -- killed by alarm\n";
    write(STDERR_FILENO, msg, sizeof msg - 1);
    _exit(1);
}

int main(void)
{
    signal(SIGALRM, timeout);
    alarm(2);

    int fd[2];
    pipe(fd);

    pid_t pid = fork();
    if (pid == 0) {
        /* BUG: no close(fd[1]) here -- this child still holds a write end. */
        char buf[64];
        ssize_t n;
        while ((n = read(fd[0], buf, sizeof buf)) > 0) {
            printf("child got %zd bytes\n", n);
        }
        exit(0);                        /* never reached: read() never returns 0 */
    }

    write(fd[1], "hello", 5);
    close(fd[1]);                       /* the parent's copy is closed ... */
    waitpid(pid, NULL, 0);              /* ... but the child's is not */

    puts("done");
    return 0;
}
