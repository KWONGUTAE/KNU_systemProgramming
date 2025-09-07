#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define oops(m, x) { perror(m); exit(x); }

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "usage: %s cmd1 arg1 cmd2 cmd3\n", argv[0]);
        exit(1);
    }

    int pipe1[2], pipe2[2];
    pid_t pid1, pid2;

    // pipe1: cat f2.txt -> sort
    if (pipe(pipe1) == -1)
        oops("pipe1 failed", 1);

    if ((pid1 = fork()) == -1)
        oops("fork1 failed", 2);

    if (pid1 == 0) {
        // child 1: run "cat f2.txt"
        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[0]);
        close(pipe1[1]);

        char *cmd1_args[] = { argv[1], argv[2], NULL };  // cat f2.txt
        execvp(cmd1_args[0], cmd1_args);
        oops("execvp cmd1 failed", 3);
    }

    // pipe2: sort -> head
    if (pipe(pipe2) == -1)
        oops("pipe2 failed", 4);

    if ((pid2 = fork()) == -1)
        oops("fork2 failed", 5);

    if (pid2 == 0) {
        // child 2: run "sort"
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[0]); close(pipe2[1]);

        char *cmd2_args[] = { argv[3], NULL };  // sort
        execvp(cmd2_args[0], cmd2_args);
        oops("execvp cmd2 failed", 6);
    }

    // parent: run "head"
    dup2(pipe2[0], STDIN_FILENO);
    close(pipe1[0]); close(pipe1[1]);
    close(pipe2[0]); close(pipe2[1]);

    char *cmd3_args[] = { argv[4], NULL };  // head
    execvp(cmd3_args[0], cmd3_args);
    oops("execvp cmd3 failed", 7);
}
