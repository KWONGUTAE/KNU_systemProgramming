/* varlib.h */

int 	VLstore(char *, char *);
char 	*VLlookup(char *);
int 	VLexport(char *);
void 	VLlist(void);
int 	VLenviron2table(char *[]);
char 	**VLtable2environ(void);

