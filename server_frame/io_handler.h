#pragma once
#include "../comm/comm.h"
#include <unistd.h>

namespace comm
{

class reactor;

class io_handler
{
public:
	io_handler():_fd(-1),_reactor(nullptr){};
	virtual ~io_handler(){if (_fd != -1) close(_fd);_reactor = nullptr;};

	virtual int open(int io_handler_cnt){return ERR;};
	virtual void on_read(){};
	virtual void on_write(){};
	virtual void on_error(){};
	int fd(){return _fd;};
protected:
	int _fd;
	reactor* _reactor;	
};

}
