/* changeenv.c - shows how to change the environment
 *		 note : calls "env" to display its new settings
 */
#include	<stdio.h>
#include	<unistd.h>

extern char **environ;

int main(void)
{
	char *table[3];

	table[0] = "TERM=vt100";	/* fill the table */
	table[1] = "HOME=/on/the/range";
	table[2] = 0;

	environ = table;

	execlp("env", "env", NULL);

	return 0;
}
