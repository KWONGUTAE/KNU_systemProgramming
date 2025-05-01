#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
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
        // file1.txt 존재 X
        printf("%s이 존재하지 않습니다.\n", av[1]);
    } else {
        // 바꾸고자 하는 파일이 존재하는 상재
        // file1.txt 존재 O
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
        } else {
            if (count_char_occurrences(av[2], '/') <= 1) {
                rename(av[1], av[2]);
            } else {
                char temp_str1[1024] = "";
                char temp_str2[1024] = "";
                // 제일 마지막 토큰이
                strcpy(temp_str1, av[2]);
                strcpy(temp_str2, av[2]);

                char *save_ptr1, *save_ptr2;
                char *tokenForSlash = strtok_r(temp_str1, "/", &save_ptr1);
                char *tokenForLast = strtok_r(temp_str2, "/", &save_ptr2);                              

                char temp_path[1024] = "";
                char *last_token = NULL;


                while (tokenForLast != NULL) {
                    last_token = tokenForLast;  // 마지막 토큰을 업데이트
                    tokenForLast = strtok_r(temp_str2, "/", &save_ptr2);  // 그 이후는 NULL을 넘겨주며 계속 나누기
                }

                while (tokenForSlash != NULL)
                {
                    printf("?");

                    if (strlen(temp_path) == 0) {  
                        snprintf(temp_path, sizeof(temp_path), "%s", tokenForSlash);
                    } else {
                        snprintf(temp_path, sizeof(temp_path), "%s/%s", temp_path, tokenForSlash);
                    }
                    printf("%s", temp_path);
                    struct stat stt;
                    if(stat(temp_path, &stt) != 0) {
                        printf("?");
                        if (tokenForLast && strcmp(tokenForLast, tokenForSlash) == 0) { // 앞까진 모두 존재하는데 마지막 파일 때문에 존재하지 않는다고 떴을 때
                            printf("??");
                            rename(av[1], av[2]);
                        } else {
                            printf("???");
                            printf("%s 파일 혹은 디렉토리가 존재하지 않습니다.\n", av[2]); // 그냥 중간에 

                        }
                    }

                    tokenForSlash = strtok_r(temp_str1, "/", &save_ptr1); 
                }
            }
        }
    }
    
    return 0;
}

int count_char_occurrences(const char *str, char ch) {
    int count = 0;

    // 문자열을 하나씩 순회하면서 원하는 문자가 나타날 때마다 count 증가
    while (*str) {
        if (*str == ch) {
            count++;
        }
        str++;
    }

    return count;
}