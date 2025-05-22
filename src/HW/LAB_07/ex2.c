// 2021115744 권구태
#include 	<unistd.h> 	//for pause()
#include        <stdio.h>
#include        <sys/time.h>
#include        <signal.h>
#include        <stdlib.h>	// for exit()

int set_ticker( int );

int main(void)
{
	void  countdown(int); 
	signal (SIGALRM, countdown);
	if ( set_ticker(1000) == -1 )
		perror("set_ticker");
	else
		while( 1 )
			pause();
	return 0;
}

void countdown(int signum)
{
	printf("Tick\n");
	fflush(stdout);
}

int set_ticker( int n_msecs )
{
	struct itimerval new_timeset;
	long    n_sec, n_usecs;
	
	n_sec = n_msecs / 1000 ;			/* int part*/
	n_usecs = ( n_msecs % 1000 ) * 1000L ;  	/* remainder*/

	new_timeset.it_interval.tv_sec  = n_sec;        /* set reload       */
	new_timeset.it_interval.tv_usec = n_usecs;      /* new ticker value */
	new_timeset.it_value.tv_sec     = n_sec  ;      /* store this       */
	new_timeset.it_value.tv_usec    = n_usecs ;     /* and this         */

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}