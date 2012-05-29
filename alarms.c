/*
 * alarm.c
 *
 *  Created on: 2012-2-29
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

	 struct replay_packet * replay;
	 struct common_packet request;
	 struct alarm_data data;

	 request.head.len = sizeof(request.data);
	 request.head.encrpyt = ENCRPYT_NO;
	 request.head.mo = MO_CLIENT;

	 request.head.ki = KI_ALARM;

	 cgiHeaderContentType("text/html");

	 int passed, id, set, hour, minute, fangjian, yinyue, sun, mon, tue, wed, thu, fri, sat;
	 cgiFormInteger("passed", &passed, 0);
	 cgiFormInteger("id", &id, 0);
	 cgiFormInteger("set", &set, 0);
	 cgiFormInteger("hour", &hour, 0);
	 cgiFormInteger("minute", &minute, 0);
	 cgiFormInteger("fangjian", &fangjian, 0);
	 cgiFormInteger("yinyue", &yinyue, 0);
	 cgiFormInteger("sun", &sun, 0);
	 cgiFormInteger("mon", &mon, 0);
	 cgiFormInteger("tue", &tue, 0);
	 cgiFormInteger("wed", &wed, 0);
	 cgiFormInteger("thu", &thu, 0);
	 cgiFormInteger("fri", &fri, 0);
	 cgiFormInteger("sat", &sat, 0);

	 char tishi[244];
	 cgiFormStringNoNewlines("tishi", tishi, 244);

	 data.id = id;
	 data.set = set;
	 data.hour = hour;
	 data.minute = minute;
	 data.fangjianid = fangjian;
	 data.yinyue = yinyue;
	 data.sun = sun;
	 data.mon = mon;
	 data.tue = tue;
	 data.wed = wed;
	 data.thu = thu;
	 data.fri = fri;
	 data.sat = sat;
	 memcpy(data.tishi, tishi, 244);

	 request.head.extent = passed;
	 memset(request.data, 0, 256);
	 memcpy(request.data, &data, 256);



	 writen(fd, (void *)&request, sizeof(struct register_struct));
	 readnd = readn(fd, (void *)buffer, sizeof(struct replay_packet));
	 replay = (struct replay_packet *)buffer;

	 if (readnd == sizeof(struct replay_packet)) {
	 	if (replay->head.ki == KI_REPLAY) {
	 			fprintf(cgiOut, "id=%d;replay=%d", id, replay->data);
	 	}
	}
	 tcp_close(fd);
	 return 0;
}



























