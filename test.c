/*
 * test.c
 *
 *  Created on: 2012-5-4
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
	 cgiHeaderContentType("text/html");
	 fprintf(cgiOut, "statuss=%d;passed=%d", 1, 111);
	 return 0;
}




























