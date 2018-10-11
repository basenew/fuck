#include "typedef.h"
#include "log.h"
#include <sys/types.h>
#include <sys/socket.h>


Server::Server(uint16 port = 9898, int thread_count = 2, bool block = false)
{

}

Server::~Server()
{

}

void Server::run()
{

}

void Server::read(clientid id, pcchar buf, int size)
{

}

int  Server::write(clientid id, pcchar buf, int size)
{

}

void Server::client_connected(client id)
{

}

void Server::client_close(client id)
{

}

void Server::shut_down()
{

}


int main(int argc, char* argv[])
{
	return 0
}
