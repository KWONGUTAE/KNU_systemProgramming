#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

void do_ls(const char *dirname);

int main(int ac, char *av[]) {
    if (ac == 1) {
        // 실행: ./ls_R
        do_ls(".");
    } else {
        printf("다시 입력해주세요.\n");
    }

    return 0;
}

void do_ls(const char *dirname) {
    DIR *dir_ptr;
    struct dirent *dirent_ptr;
    
    if ((dir_ptr = opendir(dirname)) == NULL) {
        perror("opendir");
        return;
    }

    printf("%s:\n", dirname);  // 현재 디렉토리 출력

    char **subdirs = NULL;
    int subdir_count = 0;

    while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
        if (strcmp(dirent_ptr->d_name, ".") == 0 || strcmp(dirent_ptr->d_name, "..") == 0)
            continue;  // 현재 디렉토리(.)와 부모 디렉토리(..) 제외
        
        // 파일 및 디렉토리 전체 경로 생성
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dirname, dirent_ptr->d_name);

        struct stat st;
        if (stat(full_path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                // 디렉토리면 리스트에 저장 후 나중에 재귀 호출
                subdirs = realloc(subdirs, sizeof(char*) * (subdir_count + 1));
                subdirs[subdir_count] = strdup(full_path);
                subdir_count++;
            }
        }

        printf("%s\n", dirent_ptr->d_name);
    }

    closedir(dir_ptr);

    // 하위 디렉토리 탐색 (재귀 호출)
    for (int i = 0; i < subdir_count; i++) {
        printf("\n");
        do_ls(subdirs[i]);
        free(subdirs[i]);
    }
    free(subdirs);
}
