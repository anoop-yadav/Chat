#include "client.h"
#include "parser.h"

int startClient() {
	int sockfd, numbytes, newfd;
	char buffer[BUFFER_SIZE];
	struct addrinfo hints, *serverInfo, *p;
	int rv;
	char ipAddress[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; //always the same. At least in this class
	hints.ai_socktype = SOCK_STREAM; //set tcp


	//get information about the host and port specified by the user
	if ((rv = getaddrinfo(getIP(), getPort(), &hints, &serverInfo)) != 0) {
		printf("getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for (p = serverInfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	//inet_ntop(p->ai_family, (struct sockaddr*)get_in_addr((struct sockaddr *) p->ai_addr),
	//		ipAddress, sizeof ipAddress);
	//printf("client: connecting to %s\n", ipAddress);
	printf("Connecting to server... Connected!\n");
	printf("Connected to a friend! You send first.\n");
	freeaddrinfo(serverInfo); // all done with this structure

	while(1) { 
		
		
		// Main communication loop
		while(1)  {
			// Send Message	
			while(1) { // this is the child process
				printf("You:");
				char *msg=malloc(sizeof(char)*141);
				//fgets(msg, 141, stdin);
				int limit = 141;
				int bytes_read = getline(&msg, (size_t*)&limit, stdin);
				if(bytes_read > 140)  {
					printf("Error: Input too long!");
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
				if (send(sockfd, buffer1, 4+strlen((msg)), 0) == -1)  { 
					printf("send");
					close(sockfd);
					exit(0);
				}
				break;
			}
			
			// Recieve Message
			int numbytes;
			char buffer[BUFFER_SIZE];
			if ((numbytes = recv(sockfd, buffer, BUFFER_SIZE, 0)) == -1) {
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
				printf("\nInvalid Packet version!");
				printf("%hu", version1);
				exit(1);
			}
			if(length1 != strlen(messageNew))  {
				printf("\nInvalid header!");
				exit(1);
			}
			printf("Friend: %s", messageNew);
		}
	}

	close(sockfd);
	//close(newfd);

	return 0;
}

