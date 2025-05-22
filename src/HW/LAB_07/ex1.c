// 2021115744 권구태
 #include <stdio.h>
 #include <curses.h>
 
 #define LINES 100
 
 void clearly();

 int main(int ac, char *av[])
 {
    initscr();
    clear();

    if (ac == 1) {
        while (1)
        {
            addstr("Hello, World");	
            clearly();
        }
    } else {
        while (1)
        {
            addstr(av[1]);	
            clearly();
        }       
    } 
     endwin();
 
     return 0;
 }

 void clearly() {
    refresh();
    clear();
    sleep(1);
    refresh();
    clear();
    sleep(1);
 }
 