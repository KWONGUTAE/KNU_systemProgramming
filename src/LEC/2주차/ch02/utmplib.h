// utmplib.h - 함수 원형 선언
#ifndef UTMLIB_H
#define UTMLIB_H

#include <utmp.h>

// 함수 원형(프로토타입) 선언
int utmp_open(char *filename);
struct utmp *utmp_next();
int utmp_reload();
void utmp_close();

#endif
