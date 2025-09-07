#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    struct stat statbuf;

    if (stat("linux.txt", &statbuf) == -1) {
        perror("stat linux.txt");
        exit(1);
    }
    printf("Before link(): linux.txt link count= %d\n", (int)statbuf.st_nlink);

    if (link("linux.txt", "linux.ln") == -1) {
        perror("link");
        exit(1);
    }

    if (stat("linux.txt", &statbuf) == -1) {
        perror("stat linux.txt");
        exit(1);
    }
    printf("After link(): linux.txt link count= %d\n", (int)statbuf.st_nlink);

    if (stat("linux.ln", &statbuf) == -1) {
        perror("stat linux.ln");
        exit(1);
    }
    printf("After link(): linux.ln link count= %d\n", (int)statbuf.st_nlink);

    return 0;
}
