#include "parser.h"
#include "server.h"
#include "client.h"
//using namespace std;
int main(int argc, char** argv)  {
	//Parser parser;
	//Server server;
	//int result = parser.parse(argc, argv);
	//initialize();
	int result = parseArguments(argc, argv);
	char* info;
	info = malloc(30*sizeof(char));
	switch(result) {
		case SERVER_FLAG:
			//cout << "Start Server." << endl;
			startServer();
			break;
		case CLIENT_FLAG:
			//cout << "Start Client." << endl;
			//printf("Start client!\n");
			//sprintf(info, "IP%s Port%s\n", getIP(),getPort());
			//printf("%s, %s", getIP(), getPort()); 
			startClient();
			break;
		case INVALID_FLAG:
			//cout << "Error!" << endl;
			break;
	}
	return 0;
}
