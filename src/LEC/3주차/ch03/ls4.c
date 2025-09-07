/** ls4.c
 ** purpose - list contents of directory or directories
 ** action - if no args, use . else list files in args
 **/
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>


#define MAX 100

void do_ls(char[], int inverse);
void quickSort( char *strArr[], int left, int right );

int i = 0;

int main (int ac, char *av[])
{
    if( ac == 1)
		do_ls(".", 0);
    else if (ac == 2 && strcmp(av[1], "-r") == 0) {
		do_ls(".", 1);
	} else {
		printf("%s\n\n %d", av[1], ac);
		while( --ac ){
			printf("%s:\n", *++av );
			do_ls( *av , 0);
		}
	}
}

void do_ls( char dirname[], int inverse )
/*
 * list files in directory called dirname
 */
{
    DIR			*dir_ptr;	/* the directory */
    struct dirent	*direntp;	/* each entry	 */
	if( ( dir_ptr = opendir( dirname ) ) == NULL )
		fprintf(stderr, "ls4: cannot open %s\n", dirname);
	else
	{
		char *strArr[MAX];

		while ((direntp = readdir(dir_ptr)) != NULL) {
			strArr[i] = strdup(direntp->d_name); // 파일명 복사
			i++; 
		}
		closedir(dir_ptr);
		quickSort(strArr, 0, i - 1); // i == 11
		if (inverse) {
			for (int j = i - 1; j >= 0; j--) {
				printf("%s\n", strArr[j]);
			}
		} else {
			for (int j = 0; j < i; j++) {
				printf("%s\n", strArr[j]);
			}
		}
	}
}

void quickSort( char *strArr[], int left, int right ) {
	if (left >= right) return;  // 재귀 종료 조건

    int i = left; // 0
    int j = right; // 11
    char *pivot = strArr[(left + right) / 2]; // 5


	while (i <= j) {
		while (strcmp(strArr[i], pivot) < 0) i++;
		while (strcmp(strArr[j], pivot) > 0) j--;

		if (i <= j) {
            char temp[256];
            strcpy(temp, strArr[i]);
            strcpy(strArr[i], strArr[j]);
            strcpy(strArr[j], temp);
            i++;
            j--;
        }
	}

	if (left < j) quickSort(strArr, left, j - 1);
	if (i < right) quickSort(strArr, i + 1, right);
}    