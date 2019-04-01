#include "socketop.h"
#include <poll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace comm
{
	sockaddr_in trans_sockaddr(const string& ip, ushort port)
	{
		sockaddr_in addr{0};
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(ip.c_str());
		addr.sin_port = port;
		return addr;
	}

	void trans_straddr(const sockaddr_in& addr, string& ip)
	{
		char* ip_tmp = inet_ntoa(addr.sin_addr);
		ip = ip_tmp;
	}

	string trans_straddr(const sockaddr_in& addr)
	{
		char* ip_tmp = inet_ntoa(addr.sin_addr);
		return string(ip_tmp);	
	}

	int check_sock_event(int fd, bool read_event, bool write_event, int timeout_ms)
	{
		pollfd plfd;
		plfd.fd = fd;
		plfd.events = 0;
		plfd.events = read_event ? POLLIN:0;
		plfd.events |= write_event ? POLLOUT:0;

		if (poll(&plfd, 1, timeout_ms) < 1)
		{
			errno = ETIMEDOUT;
			LOG_ERR << get_errno_str() << endl;	
			return -1;
		}

		return 0;
	}

	int open_tcp_client(const string& remote_ip, ushort remote_port, bool nonblock, int timeout_ms)
	{
		int fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd == -1)
		{
			LOG_ERR << get_errno_str() << endl;	
			return -1;
		}

		sockaddr_in addr = trans_sockaddr(remote_ip, remote_port);
		if (nonblock && 0 != set_nonblock(fd))
		{
			close(fd);
			LOG_ERR << get_errno_str() << endl;	
			return -1;
		}

		if (0 != connect(fd, (sockaddr*)&addr, sizeof(addr)) && errno != EINPROGRESS)
		{
			close(fd);
			LOG_ERR << get_errno_str() << endl;	
			return -1;
		}

		if (nonblock)
		{
			if (0 != check_sock_event(fd, true, true, timeout_ms))
			{
				close(fd);
				return -1;
			}
		}
	
		return fd;
	}


	int open_tcp_service(const string& local_ip, ushort local_port, bool nonblock, bool reuse)
	{
		int fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd == -1)
		{
			LOG_ERR << get_errno_str() << endl;	
			return -1;
		}
		
		sockaddr_in addr = trans_sockaddr(local_ip, local_port);
		if (nonblock) set_nonblock(fd);
		if (reuse) set_reuse(fd);
		if (0 != bind(fd, (sockaddr*)&addr, sizeof(addr)))
		{
			close(fd);
			LOG_ERR << get_errno_str() << endl;	
			return -1;
		}
	
		return fd;
	}


	int open_udp_client()
	{
		return 0;
	}


	int open_udp_service(const string& remote_ip, ushort port)
	{
		return 0;
	}

	int get_socket_error(int fd)
	{
	    int error_code = 0 ;
	    socklen_t len = (socklen_t)sizeof(error_code);
	    getsockopt(fd, SOL_SOCKET, SO_ERROR, &error_code, &len);
	
	    return error_code ;
	}

	int set_reuse(int fd)
	{
		int reuse = 1;
		
		if (0 != setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)))
		{
			LOG_ERR << get_errno_str() << endl;	
			return -1;
		}
		if (0 != setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)))
		{
			LOG_ERR << get_errno_str() << endl;	
			return -1;
		}
		return 0;
	}


	int set_nonblock(int fd)
	{
		int flag = fcntl(fd,F_GETFL) ;
    		if ( flag == -1)
    		{
        		return -1 ;
		}

		return fcntl(fd,F_SETFL,flag | O_NONBLOCK) ;
	}

	int set_nodelay(int fd)
	{
		int nodelay = 1;
	
		//todo if (0 != setsockopt(fd, IPPROTO_TCP, TCP_NONDELAY, &nodelay, sizeof(nodelay)))
		{
			LOG_ERR << get_errno_str() << endl;	
			return -1;
		}
		return 0;
	}

	int set_keepalive(int fd, int idle, int count, int interval)
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
