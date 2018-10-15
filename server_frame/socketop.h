#ifndef __SOCKETOP_H__
#define __SOCKETOP_H__
#include "typedef.h"
#include "log.h"

namespace comm
{

	int open_tcp_client(const string& remote_ip, ushort remote_port, int timeout_sec);
	int open_tcp_service(const string& local_ip, ushort local_port);
	int open_udp_client();
	int open_udp_service(const string& remote_ip, ushort port);
	int set_reuse(int fd);
	int set_nonblock(int fd);
	int set_keepalive(int fd, int idle=30, int count=3, int interval=10);
	int set_rcv_buf_size(int fd, int size);
	int set_snd_buf_size(int fd, int size);
}
