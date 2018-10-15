#include "typedef.h"
//#include "log.h"
#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

namespace comm
{
Server::Server():
_status(STOPED),
_fd(-1),
_active_conn_cnt(0),
_conn_cnt(0),
_selector(-1)
{
	LOG_SYS << "server created" <<endl;
}

Server::~Server()
{
	close(_fd);
	LOG_SYS << "server exit" << endl;
}

Err Server::create_listener()
{
	_fd = socket(AF_INET,SOCK_STREAM, 0);
	if (_fd == -1)
	{
		LOG_SYS << "create socket for listen err:" << get_errno_str() << endl;
		return ERR;
	}
	
	sockaddr_in addr;
        bzero(&addr,sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(_conf.ip.c_str());
        addr.sin_port = htons(_conf.port);
        int reuse = 1;
        setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
	if (0 != bind(_fd, (sockaddr*)&addr, sizeof(addr)))
	{
		close(_fd);
		_fd = -1;
		LOG_SYS << "bind socket err:" << get_errno_str() << endl;
		return ERR;
	}

	return OK;
}

void Server::run()
{
	LOG_SYS << "server start to run" << endl;
	if (OK != create_listener())
		return;

	if (0 != listen(_fd, _conf.net_conf.preconn_cnt))
	{
		LOG_SYS << "listen on " << _conf.ip << ":"<< _conf.port << " err:" << get_errno_str() << endl;
		close(_fd);
		_fd = -1;
		return;
	}

	_status = RUNNING;
	while(_status != STOPED)
	{
		sockaddr_in addr;
		int len = sizeof(addr);
		int fd = accept(_fd, (sockaddr*)&addr, (socklen_t *)&len);	
		if (fd == -1)
		{
			LOG_ERR << "accept err:" << get_errno_str() << endl;
		}
		else
		{
			_active_conn_cnt++;
			_conn_cnt++;
			LOG_MSG << "accept " << inet_ntoa(addr.sin_addr) << ":" << addr.sin_port << endl;	
			LOG_MSG << "active connects:" << _active_conn_cnt << " total connects:" << _conn_cnt << endl;	
		}
	}	
	LOG_SYS << "server running" << endl;

}

void Server::read(cid id, pcchar buf, int size)
{

}

int Server::write(cid id, pcchar buf, int size)
{

}

void Server::client_connected(cid id)
{

}

void Server::client_close(cid id)
{

}

void Server::shut_down()
{

}


}
