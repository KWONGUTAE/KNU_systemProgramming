#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>

struct termios orig_termios;
ino_t initial_inode;

// 터미널 모드 복구용 함수
void reset_terminal_mode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

// 터미널을 non-canonical, no-echo 모드로 변경
void set_terminal_mode() {
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);       // 현재 모드 저장
    atexit(reset_terminal_mode);                  // 종료 시 복구

    new_termios = orig_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);      // 라인 입력/에코 비활성화
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

// SIGINT 핸들러
void handle_sigint(int sig) {
    struct stat buf;
    stat(".", &buf);
    if (buf.st_ino != initial_inode) {
        printf("\nMoved!\n");
    } else {
        printf("\nSame location!\n");
    }
    exit(0);
}

int main() {
    struct stat buf;
    char cwd[1024];
    char ch;

    // 현재 디렉토리 inode 저장
    if (stat(".", &buf) == -1) {
        perror("stat");
        exit(1);
    }
    initial_inode = buf.st_ino;

    printf("Initial inode: %ld\n", (long)initial_inode);
    printf("\nPress any key to check current location...\n");

    // 시그널 핸들러 등록
    signal(SIGINT, handle_sigint);

    // 터미널 모드 설정
    set_terminal_mode();

    while (1) {
        // 1바이트 키 입력 대기
        if (read(STDIN_FILENO, &ch, 1) == 1) {
            if (stat(".", &buf) == -1) {
                perror("stat");
                exit(1);
            }

            if (getcwd(cwd, sizeof(cwd)) == NULL) {
                perror("getcwd");
                exit(1);
            }

            printf("Current inode: %ld\n", (long)buf.st_ino);
            printf("Current path: %s\n\n", cwd);
            printf("Press any key to check again (Ctrl-C to exit)...\n");
        }
    }

    return 0;
}


/**
 * 
 * 
 * 📘 문제: 내 위치를 알려줘!

다음 요구 사항을 만족하는 프로그램 whereami.c를 작성하시오.
🔧 요구 사항

    프로그램이 실행되면 현재 작업 디렉토리의 inode 번호를 출력한다.

    그런 다음 사용자가 아무 키나 입력할 때마다, 다음을 수행한다:

        입력 버퍼를 기다리지 않고 바로 반응(non-canonical mode, no-echo)

        현재 작업 디렉토리의 inode 번호와 pwd 명령어로 얻을 수 있는 절대 경로를 출력한다.

    사용자가 Ctrl-C(SIGINT)를 입력하면 다음을 수행한다:

        현재 디렉토리의 inode를 다시 확인하고,

        프로그램 시작 시의 inode와 비교하여 디렉토리 이동이 있었는지 판단한다.

        이동이 있었으면 "Moved!", 그렇지 않으면 "Same location!"을 출력하고 프로그램 종료.

💡 힌트

    getcwd() or realpath() → 경로 확인

    stat(".", &buf) → 현재 디렉토리의 inode 확인

    stty 설정 변경 (non-canonical, no-echo)

    signal(SIGINT, handler)로 시그널 처리

    read(STDIN_FILENO, ...)를 이용한 비차단 입력

✅ 출력 예시

Initial inode: 131073

Press any key to check current location...
[Key Pressed]
Current inode: 131073
Current path: /home/user/project

[Key Pressed]
Current inode: 131074
Current path: /home/user/project/subdir

^C
Moved!
 */