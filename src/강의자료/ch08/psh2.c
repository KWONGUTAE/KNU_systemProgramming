/** prompting shell version 2 (psh2.c)
 **
 **	Soloves the 'one-shot'problem of version 1
 **		uses execvp(), but fork()s first so that the
 **		shell waits around to perform another command
 **	New problem: shell catches signals. Run vi, press ^c.
 **/

#include	<stdio.h>
#include	<signal.h>
#include	<string.h>
#include	<stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


#define	MAXARGS	   20	/* cmdline args	*/
#define	ARGLEN      100	/* token length	*/

char *makestring(char*);
void execute(char*[]);

int main(void)
{
	char	*arglist[MAXARGS+1];		/* an array of ptrs*/
	int	numargs = 0;			/* index into array*/
	char	argbuf[ARGLEN];			/* read stuff here	*/

	while ( numargs < MAXARGS )
	{					
		printf("Arg[%d]? ", numargs);
		if ( fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n' )
			arglist[numargs++] = makestring(argbuf);
		else if ( numargs > 0 ){		/* any args?	*/
				arglist[numargs]=NULL;	/* close list	*/
				execute( arglist );	/* do it	*/
				numargs = 0;		/* and reset	*/
			}
	}

	return 0;
}

void execute( char *arglist[] )
/*
 *	use fork and execvp and wait to do it
 */
{
	int	pid,exitstatus;				/* of child	*/

	pid = fork();					/* make new process */
	switch( pid ){
		case -1:	
			perror("fork failed");
			exit(1);
		case 0:
			execvp(arglist[0], arglist);		/* do it */
			perror("execvp failed");
			exit(1);
		default:
			while( wait(&exitstatus) != pid )
				;
			printf("child exited with status %d,%d\n",
					exitstatus>>8, exitstatus&0377); // C에서 숫자가 0으로 시작하면 8진수로 인식함.
	}
}

char *makestring( char *buf )
/*
 * trim off newline and create storage for the string
 */
{
	char	*cp /* *malloc() */;

	buf[strlen(buf)-1] = '\0';		/* trim newline	*/
	cp = malloc( strlen(buf)+1 );		/* get memory	*/
	if ( cp == NULL ){			/* or die	*/
		fprintf(stderr,"no memory\n");
		exit(1);
	}
	strcpy(cp, buf);		/* copy chars	*/
	return cp;			/* return ptr	*/
}
