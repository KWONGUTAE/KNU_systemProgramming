#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

void do_ls(char *filename);
int get_inode(char* filename);
void do_ls_dir(char *filename, char *ptr);

int main(int ac, char* av[]) {
    if (ac == 2) {
        do_ls(".");
    } else if (ac == 3) {
        do_ls_dir(".", av[2]);
    } else {
        printf("다시 입려해주세요.\n");
    }
    return 0;
}

void do_ls(char *filename) {
    struct dirent *dirent_ptr;

    DIR *dir;

    if ((dir = opendir(filename)) == NULL) {
        perror("opendir");
        exit(1);
    }

    while ((dirent_ptr = readdir(dir)) != NULL)
    {
        printf("%d %s\n", get_inode(dirent_ptr->d_name), dirent_ptr->d_name);
    }
}

int get_inode(char* filename) {
    struct stat st_info;

    if(stat(filename, &st_info) == -1) {
        fprintf(stderr, "Cannot stat ");
        perror(filename);
        exit(1);
    }
    return st_info.st_ino;
}

void do_ls_dir(char *filename, char *ptr) {
    struct dirent *dirent_ptr;

    DIR *dir;

    // ptr이 들어왔고, 해당 ptr의 경로가 맞으면 그대로 출력 맞지 않으면 아니다 출력

    if (chdir(ptr) == -1) {
        printf("경로가 잘못되었습니다.\n");
        exit(1);
    } else {
        if ((dir = opendir(filename)) == NULL) {
            perror("opendir");
            exit(1);
        }

        while ((dirent_ptr = readdir(dir)) != NULL)
        {
            printf("%d %s\n", get_inode(dirent_ptr->d_name), dirent_ptr->d_name);
        }
    }
}

