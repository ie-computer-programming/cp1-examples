/* 08 -- socketpair(): a pipe that works in BOTH directions.
 *
 * Everything so far has been one-way (a pipe) or shared-and-unprotected
 * (shared memory). A socket is a two-way channel, and socketpair() makes two
 * connected sockets in one call -- no network involved.
 *
 * Why it matters: it uses exactly the same send/recv calls you will use over
 * the internet from Session 19 onward. A socket is simply IPC that can also
 * cross a machine boundary.
 *
 *     parent <== request  =====> child
 *     parent <=== reply ======== child
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int sv[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) < 0) {
        perror("socketpair");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {                          /* CHILD: an "upper-case server" */
        close(sv[0]);
        char buf[64];
        ssize_t n = recv(sv[1], buf, sizeof buf - 1, 0);
        if (n > 0) {
            for (ssize_t i = 0; i < n; i++) {
                buf[i] = (char)toupper((unsigned char)buf[i]);
            }
            send(sv[1], buf, (size_t)n, 0);  /* the reply goes back the SAME way */
        }
        close(sv[1]);
        _exit(0);
    }

    close(sv[1]);                            /* PARENT: the client */
    const char *request = "hello, child";
    send(sv[0], request, strlen(request), 0);

    char reply[64];
    ssize_t n = recv(sv[0], reply, sizeof reply - 1, 0);
    if (n > 0) {
        reply[n] = '\0';
        printf("sent  : \"%s\"\n", request);
        printf("reply : \"%s\"\n", reply);
    }
    close(sv[0]);

    waitpid(pid, NULL, 0);
    return 0;
}
