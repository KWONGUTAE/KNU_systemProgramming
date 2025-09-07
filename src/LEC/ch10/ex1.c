/*
 lab10_ex1
*/
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char line[100]; 
    char matrix[100][100];
    char temp[100];

	FILE *fp;

    fp = fopen(argv[1], "r");

	if(fp == NULL){
		fprintf(stderr, "Could not duplicate fd to 0\n");
		exit(1);
	} else {
        int i = 0;
        while (fgets(line, 100, fp) != NULL) {
            strcpy(matrix[i], line);
            i++;
        }

        // 문자열 정렬
        for (int j = 0; j < i; j++) {
            for (int k = j; k < i; k++) {
                if (strcmp(matrix[k], matrix[j]) < 0) {
                    strcpy(temp, matrix[k]);
                    strcpy(matrix[k], matrix[j]);
                    strcpy(matrix[j], temp);
                }
            }
        }

        for (int m = 0; m < i; m++) {
            printf("%s", matrix[m]);
        }
    }

    return 0;
}
