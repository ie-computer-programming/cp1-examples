/* 03-good -- fork, exec, wait: how a shell runs a command.
 *
 *     ./03-good-exec                      runs: echo hello from exec
 *     ./03-good-exec ls -l                runs: ls -l
 *     ./03-good-exec no-such-program      the failure path
 *
 * 1. fork()    make a child
 * 2. exec*()   the child REPLACES itself with the new program
 * 3. waitpid() the parent waits and reads the exit code
 *
 * If exec fails, the child reports it and leaves with _exit(127) -- the code
 * shells use for "command not found". _exit, not exit: the child is a copy of
 * the parent, and exit() would flush a second copy of the parent's buffered
 * output.
 */
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    char *default_cmd[] = { "echo", "hello", "from", "exec", NULL };
    char **cmd = (argc > 1) ? &argv[1] : default_cmd;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        execvp(cmd[0], cmd);           /* p: search PATH; v: arguments as a vector */
        perror(cmd[0]);                /* only reached if exec failed */
        _exit(127);
    }

    int status = 0;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
        printf("[parent] '%s' finished with exit code %d\n", cmd[0], WEXITSTATUS(status));
    }
    return 0;
}
