/*
 * server.cpp
 * 
 * Copyright 2013 Anoop Yadav <anzy@dream-machine>

 * 
 */


#include "server.h"
#include "parser.h"
// Get client information
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);	
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Detect the machines IP address
int getIpAddress()  {
	struct ifaddrs *ifAddrStruct;
    struct ifaddrs *ifa;
    void *tmpAddrPtr=NULL;
    //string ip = "0.0.0.0";

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa ->ifa_addr->sa_family==AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            //string interface = ifa->ifa_name;
            if(strcmp(ifa->ifa_name,"lo") != 0)  {
				//ipAddress = addressBuffer;
				//cout << "IP Address:" << addressBuffer << endl;
				//ip = addressBuffer;
				printf("%s", addressBuffer);
				freeifaddrs(ifAddrStruct);
				return 0;
				//return ip;
			} 
        } else if (ifa->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            //string interface = ifa->ifa_name;
            if(strcmp(ifa->ifa_name, "lo") != 0)  {
				//ipAddress = addressBuffer;
				//ip = addressBuffer;
				printf("%s", addressBuffer);
				freeifaddrs(ifAddrStruct);
				return 0;
				//return ip;
			}
        } 
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
    return 1;
    //return ip;
}



// Launch the server
int startServer()  {
	struct addrinfo hints;
	struct addrinfo *servinfo, *p; // will point to the results
	int status;
	int sockfd, new_fd; // File Descriptors
	int yes;
	struct sockaddr_storage their_addr; // Store the client's information
	
	char s[INET6_ADDRSTRLEN];
	printf("Welcome to Chat!\n");
	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE; // fill in my IP for me
	
	// Call to addrinfo, get the connection information
	if ((status = getaddrinfo(NULL, "0", &hints, &servinfo)) != 0) {
		printf("getaddrinfo error:%s",gai_strerror(status));
		exit(1);
	}
	
	//cout << status;
	
	// Bind to the port
	for(p = servinfo; p != NULL; p = p->ai_next) {	
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			printf("server: socket\n");
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			printf("setsockopt\n");
			exit(1);
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			printf("server: bind\n");
			continue;
		}
		break;
	}
	
	
	// Exit if bind fails
	if (p == NULL) {
		printf("server: failed to bind\n");
		return 2;
	}
	
	
	// free the linked-list
	freeaddrinfo(servinfo); 
	
	
	// Start listening
	if (listen(sockfd, 10) == -1) {
		printf("listen");
		exit(1);
	}
	printf("Waiting for connecction on ");
	getIpAddress();
	// Get Server Stats
	struct sockaddr_in localAddress;
	socklen_t *addressLength = malloc(sizeof(socklen_t));
	*addressLength = sizeof localAddress;
	getsockname(sockfd, (struct sockaddr*)&localAddress, addressLength);
	//printf("local address: %s\n", inet_ntoa( localAddress.sin_addr));
	printf(" port %d", (int) ntohs(localAddress.sin_port));
	fflush(stdout);
	free(addressLength);
	
	// Main communication loop
	socklen_t *sin_size = malloc(sizeof(socklen_t));
	*sin_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, sin_size);
	if (new_fd == -1) {
		printf("accept");
		return -1;
	}
	inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);
	//printf("\nserver: got connection from %s", s);
	printf("\nFound a friend. You recieve first.\n");
	fflush(stdout);
	while(1) { 
		
		int numbytes;
		char buffer[BUFFER_SIZE];
		if ((numbytes = recv(new_fd, buffer, BUFFER_SIZE, 0)) == -1) {
			printf("\nError recieving message!\n");
			exit(1);
		}	
		
		// Error checks
		uint8_t msb, lsb;
		msb = buffer[0];
		lsb = buffer[1];
		uint16_t version1 =  (msb << 8) | lsb;
		
		msb = buffer[2];
		lsb = buffer[3];
		uint16_t length1 =  (msb << 8) | lsb;
		
		char messageNew[141] = "";
		strncpy(messageNew, buffer + 4, length1);	
		if(version1 != 457)  {
			printf("\nMalformed packet.");
			//printf("%hu", version1);
			exit(1);
		}
		if(length1 != strlen(messageNew))  {
			printf("\nIMalformed packet.");
			exit(1);
		}
		printf("Friend: %s", messageNew);
		while(1) { // this is the child process
			//close(sockfd); // child doesn't need the listener
			printf("You:");
			char *msg=malloc(sizeof(char)*141);
			//fgets(msg, 141, stdin);
			int limit = 141;
			int bytes_read = getline(&msg, (size_t*)&limit, stdin);
			if(bytes_read > 140)  {
				printf("Error: Input too long.\n");
				continue;
			}
			char buffer1[145];
			int16_t version = 457;
			int16_t length = strlen((msg));
			if(isBigEndian())  {
				version = ntohs(version);
				length = ntohs(length);
			}
			buffer1[0] = (uint8_t) ((version >> 8) & 0xff);
			buffer1[1] = (uint8_t) (version & 0xff);
			buffer1[2] = (uint8_t) ((length >> 8) & 0xff);
			buffer1[3] = (uint8_t) (length & 0xff);
			strncpy(buffer1 + 4, (msg), (size_t)length);
			//printf("%s", buffer1+4);
			//printf("%d", strlen(buffer1));
			if (send(new_fd, buffer1, 4+strlen((msg)), 0) == -1)  { 
				printf("send");
				close(new_fd);
				exit(0);
			}
			break;
		}
	}
	close(new_fd);
	
	return 0;
}
