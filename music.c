/*
 * music.c
 *
 *  Created on: 2012-3-8
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
	 printf("IP is : %s\n", ip);
	 fd = tcp_init_client(ip, portt);


	 int musicid, passed;
	 struct replay_packet * replay;

	 struct common_packet request;

	 request.head.len = sizeof(request.data);
	 request.head.encrpyt = ENCRPYT_NO;
	 request.head.mo = MO_CLIENT;

	 request.head.ki = KI_BACK_MUSIC;

	 cgiHeaderContentType("text/html");
	 cgiFormInteger("musicid", &musicid, 0);
	 cgiFormInteger("passed", &passed, 0);
	 request.head.ttl = musicid;
	 request.head.extent = passed;

	 writen(fd, (void *)&request, sizeof(struct register_struct));
	 readnd = readn(fd, (void *)buffer, sizeof(struct replay_packet));
	 replay = (struct replay_packet *)buffer;

	 if (readnd == sizeof(struct replay_packet)) {
	 	if (replay->head.ki == KI_REPLAY) {
	 			fprintf(cgiOut, "music=%d;statuss=%d", lamp, replay->data);
	 	}
	 }
	 tcp_close(fd);
	 return 0;
}




























