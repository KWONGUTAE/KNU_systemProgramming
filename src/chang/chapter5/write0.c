#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


int main(int ac, char *av[]) {
    int fd = 0;
    char buf[BUFSIZ];

    if (ac != 2) {
        fprintf(stderr, "Usage: write0 ttyname\n");
        exit(-1);
    }
    
    fd = open(av[1], O_WRONLY);bufror(av[1]);
        exit(-1);
    }

    while (fgets(buf, BUFSIZ, stdin) != NULL)
    {
        printf("one iteration executed\n");
        if (write(fd, buf, strlen(buf)) == -1) {
            break;
        }
        printf("write one piece of contents\n");
    }
    close(fd);
}
