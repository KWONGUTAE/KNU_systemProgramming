#include <stdio.h>
#include <stdlib.h>
#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *);
int see_more(FILE *);

int main(int ac, char *av[]) {
    FILE *fp;
    if (ac == 1) {
        do_more(stdin);
    } else {
        while (--ac)
        {
            if ((fp = fopen(*++av, "r")) != NULL) {
                do_more(fp);
                fclose(fp);
            } else {
                exit(1);
            }
        }
        
    }
}

void do_more(FILE *fp) {
    char line[LINELEN];
    int num_of_line = 0;
    int reply = 0;
    FILE *fp_tty = fopen("/dev/tty", "r");
    if (fp_tty == NULL) {
        exit(1);
    }

    while (fgets(line, LINELEN, fp) != NULL) {
        if (num_of_line == PAGELEN) {
            reply = see_more(fp_tty);
            if (reply == 0) break;
            num_of_line -= reply;
        }
        if (fputs(line, stdout) == EOF)
            exit(1);

        num_of_line++;
    }
}

int see_more(FILE *cmd) {
    int c = 0;
    printf("\033[7m more? \033[m");

    while ((c = getc(cmd)) != EOF) {
#ifdef DEBUG
        printf("\nsee more: %c ", c);
#endif
        if (c == 'q') return 0;
        if (c == ' ') return PAGELEN;
        if (c == '\n') {
#ifdef DEBUG
            printf("carriage return \n");
#endif
            return 1;
        }
    }
    return 0;
}