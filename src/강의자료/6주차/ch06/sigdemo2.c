/* sigdemo2.c - shows how to ignore a signal
 *	      - press Ctrl-\ to kill this one
 */

#include 	<stdio.h>
#include	<signal.h>

int main(void)
{
	signal( SIGINT, SIG_IGN );
	printf("you can't stop me!\n");
	while(1)
	{
		sleep(1);
		printf("haha\n");
	}	
	// 종료 원할 시 ctrl + 역슬래시 -> In Linux, 사용자가 터미널에서 종료키를 누를 때 사용하는 키.
	
	return 0;
}

