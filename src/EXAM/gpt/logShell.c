#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <sys/stat.h>

#define MAX_CMD_LEN 1024

struct termios original_mode;

void restore_terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_mode);
}

void handle_sigint(int sig) {
    int fd = open("interrupt.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd != -1) {
        write(fd, "Interrupted by user\n", 21);
        close(fd);
    }
    restore_terminal();
    printf("\nInterrupt logged. Exiting...\n");
    exit(1);
}

void set_noncanonical_mode() {
    struct termios new_mode;
    tcgetattr(STDIN_FILENO, &original_mode);
    new_mode = original_mode;
    new_mode.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_mode);
    atexit(restore_terminal);
}

int main() {
    struct stat st;
    char buffer[MAX_CMD_LEN];
    int log_fd;

    if (stat(".", &st) == -1) {
        perror("stat");
        exit(1);
    }

    printf("Current dir inode: %lu\n", (unsigned long)st.st_ino);
    printf("cmd> ");
    fflush(stdout);

    set_noncanonical_mode();
    signal(SIGINT, handle_sigint);

    log_fd = open("commands.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd == -1) {
        perror("open log");
        exit(1);
    }

    int idx = 0;
    char ch;
    while (1) {
        ssize_t n = read(STDIN_FILENO, &ch, 1);
        if (n > 0) {
            if (ch == '\n') {
                buffer[idx] = '\0';
                write(log_fd, buffer, idx);
                write(log_fd, "\n", 1);

                if (strcmp(buffer, "exit") == 0) {
                    printf("\nBye!\n");
                    break;
                }

                idx = 0;
                printf("cmd> ");
                fflush(stdout);
            } else {
                if (idx < MAX_CMD_LEN - 1) {
                    buffer[idx++] = ch;
                    write(STDOUT_FILENO, &ch, 1);  // echo
                }
            }
        }
    }

    close(log_fd);
    return 0;
}
