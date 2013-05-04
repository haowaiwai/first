#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/select.h>
#include <stdbool.h>
#include <fcntl.h>

#define BUF_SIZE 200

int main(int argc, char *argv[]) {
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;

	char bufSocket[BUF_SIZE];
	char bufSerial[BUF_SIZE];
	time_t ticks;
	bool bConnect = false;
	fd_set fdsr;
	int selectret;
	int iLen;
	char pDev[30];
	int port = 5000;

	if (argc != 3) {
		printf("\n Usage: %s serialdev port \n", argv[0]);
		return 1;
	}

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(bufSocket, '0', BUF_SIZE);
	memset(bufSerial, '0', BUF_SIZE);
	memset(pDev, '0', 30);
	strcpy(pDev, argv[1]);
	port = atoi(argv[2])

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

	listen(listenfd, 10);

	while (1) {
		connfd = accept(listenfd, (struct sockaddr*) NULL, NULL );
		if (bConnect) {
			close(connfd);
			bConnect = false;
			break;
		}
		int flags = fcntl(connfd, F_GETFL, 0);
		fcntl(connfd, F_SETFL, flags | O_NONBLOCK);
		//打开串口
		int fdSerial = opensensor(pDev);
		if (fdSerial == -1) {
			close(connfd);
			bConnect = false;
			break;
		}
		FD_ZERO(&fdsr);
		FD_SET(connfd, &fdsr);
		FD_SET(fdSerial, &fdsr);
		//超时的设定
		struct timeval tv;
		tv.tv_sec = 30;
		tv.tv_usec = 0;
		while (1) {
			printf("3333333333--%d--%d\n", connfd, fdSerial);
			if (connfd > fdSerial) {
				selectret = select(connfd + 1, &fdsr, NULL, NULL, &tv);
			} else {
				selectret = select(fdSerial + 1, &fdsr, NULL, NULL, &tv);
			}
			printf("3333333333----%d\n", selectret);
			if (selectret < 0) {
				close(connfd);
				bConnect = false;
				break;
			} else if (selectret == 0) {
				continue;
			} else {
				if (FD_ISSET(connfd, &fdsr)) {
					printf("11111111\n");
					int ret = recv(connfd, bufSocket, BUF_SIZE, 0);
					printf("11111111--%d\n", ret);
					if (ret <= 0) {
						close(connfd);
						FD_CLR(connfd, &fdsr);
					} else {
						write_port(fdSerial, bufSocket, ret);
					}
				}
				if (FD_ISSET(fdSerial, &fdsr)) {
					printf("2222222222222\n");
					while ((iLen = read_port(fdSerial, bufSerial, BUF_SIZE)) > 0) {
						printf("222222222222--%d\n", iLen);
						write(connfd, bufSerial, iLen);
						sleep(1);
					}
				}
			}
		}
		//关闭串口
		sleep(1);
	}
}

int opensensor(char *pDev) {
	int iResult = -1;
	int fd = -1, iBaudRate, iDataSize, iStopBit;
	char cParity;

	fd = open_port(pDev);
	if (fd < 0) {
		return -1;
	}

	iBaudRate = 115200;
	iDataSize = 8;
	cParity = 'N';
	iStopBit = 1;
	iResult = set_port(fd, iBaudRate, iDataSize, cParity, iStopBit);
	if (iResult < 0) {
		return -1;
	} else {
		return fd;
	}
}
