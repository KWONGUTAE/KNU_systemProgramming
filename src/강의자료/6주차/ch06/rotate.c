/* rotate.c : map a->b, b->c, .. z->a
 *  urpose: useful for showing tty modes
 */

#include <stdio.h>
#include <ctype.h>

int main(void)
{
	int c;

	while( ( c=getchar())!= EOF ) {
		if( c == 'z' )
			c = 'a';
		else if ( islower(c) )
			c++;

		putchar(c);
	}

	return 0;	
}
