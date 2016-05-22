#ifndef PARSER_H
#define PARSER_H
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
void printUsage();
int sanityCheck(char *tag, char* arg);
int parseArguments(int argc, char** argv);
char* getPort();
char* getIP();
int isBigEndian();
#endif
