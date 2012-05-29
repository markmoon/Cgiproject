OBJS = login.o register.o turns.o getstatus.o curtains.o television.o conditioner.o net.o cgic.o getip.o \
 alarms.o

CC = arm-linux-gcc

CFLAGS = -Wall -O -g

all : login.cgi register.cgi turns.cgi getstatus.cgi curtains.cgi conditioner.cgi television.cgi alarms.cgi

login.cgi : login.o net.o cgic.o getip.o
	$(CC) $^ -o $@ 

register.cgi : register.o net.o cgic.o getip.o
	$(CC) $^ -o $@ 

turns.cgi : turns.o net.o cgic.o getip.o
	$(CC) $^ -o $@ 

getstatus.cgi : getstatus.o net.o cgic.o getip.o
	$(CC) $^ -o $@ 

curtains.cgi : curtains.o net.o cgic.o getip.o
	$(CC) $^ -o $@ 

conditioner.cgi : conditioner.o net.o cgic.o getip.o
	$(CC) $^ -o $@ 

television.cgi : television.o net.o cgic.o getip.o
	$(CC) $^ -o $@ 

alarms.cgi : alarms.o net.o cgic.o getip.o
	$(CC) $^ -o $@ 

login.o : login.c types.h define.h

register.o : register.c types.h define.h

turns.o : turns.c types.h define.h

net.o : net.c net.h types.h define.h 

cgic.o : cgic.c

getip.o : getip.c getip.h

clean :
	rm *.cgi $(OBJS) 
