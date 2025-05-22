/* setecho.c
 *	usage: setecho [y|n]
 *	shows: how to read, change, reset tty attributes
 */

 #include 	<stdio.h>
 #include	<termios.h>
 #include	<stdlib.h>
 #include <string.h>    
 
 #define oops(s,x) { perror(s); exit(x); }
 
 int main ( int ac, char *av[])
 {
    struct termios newInfo, oldInfo;
	char	buf[BUFSIZ];
 
     if( ac != 1 )
         exit(0);
 
     if( tcgetattr(0, &oldInfo) == -1 ) {
        oops("tcgetattr", 1);
     }

     newInfo = oldInfo;

     newInfo.c_lflag ^= ECHO;

     if( tcsetattr(0, TCSANOW, &newInfo) == -1)	/* set attribs */
     oops("tcsetattr", 2);

    while (1)
    {
        printf("Enter password: ");
        fgets(buf, BUFSIZ, stdin);
        printf("\n");

        if (strcmp(buf, "quit\n") == 0){
            printf("\n");
            break;
        }
        printf("Your Password is: %s\n", buf);
    }

    if( tcsetattr(0, TCSANOW, &oldInfo) == -1)	/* set attribs */
     oops("tcsetattr", 2);

 
     return 0;
 }
 