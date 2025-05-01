#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void oops(char *, char *);

int main(int ac, char *av[])
{
    int in_fd, out_fd, n_chars;
    char buf[BUFFERSIZE];
    int interactive = 0; // -i 옵션 여부

    /* 옵션 처리 */
    if (ac < 3 || ac > 4) {
        fprintf(stderr, "usage: %s [-i] source destination\n", av[0]);
        exit(1);
    }

    /* -i 옵션이 있는지 확인 */
    if (ac == 4) {
        if (strcmp(av[1], "-i") == 0) {
            interactive = 1;
            av++;  // 옵션을 건너뛰고 source, destination을 맞춤
            ac--;
        } else {
            fprintf(stderr, "Invalid option: %s\n", av[1]);
            exit(1);
        }
    }

    /* 같은 파일인지 확인 */
    struct stat src_stat, dest_stat;
    if (stat(av[1], &src_stat) == 0 && stat(av[2], &dest_stat) == 0) {
        if (src_stat.st_ino == dest_stat.st_ino && src_stat.st_dev == dest_stat.st_dev) {
            fprintf(stderr, "%s: '%s'와 '%s'는 같은 파일입니다.\n", av[0], av[1], av[2]);
            exit(1);
        }
    }

    /* -i 옵션이 있으면 덮어쓰기 확인 */
    if (interactive && access(av[2], F_OK) == 0) {
        char response;
        fprintf(stderr, "%s: '%s'를 덮어쓸까요 (y/n)? ", av[0], av[2]);
        scanf(" %c", &response);
        if (response != 'y' && response != 'Y') {
            fprintf(stderr, "복사를 취소했습니다.\n");
            exit(1);
        }
    }

    /* 원본 파일 열기 */
    if ((in_fd = open(av[1], O_RDONLY)) == -1)
        oops("Cannot open ", av[1]);

    /* 대상 파일 생성 */
    if ((out_fd = creat(av[2], COPYMODE)) == -1)
        oops("Cannot create ", av[2]);

    /* 파일 복사 */
    while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
        if (write(out_fd, buf, n_chars) != n_chars)
            oops("Write error to ", av[2]);

    if (n_chars == -1)
        oops("Read error from ", av[1]);

    /* 파일 닫기 */
    if (close(in_fd) == -1 || close(out_fd) == -1)
        oops("Error closing files", "");

    return 0;
}

void oops(char *s1, char *s2)
{
    fprintf(stderr, "Error: %s ", s1);
    perror(s2);
    exit(1);
}
