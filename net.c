/*
 * net.c
 *
 *  Created on: 2011-11-6
 *      Author: poseidon
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "types.h"
#include "define.h"
#include "net.h"


int tcp_init_server(int port) {
	int fd;
	struct sockaddr_in addr;
	//int addr_len = sizeof(struct sockaddr_in);
	int optval = 1;
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return -1;
	}
	printf("Listen sock: %d\n", fd);
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	fprintf(stderr, "%x\n", INADDR_ANY);

	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		sleep(5);
		if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			perror("bind");
			return -1;
		}
	}
	if (listen(fd, MAX_CONNECTED) < 0) {
		perror("listen");
		return -1;
	}
	return fd;
}

int tcp_init_client(char ip[16], unsigned short port) {
	int fd;
	//char ip[16] = "127.0.0.1";
	struct hostent *host;
	if ((host = gethostbyname(ip)) == NULL) {
		fprintf(stderr,"Gethostname error\n");
		 exit(1);
	}
	struct sockaddr_in addr;
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "Create Socket Error!");
		return -1;
	}
	bzero((void *)&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr = *((struct in_addr *)host->h_addr);

	fprintf(stderr, "port:%x\n", port);
	fprintf(stderr, "htons(port) :%x\n", htons(port));

	fprintf(stderr, "server ip:port: %x : %d \n", addr.sin_addr.s_addr, addr.sin_port);

	if (connect(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) < 0) {
		fprintf(stderr, "Connect Server error!");
		return -1;
	}
	return fd;

}


void tcp_close(int fd) {
	close(fd);
}

ssize_t readn(int fd, void * vptr, size_t n) {
	ssize_t nread;
	size_t nleft;
	char *ptr;
	ptr = vptr;
	nleft = n;
	while(nleft > 0) {
		if ((nread = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR) {
				nread = 0;
			} else {
				return -1;
			}
		} else if (nread == 0) {
			break;
		}
		nleft -= nread;
		ptr += nread;
	}

	return (n-nleft);

}

ssize_t writen(int fd, const void * vptr, size_t n) {
	size_t nleft;
	ssize_t nwriten;
	const char *ptr;
	ptr = vptr;
	nleft = n;
	while(nleft > 0) {
		if ((nwriten = write(fd, ptr, nleft)) <= 0) {
			if (nwriten < 0 && errno == EINTR) {
				nwriten = 0;
			} else {
				return -1;
			}
		}
		nleft -= nwriten;
		ptr += nwriten;
	}
	return n;
}





















