#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define MAXCOUNT 3

int is_prime(unsigned long num);
void signal_handler(int signum);

int CallSignalCount = 0;
unsigned long cnt = 1;
unsigned long p = 0;

int main() {
    signal(SIGINT, signal_handler);

    while (1)
    {
        int bool = is_prime(cnt);
        if (bool == 1 && cnt != 1) {
            p = cnt;
        }
        cnt++;
    }
    

    return 0;
}

int is_prime(unsigned long num) {
    for (unsigned long i = 2; i < num; i++) {
        if(num % i == 0) {
            return 0;
        }
    }
    return 1;
}

void signal_handler(int signum) {
    CallSignalCount++;    
    printf("\n현재까지 확인한 수: %lu개\n", cnt);
    printf("가장 큰 소수: %lu\n", p);    
    printf("Ctrl + C %d회 입력\n\n", CallSignalCount);
    if (CallSignalCount == MAXCOUNT) {
        printf("SIGINT %d번 발생\n\n", MAXCOUNT);
        exit(130);
    }
}