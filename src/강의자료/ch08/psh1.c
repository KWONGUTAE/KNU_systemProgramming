/* psh1.c - prompting shell versin 1
	Prompts for the commandand its argments.
	Builds the argument vector for the call to execvp.
	Uses execvp(), and never returns.
*/

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#define MAXARGS	20		/* cmdline args */
#define ARGLEN	100		/* token length */

char *makestring(char*);
int execute(char*[]);		

int main(void)
{
	char* arglist[MAXARGS + 1];	/* an array of ptrs	*/
	int numargs = 0;		/* index into array	*/
	char argbuf[ARGLEN];		/* read stuff here	*/

	while ( numargs < MAXARGS )
	{
		printf ("Arg[%d]? ", numargs);
		if ( fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n' )
			arglist[numargs++] = makestring(argbuf);
		else
		{
			if ( numargs > 0 )		/* any args?	*/
			{
				arglist[numargs] = NULL;/* close list	*/
				execute( arglist );	/* do it	*/
				numargs = 0;		/* and reset	*/
			}
		}
	}
	return 0;
}

int execute(char* arglist[])
/*
 *	use execvp to do it
 */
{
	execvp(arglist[0], arglist); 	/* do it */
	perror("execvp failed");
	exit(1);
}

char* makestring( char* buf )
/*
 * trimoff newline and create storage for thestring
 */
{
	char *cp;
	
	buf[strlen(buf) - 1] = '\0';  /*trim newline */
	cp = malloc( strlen(buf) + 1 );
	if (cp == NULL)
	{
		fprintf(stderr, "no memory\n");
		exit(1);
	}
	strcpy(cp, buf);
	return cp;
}
