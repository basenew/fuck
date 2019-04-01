#include "tcp_acceptor.h"
#include "socketop.h"

namespace comm
{


int tcp_acceptor::open(const string& ip, ushort port, accept_cb cb, void* context)
{
	_fd = open_tcp_service(ip, port, false, false);	
	if (-1 == _fd)
		return -1;

	_cb = cb;
	_context = context;
	return 0;
}


void tcp_acceptor::close()
{
	if (-1 != _fd)
	{
		::close(_fd);
		_fd = -1;
	}
}


#define MAX_ACCEPT_ONCE 32
void tcp_acceptor::on_read()
{
	sockaddr_in addr{0};
	socklen_t len = sizeof(addr);
	int fd;
	for (int i = 0; i < MAX_ACCEPT_ONCE; i++)
	{
		fd = accept(_fd, (sockaddr*)&addr, &len);
		if (fd > 0)_cb(fd, &addr, _context);
		else
		{
			if (errno == EAGAIN || errno == ENFILE) break;
			else if (errno == EINTR || errno == ECONNABORTED) continue;
			else on_error();
		}
	} 
} 

void tcp_acceptor::on_write()
{

}


void tcp_acceptor::on_error()
{
	close();
}



}
