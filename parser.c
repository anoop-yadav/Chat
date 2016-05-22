/*
 * parser.cpp
 * 
 * Copyright 2013 Anoop Yadav <anzy@dream-machine>

 * 
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
char *port;
char *ip;

void printUsage()  {
	printf("--Chat v1.0 Usage--\n");
	printf( "* Use ./chat with no arguments to invoke in server mode.\n");
	printf( "* To invoke in client mode, the program expects two arguments\n");
	printf( "* ./chat -p <port_number> -s <ip_address>\n");
	printf( "* The arguments may be supplied in any order.\n");
	printf( "* ./chat -h to display this message.\n");
}


// Utility Method
// Performs sanity check for command-line arguments
int sanityCheck(char *tag, char* arg)  {
	
	// Check for port number
	if(strcmp(tag, "-p") == 0)  {
		int i;
		for(i = 0; i < strlen(arg); i++)  {
			if(!(arg[i] >= '0' && arg[i] <= '9'))  {
				printf("Invalid port. Use chat -h for more help.\n");
				return INVALID_FLAG;;
			}
		}
		
		// String contains only numbers, now check bounds
		int temp = atoi(arg);
		if(temp > 0 && temp < 65536)  {
			// valid port number, assign to class member "port"
			port = arg;
			//sprintf(port, "%s", arg);
			//printf("Port: %d", temp);
		}
		else  {
			printf("Invalid port. Use chat -h for more help.\n");
			return INVALID_FLAG;
		}
	}
	
	// Check for IP address format
	if(strcmp(tag, "-s") == 0)  {
		// First check if it contains valid characters
		int i;
		for(i = 0; i < strlen(arg); i++)  {
			if(!((arg[i] >= '0') && (arg[i] <= '9')) || (arg[i] == '.'))  {
				printf("Invalid IP address. Use chat -h for more help.\n");
				return INVALID_FLAG;
			}
		}
		// Valid characters, now checck ranges
		char * pch;
		char *original;
		sprintf(original, "%s", arg);
		//printf("Original:%s", original);
		//printf ("Splitting string \"%s\" into tokens:\n",str);
		pch = strtok (arg,".");
		while (pch != NULL)
		{
			//printf ("%s\n",pch);
			if(atoi(pch) > 255)  {
				printf("Invalid IP address. Use chat -h for more help.\n");
				return INVALID_FLAG;
			}
			pch = strtok (NULL, ".");
		}
		ip = original;
		//sprintf(ip, "%s", original);
	}
	return 0;
		
	
}

// Utility Method
// Sets ipAddress and port via sanityCheck() if arguments supplied are good
int parseArguments(int argc, char** argv)  {
	// Check for no arguments, need to start a server
	if(argc == 1)  {
		return 1;
	}
	// Check for help flag
	else if(argc == 2)  {
		if(strcmp(argv[1], "-h") == 0)  {
			printUsage();
		}
        return INVALID_FLAG;
	}
	
	// Parse command-line arguments, must be 4
	else if(argc == 5)  {
		int i;
		for(i = 1; i < argc; i+=2)  {
			// Pass a pair of switch and argument to sanityCheck()
			
			if(sanityCheck(argv[i], argv[i + 1]) == -1)  {
				return INVALID_FLAG;
			}
		}
		return CLIENT_FLAG;
	}
	else  {
		printf("Invalid argument(s)\n");
		return INVALID_FLAG;
	}
}

char* getPort()  {
	return port;
}

char* getIP()  {
	return ip;
}

int isBigEndian()  {
	int num = 1;
	if(*(char *)&num == 1)
	{
		//printf("\nLittle-Endian\n");
		return 0;
	
	}
	else
	{
		//printf("Big-Endian\n");
		return 1;
	}
}



