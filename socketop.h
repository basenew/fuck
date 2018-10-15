#ifndef __SOCKETOP_H__
#define __SOCKETOP_H__
#include "typedef.h"
#include "log.h"

namespace comm
{

	int open_tcp();
	int open_udp();
	int open_acceptor();		
	bool set_reuse(int fd);
	bool set_nonblock(int fd);
	bool set_rcv_buf_size(int fd, int size);
	bool set_snd_buf_size(int fd, int size);
	bool bind(int fd, const string& ip, ushort port);
	bool listen(int fd, int que_size);
}
