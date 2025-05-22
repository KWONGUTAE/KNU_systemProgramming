// 2021115744 권구태
#include <unistd.h> 
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>	
#include <string.h>
#include <unistd.h>

#define   INPUTLEN   100


int set_ticker( int );
void countdown(int signum);
void inthandler(int s);

int count = 0;
int flag = 0;

int main(void)
{
    struct sigaction newhandler;	
	sigset_t	 oldmask, newmask;	
	void 		 inthandler();	
	char 		 x[INPUTLEN];

    newhandler.sa_handler = inthandler;
	newhandler.sa_flags = SA_RESTART;

    sigemptyset(&newmask);			
    sigaddset(&newmask, SIGALRM);		

	signal (SIGALRM, countdown);
	if ( set_ticker(1000) == -1 )
		perror("set_ticker");
	else {
        if ( sigaction(SIGINT, &newhandler, NULL) == -1 )
		 perror("sigaction");
        else
            while( 1 ) {
                if (flag == 0) {
                    pause();
                }
                fgets(x, INPUTLEN, stdin);
                if (strncmp(x, "quit", 4) == 0) {
                    printf("Exiting. Final tick: %d\n", count);
                    exit(1);
                } else if (strncmp(x, "stop", 4) == 0) {
                    flag = 1;
                    printf("Timer stopped.\n");
                    sigprocmask(SIG_BLOCK, &newmask, &oldmask);
                } else if (strncmp(x, "resume", 6) == 0) {
                    flag = 0;
                    printf("Timer resumed.\n");
                    sigprocmask(SIG_SETMASK, &oldmask, NULL);
                } else {
                    printf("잘못된 값 입력.\n");
                }
            }
    }
	return 0;
}

void countdown(int signum)
{
    count++;
	printf("Tick: %d\n", count);
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

void inthandler(int s)
{
	printf("Called with signal %d\n", s);
	sleep(s);
	printf("done handling signal %d\n", s);
}