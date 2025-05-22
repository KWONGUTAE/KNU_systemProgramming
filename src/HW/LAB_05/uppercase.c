/* echostate.c
 *	reports current state of echo bit in tty driver for fd 0
 *	shows how to read attributes from driver and test a bit
 */

 #include <stdio.h>
 #include <termios.h>
 #include <stdlib.h>
 
 int main(void)
 {
     struct termios info;
     int rv;
 
     rv = tcgetattr( 1, &info );	/* read values from driver */
 
     if( rv == -1 ) {
         perror( "tcgetattr" );
         exit(1);
     }

     info.c_oflag ^= OLCUC;

     tcsetattr(1, TCSANOW, &info);
 
     if( info.c_oflag & OLCUC )
         printf("OLCUC bit is on\n");
     else
         printf("OLCUC bit is OFF\n");
 } 