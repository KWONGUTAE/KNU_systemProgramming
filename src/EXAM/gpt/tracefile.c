#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>

struct termios original_mode;

void restore_terminal_mode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_mode);
}

void handle_sigint(int sig) {
    restore_terminal_mode();
    printf("\nUser terminated monitoring.\n");
    exit(0);
}

void set_non_canonical_mode() {
    struct termios new_mode;

    tcgetattr(STDIN_FILENO, &original_mode);         // 현재 설정 저장
    new_mode = original_mode;

    new_mode.c_lflag &= ~(ICANON | ECHO);            // Canonical 모드 off, echo off
    tcsetattr(STDIN_FILENO, TCSANOW, &new_mode);

    atexit(restore_terminal_mode);                   // 종료 시 복원
}

int main() {
    struct stat file_stat;
    ino_t original_inode;

    if (stat("target.txt", &file_stat) == -1) {
        perror("target.txt");
        exit(1);
    }

    original_inode = file_stat.st_ino;
    printf("Watching target.txt (inode: %lu)\n", (unsigned long)original_inode);
    printf("Press Ctrl+C to quit.\n");

    set_non_canonical_mode();
    signal(SIGINT, handle_sigint);

    while (1) {
        sleep(3);
        if (stat("target.txt", &file_stat) == -1) {
            perror("target.txt removed?");
            break;
        }

        if (file_stat.st_ino != original_inode) {
            printf("File modified or replaced!\n");
            break;
        }
    }

    return 0;
}
