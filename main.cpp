#include "server.h"

using namespace comm;


int main(int argc, char* argv[])
{
	Server& server = Server::ins();
	server.run();	
	return 0;
}
