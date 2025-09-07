/* waitdemo1.c - shows how parent pauses until child finishes
 */

#include	<stdio.h>
#include 	<stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX 100

void main(int argc, char* arvg[])
{
    if (argc != 2) {
        printf("fork할 갯수 인자 추가\n");
        exit(1);
    }
    int  newpid;
    //void child_code(), parent_code(); 
	void child_code(int), parent_code(int);

    int forkNum = atoi(arvg[1]);
    int forkNumCp1 = forkNum;
    int forkNumCp2 = forkNum;
    int randNUM[MAX] = {0};
    while (forkNumCp1--)
    {
        randNUM[forkNumCp1] = ((rand() % 5) + 1);
    }
    
    while (forkNum--)
    {
        if ( (newpid = fork()) == -1 )
            perror("fork");
        else if ( newpid == 0 ) {
            child_code(randNUM[forkNum]);
        }
    }
    while (forkNumCp2--)
    {
        if ( newpid != 0 )
            parent_code(newpid);
    }


    printf("모든 자식 종료 완료\n");
}

/*
 * new process takes a nap and then exits
 */
void child_code(int delay)
{
	printf("자식프로세스 %d: %d초 동안 sleep\n", getpid(), delay);
	sleep(delay);
	exit(delay);
}


/*
 * parent waits for child then prints a message
 */
void parent_code(int childpid)
{
	int wait_rv;		/* return value from wait() */
	int child_status;
	int high_8;
	wait_rv = wait(&child_status);
	high_8 = child_status >> 8;	/* 1111 1111 0000 0000 */
    printf("부모: 자식 %d 종료, 종료코드: %d\n", childpid,high_8 );
}

