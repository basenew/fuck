#include "socketop.h"

namespace comm
{
	int open_tcp_client(const string& remote_ip, ushort remote_port, int timeout_sec)
	{
		return 0;
	}


	int open_tcp_service(const string& local_ip, ushort local_port)
	{
		return 0;
	}


	int open_udp_client()
	{
		return 0;
	}


	int open_udp_service(const string& remote_ip, ushort port)
	{
		return 0;
	}


	int set_reuse(int fd)
	{
		return 0;
	}


	int set_nonblock(int fd)
	{
		return 0;
	}


	int set_keepalive(int fd, int idle=30, int count=3, int interval=10)
	{
		return 0;
	}


	int set_rcv_buf_size(int fd, int size)
	{
		return 0;
	}


	int set_snd_buf_size(int fd, int size)
	{
		return 0;
	}


}
