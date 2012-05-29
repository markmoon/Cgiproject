/*
 * getstatus.c
 *
 *  Created on: 2012-2-14
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

	 int device, number, passed;
	 struct replay_packet * replay;
	 struct common_packet * replay_common;
	 ctl_hdr * hdr;
	 struct common_packet request;

	 request.head.len = sizeof(request.data);
	 request.head.encrpyt = ENCRPYT_NO;

	 cgiFormInteger("device", &device, 0);
	 cgiFormInteger("number", &number, 0);
	 cgiFormInteger("passed", &passed, 0);
	 cgiHeaderContentType("text/html");
	 request.head.ttl = device;
	 request.head.mo = number;
	 request.head.extent = passed;
	 switch (device) {
	 case 101:
		 //light
		 request.head.ki = KI_LIGHT;
		 break;
	 case 102:
		 request.head.ki = KI_CURTAINS;
		 break;
	 case 104:
		 request.head.ki = KI_GETALARM;
		 break;
	 case 203:
		 request.head.ki = KI_CURLIGHT;
		 break;
	 }

	 writen(fd, (void *)&request, sizeof(struct common_packet));
	 readnd = readn(fd, (void *)buffer, sizeof(struct common_packet));
	 hdr = (ctl_hdr *)buffer;
	 //replay = (struct replay_packet *)buffer;

	 if (readnd > 0) {
	 	if (hdr->ki == KI_REPLAY) {
	 		switch (device) {
	 			 case 101:
	 				 //light
	 			 {
	 				 replay = (struct replay_packet *)buffer;
	 				 int16 data = replay->data;
	 				 int chushuturn = 512;
	 				 int turn[10] = {0,0,0,0,0,0,0,0,0,0};
	 				 int i = 0;
	 				 for (i = 0; i < 10; i ++) {
	 					 turn[9 - i] = data / chushuturn;
	 					 data = data % chushuturn;
	 					 chushuturn = chushuturn / 2;
	 				 }
	 				 fprintf(cgiOut, "turn0=%d;turn1=%d;turn2=%d;turn3=%d;turn4=%d;turn5=%d;turn6=%d;turn7=%d;turn8=%d;turn9=%d",
	 					 				turn[0], turn[1], turn[2], turn[3], turn[4], turn[5], turn[6], turn[7], turn[8], turn[9]);
	 			 }
	 				 break;
	 			 case 102:
	 			 {
	 				 replay = (struct replay_packet *)buffer;
	 				 int16 data = replay->data;
	 				 int chushucur = 27;
	 				 int curtains[4] = {0,0,0,0};
	 				 int i = 0;
	 				 for (i = 0; i < 4; i ++) {
	 					 curtains[3 - i] = data / chushucur;
	 					 data = data % chushucur;
	 					 chushucur = chushucur / 3;
	 				 }
	 				 fprintf(cgiOut, "curtain0=%d;curtain1=%d;curtain2=%d;curtain3=%d", curtains[0], curtains[1], curtains[2], curtains[3]);
	 			 }
	 				 break;
	 			 case 104:
	 			 {
	 				 replay_common = (struct common_packet *)buffer;
	 				char * temp = replay_common->data;
	 				if (number == 100) {
	 					struct alarm_all_data * alldata = (struct alarm_all_data *)temp;
	 					fprintf(cgiOut, "hour1=%d;minute1=%d;fangjian1=%d;set1=%d;hour2=%d;minute2=%d;fangjian2=%d;set2=%d;hour3=%d;minute3=%d;fangjian3=%d;set3=%d;hour4=%d;minute4=%d;fangjian4=%d;set4=%d;hour5=%d;minute5=%d;fangjian5=%d;set5=%d",
	 							alldata->data[0].hour, alldata->data[0].minute, alldata->data[0].fangjian, alldata->data[0].set,
	 							alldata->data[1].hour, alldata->data[1].minute, alldata->data[1].fangjian, alldata->data[1].set,
	 							alldata->data[2].hour, alldata->data[2].minute, alldata->data[2].fangjian, alldata->data[2].set,
	 							alldata->data[3].hour, alldata->data[3].minute, alldata->data[3].fangjian, alldata->data[3].set,
	 							alldata->data[4].hour, alldata->data[4].minute, alldata->data[4].fangjian, alldata->data[4].set);
	 				} else {
	 					struct alarm_data * alarmdata = (struct alarm_data *)temp;
	 					fprintf(cgiOut, "id=%d;hour=%d;minute=%d;fangjian=%d;yinyue=%d;set=%d;tishi=%s;sun=%d;mon=%d;tue=%d;wed=%d;thu=%d;fri=%d;sat=%d",
	 						 	alarmdata->id, alarmdata->hour, alarmdata->minute, alarmdata->fangjianid, alarmdata->yinyue, alarmdata->set, alarmdata->tishi,
	 						 	alarmdata->sun, alarmdata->mon, alarmdata->tue, alarmdata->wed, alarmdata->thu, alarmdata->fri, alarmdata->sat);
	 				}
	 			 }
	 			 	 break;
	 			 case 203:
	 			 {
	 				 replay = (struct replay_packet *)buffer;
	 				 int16 data = replay->data;
	 				 uint8 num1 = data / 10;
	 				 uint8 num2 = data % 10;
	 				 switch (number) {
	 				 case 1:
	 					 fprintf(cgiOut, "turn0=%d;curtain0=%d", num1, num2);
	 					 break;
	 				 case 2:
	 					 fprintf(cgiOut, "turn3=%d;curtain1=%d", num1, num2);
	 					 break;
	 				 case 4:
	 					 fprintf(cgiOut, "turn4=%d;curtain2=%d", num1, num2);
	 					 break;
	 				 case 8:
	 					 fprintf(cgiOut, "turn5=%d;curtain3=%d", num1, num2);
	 					 break;
	 				 case 16:
	 					 fprintf(cgiOut, "turn1=%d", num1);
	 					 break;
	 				 case 32:
	 					 fprintf(cgiOut, "turn6=%d;turn30=%d", num1, num2);
	 					 break;
	 				 }
	 			 }
	 			 	 break;
	 		}
	 	}
	 }
	 tcp_close(fd);
	 return 0;
}



























