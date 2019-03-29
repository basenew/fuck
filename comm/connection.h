#pragma once
#include "comm.h"
#include "data_buf.h"
#include <stream>

using namespace std;

namespace comm
{
class Connection
{
public:
	Connection()_fd(-1), _local_port(0), _remote_port(0){};
	Connection(int fd)_fd(fd), _local_port(0), _remote_port(0){};
	Connection(const string& remote_ip, short remote_port)
	:_remote_ip(remote_ip)
	,_remote_port(remote_port)
	,_fd(-1)
	,_local_port(0)
	,_remote_port(0)
	{};
	~Connection(){close()};

	bool connect(const string& remote_ip=string(""), short remote_port = 0, bool blocked = true);
	int send(const char* buf, int len);
	int recv(char* buf, int len);
	void close()
	{
		if (_fd != -1) close(_fd);
		_fd = -1;
		_connected = false;
		_local_ip = _remote_ip = "";
		_local_port = _remote_port = 0;
	};	

	inline bool is_connected(){return _connected;};
	inline bool is_udp(){return false;};
	inline bool is_tcp(){return true;};

	inline int   fd(){return _fd;};
	inline const string& local_ip(){return _local_ip;};
	inline const string& remote_ip(){return _remote_ip;};
	inline short local_port(){return _local_port;};
	inline short remote_port(){return _remote_port;};

private:
	int     _fd;
	bool    _connected;
	string  _local_ip;
	string  _remote_ip;
	short   _local_port;
	short   _remote_port;
	DataBuf _buf;
};
}


