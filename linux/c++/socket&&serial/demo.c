/*************************************************************
 FileName : demo.c
 FileFunc : 测试串口应用
 Version  : V0.1
 Author   : Sunrier
 Date     : 2012-06-13
 Descp    : Linux下实现串口库
 *************************************************************/
#include <stdio.h>
#include <string.h>
#include "serialport.h"
#include <sys/select.h>

int main(int argc, char *argv[]) {
	int iResult = -1;
	int fd = -1, iBaudRate, iDataSize, iStopBit;
	char cParity;
	int iLen;
	char szBuffer[30];
	int i;
	if (argc != 2) {
		printf("\n Usage: %s serialdev \n", argv[0]);
		return 1;
	}

	fd = open_port(argv[1]);
	if (fd < 0) {
		perror("open_port error !");
		return 1;
	}

	iBaudRate = 115200;
	iDataSize = 8;
	cParity = 'N';
	iStopBit = 1;
	iResult = set_port(fd, iBaudRate, iDataSize, cParity, iStopBit);
	if (iResult < 0) {
		perror("set_port error !");
		return 1;
	}

	printf("fd = %d \n", fd);

	//memset(szBuffer,0,sizeof(szBuffer));
	//iLen = read_port(fd,szBuffer,5);
	iLen = write_port(fd, "Z", 1);
	printf("iLen =  %d\n", iLen);
	fd_set rd;
	FD_ZERO(&rd);
	FD_SET(fd, &rd);
	while (FD_ISSET(fd, &rd)) {
		if (select(fd + 1, &rd, NULL, NULL, NULL ) < 0) {
			perror("select1111111111111\n");
			sleep(1);
		}
		else {
			if (FD_ISSET(fd, &rd)) {
				iLen = read_port(fd, szBuffer, 8);
				printf("nread=%d \n", iLen);
				if (iLen > 0) {
					for (i = 0; i < iLen; i++) {
						printf("%02hhx ", szBuffer[i]);
					}
					printf("\n");
					sleep(1);
				} else if (iLen <= 0) {
					printf("error\n");
					break;
				}
			}
			else {
				perror("select2222222222\n");
				sleep(1);
			}
		}
	}
	//memset(szBuffer,0,sizeof(szBuffer));
	//sleep(2);
	//iLen = read_port(fd,szBuffer,5);
	//printf("iLen =  %d ,szBuffer = %s \n",iLen,szBuffer);
	//if( iLen>0 )
	//printf("Write byte success !\n");
	//szBuffer[iLen+1] = '\0';

	return 0;
}

