/* twordcount1.c - threaded word counter for two files. Version 1 */

#include <stdio.h>
#include <ctype.h>
#include <pthread.h>

#include <stdlib.h>

int	total_words;

int main( int ac, char *av[] )
{
	pthread_t	t1, t2;		/* two threads */
	void		*count_words(void *);

	if ( ac != 3)
	{
		printf("usage: %s file1 file2\n", av[0]);
		exit(1);
	}

	total_words = 0;
	pthread_create( &t1, NULL, count_words, (void*)av[1]);
	pthread_create( &t2, NULL, count_words, (void*)av[2]);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	printf( "%5d: total words\n", total_words);
	
	return 0;
}

void *count_words(void *f)
{
	char *filename = (char *)f;
	FILE *fp;
	int c, prevc = '\0';

	if(( fp = fopen( filename, "r")) != NULL ) {
		while (( c = getc(fp)) != EOF ) {
			if( !isalnum(c) && isalnum(prevc)) 
			/*
			이전 char는 영문자(a-z, A-Z)**이거나 **숫자(0-9)**이면 **
			0이 아닌 값(참) 이면 true리턴이니까, 
			현재꺼가 앞의 구간내의 문자만 아니면 단어라고 인식할 수 있음
			*/
				total_words++;
			prevc = c; 
		}
		fclose(fp);
	}
	else
		perror(filename);
	
	return NULL;
}
