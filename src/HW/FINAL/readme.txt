# TCP 기반 파일 전송 서버 & 클라이언트

> 컴퓨터 네트워크 프로그래밍 과제  
> 2021115744 권구태

## 📌 개요

이 프로젝트는 TCP 소켓을 기반으로 한 파일 전송 시스템입니다. 클라이언트는 서버에 파일 이름과 내용을 전송하고, 서버는 해당 내용을 파일로 저장한 뒤 응답을 보냅니다. 서버는 최대 3개의 클라이언트를 동시에 처리할 수 있으며, 클라이언트의 비정상 종료나 응답 없음(타임아웃)을 처리하는 기능도 포함되어 있습니다.

---

## 📁 파일 구성

- `server.c` – TCP 기반 멀티스레드 서버 코드
- `client.c` – 사용자 입력을 받아 서버로 전송하는 클라이언트 코드
- `Makefile` – 서버와 클라이언트 프로그램 컴파일 자동화
- `start.sh`, `stop.sh` – 서버 실행 및 종료 스크립트
- `README.md` – 프로젝트 설명
- `report.md` – 리포트 문서 (별도 제공)

---

## ⚙️ 빌드 방법

```bash
$ make


$ ./server <port>
$ ./server 9000

$ ./client <server_ip> <port>
$ ./client 127.0.0.1 9000


# 클라이언트 실행 예시
파일 이름 입력 (또는 /exit): hello.txt
저장할 파일 내용 입력: Hello, World!

# 서버 출력 예시
Connected client IP: 127.0.0.1
Received filename: hello.txt
Received content: Hello, World!
Child: wrote to file hello.txt
