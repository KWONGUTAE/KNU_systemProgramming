// 2021115744 권구태
// client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>

//cmd field value
#define READY 0    // Server -> Client
#define REQUEST 1  // Client -> Server
#define RESPONSE 2 // Server -> Client
#define END 3      // Server -> Client

#define FAIL 0
#define SUCCESS 1
#define BUF_SIZE 1024

typedef struct {
  int cmd;
  char filename[256];
  char filedata[BUF_SIZE];
} REQ_PACKET;

typedef struct {
  int cmd;
  char buf[BUF_SIZE];
  int result;
} RES_PACKET;

pthread_mutex_t mutx;
int flag = 1;

void *send_msg(void *arg);
void *recv_msg(void *arg);
void error_handling(char *msg);

pthread_t snd_thread;

int main(int argc, char *argv[]) {
  int sock;
  struct sockaddr_in serv_addr;
  pthread_t rcv_thread;
  void *thread_return;

  if (argc != 3) {
    printf("Usage : %s <IP> <port>\n", argv[0]);
    exit(1);
  }

  pthread_mutex_init(&mutx, NULL);
  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1) error_handling("socket() error");

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));

  if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    error_handling("connect() error");

  pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
  pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);

  pthread_join(snd_thread, &thread_return);
  pthread_join(rcv_thread, &thread_return);

  close(sock);
  return 0;
}

void *send_msg(void *arg) {
  int sock = *((int*)arg);
  REQ_PACKET send_pckt;

  while (flag) {
    printf("파일 이름 입력 (또는 /exit): ");
    if (fgets(send_pckt.filename, 256, stdin) == NULL) break;
    send_pckt.filename[strcspn(send_pckt.filename, "\n")] = 0;

    if (strncmp(send_pckt.filename, "/exit", 5) == 0) {
      send_pckt.cmd = END;
      memset(send_pckt.filedata, 0, BUF_SIZE);
    } else {
      printf("저장할 파일 내용 입력: ");
      if (fgets(send_pckt.filedata, BUF_SIZE, stdin) == NULL) break;
      send_pckt.filedata[strcspn(send_pckt.filedata, "\n")] = 0;
      send_pckt.cmd = REQUEST;
    }

    write(sock, &send_pckt, sizeof(REQ_PACKET));
    printf("[Tx] cmd: %d, filename: %s, filedata: %s\n\n",
           send_pckt.cmd, send_pckt.filename, send_pckt.filedata);
  }
  return NULL;
}

void *recv_msg(void *arg) {
  int sock = *((int*)arg);
  RES_PACKET recv_pckt;
  int len;

  while (1) {
    len = read(sock, &recv_pckt, sizeof(recv_pckt));
    if (len <= 0) {
      printf("[서버 연결 종료 감지됨]\n");
      pthread_mutex_lock(&mutx);
      flag = 0;
      pthread_mutex_unlock(&mutx);
      pthread_cancel(snd_thread);  // fgets 대기 중인 스레드 종료
      break;
    }

    if (recv_pckt.cmd == READY) {
      printf("서버 준비 완료\n");
    } else if (recv_pckt.cmd == RESPONSE) {
      printf("[Rx] cmd: %d, result: %d. %s\n", recv_pckt.cmd, recv_pckt.result, recv_pckt.buf);
    } else if (recv_pckt.cmd == END) {
      printf("[Rx] cmd: %d, result: %d\n", recv_pckt.cmd, recv_pckt.result);
      printf("END. 프로그램 종료\n");
      pthread_cancel(snd_thread);
      break;
    } else {
      printf("Invalid cmd\n");
      exit(1);
    }
  }

  return NULL;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}
