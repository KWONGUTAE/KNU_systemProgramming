/* stdinredir1.c
 *	purpose: show how to redirect standard input by replacing file
 *	 action: read three lines from standard input, then
 *		 closes fd 0, opens a disk file, then reads in
 *		 three more lines from standard input
 */
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

void main(void)
{
	int fd;
	char line[100];
	
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);
	
	close(0);
	fd = open("/etc/passwd", O_RDONLY);
	if( fd != 0 )
	{
		fprintf(stderr,"Could not open data as fd()\n");
		//open(0, 1);
		exit(1);
	}
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);
}

