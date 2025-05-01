#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define max 1024

void do_ls(char dirname);

int main(int ac, char* av[]) {
    if (ac == 1) {
        do_ls(".");
    } else {
        printf("다시 입력해주세요.");
    }
}

void do_ls(const char* dirname) {
    DIR* dir_ptr = NULL;
    struct dirent *dirent_ptr = NULL;

    if ((dir_ptr = opendir(dirname)) == NULL) {
        perror("opendir");
        return;
    }

    printf("%s:\n", dirname);

    char **subdirs = NULL;
    int subdir_count = 0;

    while ((dirent_ptr = readdir(dir_ptr)))
    {
        if (strcmp(dirent_ptr->d_name, ".") == 0 || strcmp(dirent_ptr->d_name, "..") == 0) {
            continue;
        }

        char full_path[max];
        snprintf(full_path, sizeof(full_path), "%s/%s", dirname, dirent_ptr->d_name);

        struct stat st;
        if(stat(full_path, &st) == 0) {
            if(S_ISDIR(st.st_mode)) {
                subdirs = realloc(subdirs, sizof(char *) * (subdir_count + 1));
                subdirs[subdir_count] = strdup(full_path);
                subdir_count++;
            }
        }

        printf("%s\n", dirent_ptr->d_name);
    }

    for (int i = 0; i < subdir_count; i++) {
        printf("\n");
        do_ls(subdir[i]);
        free(subdirs[i]);
    }
    free(subdirs);
}