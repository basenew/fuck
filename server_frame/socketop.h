#pragma once
#include "../comm/comm.h"

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/types.h>

using namespace std;

namespace comm
{

	int open_tcp_client(const string& remote_ip, ushort remote_port, bool nonblock, int timeout_ms);
	int open_tcp_service(const string& local_ip, ushort local_port, bool nonblock, bool reuse);
	int open_udp_client();
	int open_udp_service(const string& remote_ip, ushort port);
	int set_reuse(int fd);
	int set_nonblock(int fd);
	int set_nodelay(int fd);
	int set_keepalive(int fd, int idle=30, int count=3, int interval=10);
	int set_rcv_buf_size(int fd, int size);
	int set_snd_buf_size(int fd, int size);
	int get_socket_error(int fd);
	sockaddr_in trans_sockaddr(const string& ip, ushort port); 
	void trans_straddr(const sockaddr_in& addr, string& ip); 
	string trans_straddr(const sockaddr_in& addr);
	int check_sock_event(int fd, bool read_event, bool write_event, int timeout_ms);
}
