/* rotate.c : map a->b, b->c, .. z->a
 *  urpose: useful for showing tty modes
 */

 #include <stdio.h>
 #include	<termio.h>
 #include <ctype.h>


 void set_cr_noecho_mode(void);
 void tty_mode(int);
 
 int main(void)
 {
    

     int c;

     tty_mode(0);				/* restore tty mode	*/
 	 set_cr_noecho_mode();			/* set -icanon, -echo	*/

     while( ( c=getchar())!= 'Q' ) {
         if( c == 'z' )
             c = 'a';
         else if ( islower(c) )
             c++;
 
         putchar(c);
     }

     tty_mode(1);				/* restore tty mode	*/

 
     return 0;	
 }

 void set_cr_noecho_mode(void)
/*
 * purpose: put file descriptor 0 into chr-by-chr mode and noecho mode
 *  method: use bits in termios
 */
{
	struct termios ttystate;
	
	tcgetattr( 0, &ttystate);		/* read curr.  setting	*/
	ttystate.c_lflag	&= ~ICANON;	/* no buffering		*/
	ttystate.c_lflag	&= ~ECHO;	/* no echo either	*/	
	ttystate.c_cc[VMIN]	= 1;		/* get 1 char at a time */
	tcsetattr( 0, TCSANOW, &ttystate);	/* install settings 	*/
}

/* how == 0 => save current mode, how == 1 ==> restore mode */
void tty_mode(int how)
{
	static struct termios original_mode;
	if( how == 0 )
		tcgetattr(0, &original_mode);
	else
		tcsetattr(0, TCSANOW, &original_mode);

}
 