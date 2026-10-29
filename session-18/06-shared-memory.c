/* 06 -- shared memory: two processes, ONE block of memory.
 *
 * fork() gives the child a COPY of everything (01). To share, ask the OS for
 * a region both processes map into their address space:
 *
 *     mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0)
 *
 * MAP_SHARED is the whole point: writes by one process are visible to the
 * other. Pipes COPY data through the kernel; shared memory copies nothing,
 * so it is the fastest IPC there is -- and, like threads (Session 17), it has
 * no protection of its own. Two writers can race.
 *
 * (MAP_ANONYMOUS memory is shared with children created by fork. Unrelated
 * processes use shm_open() to share a NAMED region instead.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON       /* the older BSD / macOS spelling */
#endif

#define N_CHILDREN 4
#define PER_CHILD  250000

int main(void)
{
    /* One counter, in memory that survives the fork as SHARED. */
    volatile long *counter = mmap(NULL, sizeof *counter, PROT_READ | PROT_WRITE,
                                  MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (counter == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    *counter = 0;

    /* Compare: an ordinary variable is COPIED, so each child bumps its own. */
    long private_copy = 0;

    for (int i = 0; i < N_CHILDREN; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        }
        if (pid == 0) {
            for (int k = 0; k < PER_CHILD; k++) {
                (*counter)++;           /* RACY, exactly like Session 16's counter++ */
                private_copy++;
            }
            _exit(0);
        }
    }
    for (int i = 0; i < N_CHILDREN; i++) {
        wait(NULL);
    }

    long expected = (long)N_CHILDREN * PER_CHILD;
    printf("ordinary variable : %ld   (each child changed only its own copy)\n", private_copy);
    printf("shared memory     : %ld   expected %ld   lost %ld increments\n",
           *counter, expected, expected - *counter);

    munmap((void *)counter, sizeof *counter);
    return 0;
}
