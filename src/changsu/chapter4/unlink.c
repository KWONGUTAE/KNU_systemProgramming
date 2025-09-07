#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    struct stat statbuf;

    if (stat("test.txt", &statbuf) == -1) {
        perror("stat test.txt");
        exit(1);
    }
    printf("test.txt: link count= %d\n", (int)statbuf.st_nlink);
    unlink("test.txt");

    if (stat("linux.ln", &statbuf) == -1) {
        perror("stat linux.ln");
        exit(1);
    }
    printf("Before unlink()linux.ln: link count= %d\n", (int)statbuf.st_nlink);

    unlink("linux.ln");

    if (stat("linux.txt", &statbuf) == -1) {
        perror("stat linux.ln");
        exit(1);
    }
    printf("After unlink(): linux.ln link count= %d\n", (int)statbuf.st_nlink);

    return 0;
}
