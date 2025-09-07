/* hello1.c
 *	purpose show the minimal calls needed to use curses
 *	outline initialize, draw stuff, wait for input, quit
 */

#include <stdio.h>
#include <curses.h>

#define LINES 100

int main(void)
{
	initscr();
	clear();

	move(10, 20);
	addstr("Hello, World");	
	move(LINES-1, 0);

	refresh();	
	getch();

	endwin();

	return 0;
}
