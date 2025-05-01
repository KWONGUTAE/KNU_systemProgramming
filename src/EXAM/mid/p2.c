#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

void printHead(char *filename, int line);

int main(int ac, char* av[]) {
    if (ac == 2) {
        char *ptr;
        int line;
        ptr = strtok(av[1], "-");
        printf("%s", ptr);
        
        line = atoi(ptr);
        printHead("p1.c", line);
    } else {
        printHead("p1.c", 10);
    }
    return 0;
}

void printHead(char *filename, int line) {
    FILE *fp;
    char buf[1024];

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("%s file open failed\n", filename);
    } else {
        int count = 0;
        while ((fgets(buf, sizeof(buf), fp)) != NULL && count < line)
        {
            printf("%s", buf);
            count++;
        }
        
    }
}