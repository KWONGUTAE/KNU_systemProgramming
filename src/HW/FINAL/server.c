// 2021115744 권구태
// server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>   // waitpid
#include <poll.h>
#include <signal.h>

//cmd field value
#define READY 0    // Server -> Client
#define REQUEST 1  // Client -> Server
#define RESPONSE 2 // Server -> Client
#define END 3      // Server -> Client

// result 값
#define FAIL 0    // 해당 인덱스에 다른 클라이언트가 선택한 경우
#define SUCCESS 1 // 해당 인덱스를 선택하는 데 성공

#define BUF_SIZE 1024
#define MAX_CLNT 3

// Client -> Server (REQUEST 메시지)
typedef struct {
  int cmd; // cmd field value
  char filename[256];
  char filedata[BUF_SIZE];
} REQ_PACKET;

// Server -> Client (READY, RESPONSE, END 메시지)
typedef struct {
  int cmd;             // cmd field value
  char buf[BUF_SIZE];
  int result;
} RES_PACKET;

// 공유 메모리
int clnt_cnt = 0;
int clnt_socks[MAX_CLNT]; 
pthread_mutex_t mutx;

void *handle_clnt(void *arg);
void error_handling(char *msg);

void sigint_handler(int signo) {
  printf("\n[서버 종료] 모든 클라이언트 연결 해제 중...\n");
  for (int i = 0; i < clnt_cnt; i++) {
    close(clnt_socks[i]);
  }
  exit(0);
}

int main(int argc, char *argv[]) {
  signal(SIGINT, sigint_handler);  // Ctrl+C 감지
  int serv_sock, clnt_sock;
  struct sockaddr_in serv_adr, clnt_adr;
  socklen_t clnt_adr_sz;
  pthread_t t_id;
  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  pthread_mutex_init(&mutx, NULL);
  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  if (serv_sock == -1) error_handling("socket() error");

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family=AF_INET;
  serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
  serv_adr.sin_port=htons(atoi(argv[1]));
  if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
    error_handling("bind() error");
  if (listen(serv_sock, 5) == -1)
    error_handling("listen() error");

  while (1)
  {
    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
    if (clnt_sock == -1) continue;

    pthread_mutex_lock(&mutx);
    if (clnt_cnt >= MAX_CLNT) {
      pthread_mutex_unlock(&mutx);
      close(clnt_sock);
      continue; // 최대 클라이언트 수 초과시 연결 끊음
    }
    clnt_socks[clnt_cnt++] = clnt_sock;
    pthread_mutex_unlock(&mutx);

    pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
    pthread_detach(t_id);

    printf("Connected client IP: %s\n", inet_ntoa(clnt_adr.sin_addr));
  }

  close(serv_sock);
  return 0;
}

void *handle_clnt(void *arg) {
  int clnt_sock = *((int*)arg);
  REQ_PACKET recv_pckt;
  RES_PACKET send_pckt;

  // READY 전송
  send_pckt.cmd = READY;
  send_pckt.result = SUCCESS;
  memset(send_pckt.buf, 0, BUF_SIZE);
  write(clnt_sock, &send_pckt, sizeof(RES_PACKET));
  

  while (1)
  {

    // poll 구조체 준비
    struct pollfd pfd;
    pfd.fd = clnt_sock;
    pfd.events = POLLIN;

    int ret = poll(&pfd, 1, 30000); // 10초 대기

    if (ret == -1) {
      perror("poll error");
      break;
    } else if (ret == 0) {
      printf("Client %d timed out (no data for 30 seconds).\n", clnt_sock);
      break;  // 타임아웃 발생 시 연결 종료
    }
    // 클라이언트 요청 수신
    ssize_t str_len = read(clnt_sock, &recv_pckt, sizeof(REQ_PACKET));

    if (str_len <= 0) break;

    if (recv_pckt.cmd == REQUEST) {
      printf("Received filename: %s\n", recv_pckt.filename);
      printf("Received content: %s\n", recv_pckt.filedata);

      int pipefd[2];
      if (pipe(pipefd) == -1) {
        perror("pipe");
        break;
      }

      pid_t pid = fork();
      if (pid == 0) {
        // 자식 프로세스: 파일 생성 및 내용 저장
        close(pipefd[1]);
        REQ_PACKET child_pckt;
        read(pipefd[0], &child_pckt, sizeof(REQ_PACKET));
        close(pipefd[0]);

        FILE *fp = fopen(child_pckt.filename, "w");
        if (fp) {
          fputs(child_pckt.filedata, fp);
          fclose(fp);
          printf("Child: wrote to file %s\n", child_pckt.filename);
          exit(0);
        } else {
          perror("fopen");
          exit(1);
        }
      } else if (pid > 0) {
        // 부모 프로세스: 자식에게 구조체 전송
        close(pipefd[0]);
        write(pipefd[1], &recv_pckt, sizeof(REQ_PACKET));
        close(pipefd[1]);

        int status;
        waitpid(pid, &status, 0);

        send_pckt.cmd = RESPONSE;
        send_pckt.result = SUCCESS;
        strcpy(send_pckt.buf, "정상적으로 저장하였습니다.");
        snprintf(send_pckt.buf, BUF_SIZE, "파일 저장 완료 (exit code: %d)", WEXITSTATUS(status));
        write(clnt_sock, &send_pckt, sizeof(RES_PACKET));
      } else {
        perror("fork failed");
        break;
      }

    } else if (recv_pckt.cmd == END) {
      printf("[RX] cmd %d, END\n", recv_pckt.cmd);
      send_pckt.cmd = END;
      write(clnt_sock, &send_pckt, sizeof(RES_PACKET));
      break;
    } else {
      printf("Invalid cmd received.\n");
    }
  }

  pthread_mutex_lock(&mutx);
  for (int i = 0; i < clnt_cnt; i++) {
    if (clnt_sock == clnt_socks[i]) {
      for (int j = i; j < clnt_cnt - 1; j++)
        clnt_socks[j] = clnt_socks[j + 1];
      clnt_cnt--;
      break;
    }
  }
  pthread_mutex_unlock(&mutx);

  printf("Client %d disconnected.\n\n", clnt_sock);
  close(clnt_sock);

  return NULL;
}

void error_handling(char *msg) {
  perror(msg);
  exit(1);
}
