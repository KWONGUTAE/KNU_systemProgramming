#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>
#include <fcntl.h>
#include <termio.h>
#include <signal.h>

#define TRIES 3
#define SLEEPTIME 2
#define QUESTION "Do you want another transaciton"
#define BEEP putchar('\a');

int get_ok_char() {
    int c;
    while ((c = getchar()) != EOF && strchr("ynYN", c) == NULL)
    {
        continue;
    }

    return c;
}

int get_response(char *question, int max_tries) {
    int input;
    printf("%s (y/n)", question);
    fflush(stdout);
    while (1)
    {
        sleep(SLEEPTIME);
        input = tolower(get_ok_char());
        max_tries -= 1;
        if (input == (int)'y') return 0;
        if (input == (int)'n') return 1;
        if (max_tries == 0) return 2;

        BEEP;
    }
}

void set_cr_noecho_mode() {
    struct termios ttyinfo;
    if (tcgetattr(0, &ttyinfo) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    ttyinfo.c_lflag &= ~ICANON;
    ttyinfo.c_lflag &= ~ECHO;
    ttyinfo.c_cc[VMIN] = 1;

    if (tcsetattr(0, TCSANOW, &ttyinfo) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void set_non_blocking_node() {
    int terflags;
    terflags = fcntl(0, F_GETFL);
    terflags |= O_NONBLOCK;
    fcntl(0, F_SETFL, terflags);
}

void tty_mode(int how) {
    static struct termios orig_mode;
    static int orig_flags;
    if (how == 0) {
        tcgetattr(0, &orig_mode);
        orig_flags = fcntl(0, F_GETFL);
    } else if (how == 1) {
        tcsetattr(0, TCSANOW, &orig_mode);
        fcntl(0, F_SETFL, orig_flags);
    }
}

void ctrlc_handler(int signum) {
    tty_mode(1);
    exit(130);
}

int main() {
    int response = 0;
    tty_mode(0);
    set_cr_noecho_mode();
    set_non_blocking_node();

    signal(SIGINT, ctrlc_handler);
    signal(SIGQUIT, SIG_IGN);

    response = get_response(QUESTION, TRIES);
    tty_mode(1);
    return response;
}
