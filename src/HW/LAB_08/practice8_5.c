#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    FILE *fp;
    int pid;
    char msg1[] = "Testing 1 2 3 ..\n";
    char msg2[] = "Hello, hello\n";

    if ((fp = fopen("testfile2", "w")) == NULL) {
        return 0;
    }
    fprintf(fp, "%s", msg1);
    if ((pid = fork()) == -1) {
        return 0;
    }
    fprintf(fp, "%s", msg2);

    fclose(fp);
    return 1;
}
