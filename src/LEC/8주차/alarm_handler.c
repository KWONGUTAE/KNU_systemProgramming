#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void alarm_handler(int signum) {
    printf("Alarm! Signal number %d recevied\n", signum);
}

int main() {
    signal(SIGALRM, alarm_handler);
    alarm(7);
    printf("Waiting for alarm signal...\n");

    for (int i = 0; i <= 10; i++) {
        printf("Doing some work...%d\n", i);
        sleep(1);
    }
    return 0;
}