#include "connection.h"
#include <sys/socket.h> 

namespace comm
{

bool Connection::connect(const string& remote_ip, short remote_port, bool nonblocked)
{
	if (remote_ip.empty())
	{
		LOG_ERR << "remote ip is empty" << endl;
		return false;
	}

	if (_connected)
	{
		if (remote_ip == _remote_ip && remote_port == _remote_port)
		{
			LOG_DBG << "already connected" << endl;
			return true;
		}
		else
		{
			close();
			_remote_ip = remote_ip;
			_remote_port = remote_port;
		}
	}

	if (_fd == -1)
	{
		_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (_fd == -1)
		{
			LOG_ERR << get_errno_str() << endl;
			return false;
		}
		if (nonblocked)
		{

		}
	}


	return true;
}

int Connection::send(const char* buf, int len)
{
	return len;
}

int Connection::recv(char* buf, int len)
{
	return len;
}
}
