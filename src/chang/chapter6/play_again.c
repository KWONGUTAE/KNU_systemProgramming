#include <stdio.h>
#include <stdlib.h>
#include <termio.h>

#define QUESTION "Do you want another transaciton"
int get_response(char *question) {
    int input = -1;
    printf("%s (y/n)", question);
    while (1)
    {
        switch (input = getchar())
        {
        case 'y':
        case 'Y':
            return 0;
        case 'n':
        case 'N':
        case EOF:
            return 1;
        default:
            printf("\ncannot understand %c, ", input);
            printf("\nPlease type y or n\n");
        }
    }
}

void set_crmode() {
    struct termios ttyinfo;
    if (tcgetattr(0, &ttyinfo) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    ttyinfo.c_lflag &= ~ICANON;
    ttyinfo.c_cc[VMIN] = 1;

    if (tcsetattr(0, TCSANOW, &ttyinfo) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void tty_mode(int how) {
    static struct termios orig_mode;
    if (how == 0) tcgetattr(0, &orig_mode);
    else if (how == 1) tcsetattr(0, TCSANOW, &orig_mode);
}

int main () {
    int response = 0;

    tty_mode(0);

    set_crmode();
    response = get_response(QUESTION);

    tty_mode(1);
    return response;
}