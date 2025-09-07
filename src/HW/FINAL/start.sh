#!/bin/bash

./server 9190 > server.log 2>&1 &
./client 127.0.0.1 9190 > client1.log 2>&1 &
./client 127.0.0.1 9190 > client2.log 2>&1 &

echo "서버와 클라이언트가 실행되었습니다."
