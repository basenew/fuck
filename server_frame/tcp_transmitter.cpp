#include "tcp_transmitter.h"
#include "socketop.h"

namespace comm
{

	tcp_transmitter::tcp_transmitter()
	{

	}

	tcp_transmitter::~tcp_transmitter()
	{

	}

	int tcp_transmitter::open(int fd)
	{
		_fd = fd;
		on_read();
		return 0;
	}

	int tcp_transmitter::open(const string& remote_ip, ushort remote_port, int timeout_ms)
	{
		//todo
		return 0;
	}

	void tcp_transmitter::close()
	{

	}

	void tcp_transmitter::on_read()
	{

	}

	void tcp_transmitter::on_write()
	{

	}

	void tcp_transmitter::on_error()
	{

	}


}

