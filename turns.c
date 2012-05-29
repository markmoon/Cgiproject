/*
 * lamps.c
 *
 *  Created on: 2012-2-13
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


	 int turn, stas, passed;
	 struct replay_packet * replay;

	 struct common_packet request;

	 request.head.len = sizeof(request.data);
	 request.head.encrpyt = ENCRPYT_NO;

	 request.head.ki = KI_LIGHT;

	 cgiHeaderContentType("text/html");
	 cgiFormInteger("turn", &turn, 0);
	 cgiFormInteger("stas", &stas, 0);
	 cgiFormInteger("passed", &passed, 0);
	 request.head.ttl = turn;
	 request.head.mo = stas;
	 request.head.extent = passed;

	 writen(fd, (void *)&request, sizeof(struct register_struct));
	 readnd = readn(fd, (void *)buffer, sizeof(struct replay_packet));
	 replay = (struct replay_packet *)buffer;

	 if (readnd == sizeof(struct replay_packet)) {
	 	if (replay->head.ki == KI_REPLAY) {
	 			fprintf(cgiOut, "turn=%d;statuss=%d", turn, replay->data);
	 	}
	 }
	 tcp_close(fd);
	 return 0;
}




























