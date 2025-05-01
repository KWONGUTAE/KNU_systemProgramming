/* write0.c
 *
 *	purpose: send messages to another terminal
 *	 method: open the other terminal for output then
 *		 copy from stdin to that terminal
 *	  shows: a terminal is just a file supporting regular i/o
 *	  usage: write0 ttyname
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main( int ac, char *av[] )
{
	int 	fd;
	char	buf[BUFSIZ];

	/* check args */
	if( ac != 2 ){
		fprintf(stderr, "usage: write0 ttyname\n");
		exit(1);
	}

	/* open devices */
	fd = open( av[1], O_WRONLY );
	if( fd == -1 ) {
		perror(av[1]);
		 exit(1);
	}

	/* loop until EOF on input */
	while( fgets(buf, BUFSIZ, stdin) != NULL )
		if( write(fd, buf, strlen(buf)) == -1 ) // 파일이나 디바이스나 모두 같은 파일 취급. 디바이스도 실제론 디바이스 파일임.
			break;
	close( fd );
}

