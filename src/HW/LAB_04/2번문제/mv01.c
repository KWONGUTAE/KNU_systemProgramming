#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int count_char_occurrences(const char *str, char ch);

int main(int ac, char *av[]) {
    struct stat buf;

    if (ac != 3) {
        printf("잘못된 명령어 입니다.\n");
        return 1;
    }

    if (stat(av[1], &buf) != 0) {
        // 바꾸고자 하는 파일이 없는 상태
        printf("%s이 존재하지 않습니다.\n", av[1]);
    } else {
        // 바꾸고자 하는 파일이 존재하는 상태
        struct stat st;
       
 
 
        if (stat(av[2], &st) == 0) {
            // 바꾸고자 하는 이름의 파일이 있는 상태
            if (S_ISDIR(st.st_mode)) {
                //./mv01 file1.txt ./a(존재)
 
 
                char result[1024];  // 충분한 크기의 배열 선언 (버퍼 오버플로우 주의)
                result[0] = '\0';   // 문자열을 빈 문자열로 초기화
 
 
                strcat(result, av[2]);
                strcat(result, "/");
                strcat(result, av[1]);
 
 
                rename(av[1], result);
            } else {
                //./mv01 file1.txt file2.txt(file2가 정의되어있는 상태)
                rename(av[1], av[2]);
            }
 
 
        } else
            // 바꾸고자 하는 이름의 파일이 없는 상태
            //./mv01 file1.txt ./a/file2.txt 또는
            //./mv01 file1.txt file2.txt(file2가 정의되어있지 않은 상태)
           
            if (count_char_occurrences(av[2], '/') <= 1) {
                printf("디렉토리가 존재하지 않습니다.\n");
            } else {
                if (rename(av[1], av[2]) == -1) {
                    printf("디렉토리가 존재하지 않습니다.\n");
                }
            }
 
        }
    return 0;
}

int count_char_occurrences(const char *str, char ch) {
    int count = 0;
    while (*str) {
        if (*str == ch) {
            count++;
        }
        str++;
    }
    return count;
}
