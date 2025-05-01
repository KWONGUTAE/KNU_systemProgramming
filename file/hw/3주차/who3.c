#include <stdio.h>
#include <sys/types.h>
#include <utmp.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include "utmplib.h"

int utmp_open(char *filename);
struct utmp *utmp_next();
void utmp_close();
int utmp_reload();

#define SHOWHOST                   // 원격 호스트 정보를 포함하도록 설정

void showtime(long);
void show_info(struct utmp *utbufp);

int main()
{
    struct utmp *utbufp, *utmp_next();

	if (utmp_open(UTMP_FILE) == -1) {
		perror(UTMP_FILE);
		exit(1);
	}
	while ( (utbufp = utmp_next()) != ((struct utmp *) NULL)) {
		show_info(utbufp);
	}
	utmp_close();

	return 0;
}

void show_info(struct utmp *utbufp)
{
    if (utbufp->ut_type != USER_PROCESS)  // 사용자 프로세스만 출력
        return;

    printf("%-8.8s ", utbufp->ut_user);  // 로그인 이름
    printf("%-8.8s ", utbufp->ut_line);  // 터미널 이름
    showtime(utbufp->ut_tv.tv_sec);

#ifdef SHOWHOST
    if (utbufp->ut_host[0] != '\0')
        printf(" (%s)", utbufp->ut_host);  // 원격 호스트
#endif
    printf("\n");
}

void showtime(long timeval)
{
    char *cp = ctime(&timeval);
    printf("%12.12s", cp + 4);  // 월, 일, 시간만 출력
}