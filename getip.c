/*
 * getip.c
 *
 *  Created on: 2012-1-8
 *      Author: poseidon
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "getip.h"

int  getip(char *filename, char *ip) {
	int filefd;
	int ret;
	char *ptr;
	char temp[16];
	filefd = open(filename, 0444);
	if (filefd != 0) {
		fprintf(stderr, "Open file error!");
	}
	ret = read(filefd, temp, 16);
	close(filefd);
	if (ret > 0) {
		ptr = strchr(temp, '\n');
		if (ptr) {
			strncpy(ip, temp, (ptr-temp));
			ip[ptr-temp] = '\0';
		}
		return 0;
	}
	return -1;
}
