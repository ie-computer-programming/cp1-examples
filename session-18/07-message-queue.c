/* 07 -- a message queue: discrete MESSAGES, not a stream of bytes.
 *
 * A pipe is a stream: three write()s may arrive as one read(), or the other
 * way round. A message queue keeps message boundaries -- each msgsnd is
 * received by exactly one msgrcv -- and every message carries a TYPE, so a
 * receiver can ask for just the kind it wants.
 *
 * This uses System V message queues, not POSIX ones (mq_open): macOS does not
 * implement POSIX message queues at all, but System V works on both.
 *
 * Queues live in the KERNEL and outlive the program. A crash that skips
 * msgctl(IPC_RMID) leaves one behind: see `ipcs -q`, remove with `ipcrm -q`.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

struct message {
    long type;                  /* REQUIRED first member, and must be > 0 */
    char text[64];
};

#define TYPE_JOB  1
#define TYPE_STOP 2

int main(void)
{
    int q = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
    if (q < 0) {
        perror("msgget");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        msgctl(q, IPC_RMID, NULL);
        return 1;
    }

    if (pid == 0) {                              /* WORKER */
        struct message m;
        for (;;) {
            if (msgrcv(q, &m, sizeof m.text, 0, 0) < 0) {    /* type 0 = any */
                perror("msgrcv");
                _exit(1);
            }
            if (m.type == TYPE_STOP) {
                break;
            }
            printf("worker: got job \"%s\"\n", m.text);
        }
        printf("worker: told to stop\n");
        _exit(0);
    }

    const char *jobs[] = { "resize image", "send email", "build report" };
    struct message m;
    for (int i = 0; i < 3; i++) {
        m.type = TYPE_JOB;
        snprintf(m.text, sizeof m.text, "%s", jobs[i]);
        msgsnd(q, &m, sizeof m.text, 0);         /* size of the payload, NOT of `type` */
    }
    m.type = TYPE_STOP;
    m.text[0] = '\0';
    msgsnd(q, &m, sizeof m.text, 0);

    waitpid(pid, NULL, 0);
    msgctl(q, IPC_RMID, NULL);                   /* the kernel will NOT clean up for you */
    return 0;
}
