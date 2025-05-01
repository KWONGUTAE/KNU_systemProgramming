#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

int main() {
int fd;

fd = open("nonexistentfile.txt", O_RDONLY);

if (fd == -1) {
printf("Error %d: %s\n", errno, strerror(errno));
} else {
printf("File opened.\n");
}

return 0;
}
