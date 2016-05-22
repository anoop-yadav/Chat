/*
 * server.h
 * 
 * Copyright 2013 Anoop Yadav <anzy@dream-machine>
 * 
 */

#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include "constants.h"

		void *get_in_addr(struct sockaddr *sa);
		
		// Launch Server
		int startServer();
		// Get the server's IP address
		int getIpAddress();
		int isBigEndian();
#endif	
			
