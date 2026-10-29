/* 04 -- a pipe: a one-way byte channel between related processes.
 *
 *     int fd[2];
 *     pipe(fd);     fd[0] = READ end,  fd[1] = WRITE end
 *
 * Create the pipe BEFORE fork(), so parent and child both inherit both ends.
 * Then each side closes the end it will not use, and what is left is a
 * one-way channel:   parent --write--> [ pipe ] --read--> child.
 *
 * Files and pipes are both "file descriptors" (Session 9): read() and
 * write() work on either. This is what `ls | grep foo` is made of.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int fd[2];
    if (pipe(fd) < 0) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {                     /* CHILD: reads */
        close(fd[1]);                   /* not writing */
        char buf[64];
        ssize_t n;
        while ((n = read(fd[0], buf, sizeof buf - 1)) > 0) {   /* 0 = end of data */
            buf[n] = '\0';
            printf("child read %zd bytes: \"%s\"\n", n, buf);
        }
        printf("child: read() returned %zd -- the writer closed its end\n", n);
        close(fd[0]);
        exit(0);
    }

    close(fd[0]);                       /* PARENT: writes; not reading */
    const char *msg[] = { "hello", "through", "a pipe" };
    for (int i = 0; i < 3; i++) {
        write(fd[1], msg[i], strlen(msg[i]));
        usleep(10000);                  /* so each message arrives on its own */
    }
    close(fd[1]);                       /* THIS is what lets the child's read() return 0 */

    waitpid(pid, NULL, 0);
    return 0;
}
