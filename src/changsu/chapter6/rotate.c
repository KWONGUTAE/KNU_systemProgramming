#include <stdio.h>
#include <ctype.h>

int main() {
    int c = -1;
    while ((c = getchar()) != EOF)
    {
        if (c == 'z') {
            c = 'a';
        } else if (islower(c)) {
            c += 1;
        }
        putchar(c);
    }
    return 0;
    
}