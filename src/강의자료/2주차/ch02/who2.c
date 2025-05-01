/* who2.c - read /var/adm/utmp and list info therein
*			     - suppresses empty records
*               - formats time nicely
*/
#include <stdio.h>
#include <unistd.h>
#include <utmp.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

#define SHOWHOST			/* include remote machine on output */
void showtime(long);
void show_info( struct utmp *utbufp );

int main()
{
	struct utmp *utbufp, *utmp_next();					/* read info into here */

	if( ( utmp_open(UTMP_FILE, O_RDONLY)) == -1 ) {
		perror( UTMP_FILE );				/* UTMP_FILE is in utmp.h */
		exit(1);
	}

	while( (utbufp = utmp_next()) != ((struct utmp *) NULL) ) {
		show_info(utbufp);
	}
	utmp_close();
	return 0;
}

/*
* show info()
*		display the contents of the utmp struct in human readale form
*		*displays nothing if record has no user name
*/
void show_info( struct utmp *utbufp )
{
	if( utbufp->ut_type != USER_PROCESS)		/* users only ! */
		return;

	printf("%-8.8s", utbufp->ut_name);	/* the logname */
	printf(" ");											/* a space		*/
	printf("%-8.8s", utbufp->ut_line);		/* the tty */
	printf(" ");											/* a space		*/
	showtime( utbufp->ut_time);
#ifdef SHOWHOST
	if( utbufp->ut_host[0] != '\0' )
		printf(" (%s) ", utbufp->ut_host);		/* the host */
#endif
	printf("\n");	
}

void showtime( long timeval )
/*
 * displays time in a format fit for human consumption
 * uses ctime to build a string then pics parts out of it
 * Note: %12.12s prints a string 12 chars wide and LIMITS it to 12 chars.
 */
{
	char *cp;								/* to hold address of time */
	cp = ctime(&timeval);			/* convert timeto stirng */
												/* string looks like Mon Feb 4 00:46:40 EST 1991 */
	printf("%12.12s", cp+4);	/* pick 12 chars from pos 4 */
}