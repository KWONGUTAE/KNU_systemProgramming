#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>
#include <fcntl.h>
#include <termio.h>

#define TRIES 3
#define SLEEPTIME 2
#define QUESTION "Do you want another transaciton"
#define BEEP putchar('\a');

int get_ok_char() {
    int c;
    while ((c = getchar()) != EOF && strchr("ynYN", c) == NULL) {
        continue;
    }
    printf("get_ok_char(): %d\n", c);
    return c;
}
