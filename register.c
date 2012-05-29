/*
 * register.c
 *
 *  Created on: 2012-1-8
 *      Author: poseidon
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "define.h"
#include "net.h"
#include "proc_packet.h"
#include "cgic.h"
#include "getip.h"

 int cgiMain()
{
	 char *filename = "ip_config.conf";
	 char ip[16];
	 unsigned short portt = 8887;
	 int fd;
	 int readnd = 0;
	 char buffer[512];
	 getip(filename, ip);
	 fd = tcp_init_client(ip, portt);

	 struct replay_packet * replay;

	 struct register_struct re_struct;

	 re_struct.head.len = sizeof(struct user);
	 re_struct.head.encrpyt = ENCRPYT_NO;
	 re_struct.head.mo = MO_CLIENT;
	 re_struct.head.ttl = 0;
	 re_struct.head.extent = 0;
	 re_struct.head.ki = KI_REGISTER;

	 strcpy(re_struct.user_struct.username, "");
	 strcpy(re_struct.user_struct.password,  "");

	 char username[50];
	 char password[50];
	 cgiHeaderContentType("text/html");
	 cgiFormString("username", username, 50);
	 cgiFormString("password", password, 50);
	 strcpy(re_struct.user_struct.username, username);
	 strcpy(re_struct.user_struct.password, password);
	 writen(fd, (void *)&re_struct, sizeof(struct register_struct));
	 readnd = readn(fd, (void *)buffer, sizeof(struct replay_packet));
	 replay = (struct replay_packet *)buffer;

	 if (readnd == sizeof(struct replay_packet)) {
	 	 	if (replay->head.ki == KI_REPLAY) {
	 	 		if (replay->data == RPL_REGISTER_SUCCESS) {
	 	 			fprintf(cgiOut, "%d", 1);
	 	 		} else if (replay->data == RPL_REGISTER_FALLED) {
	 	 			 fprintf(cgiOut, "%d", 0);
	 	 		}
	 	 	}
	 	 }
	 return 0;
}

















































