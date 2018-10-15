#pragma once

namespace comm
{

class reactor;

class io_handler
{
public:
	io_handler():fd(-1),_reactor(nullptr){};
	virtual ~io_handler(){if (fd != -1) close(fd);_reactor = nullptr;};

	virtual void on_read();
	virtual void on_write();
	virtual void on_error();
	int fd(){return _fd;};
private:
	int _fd;
	reactor* _reactor;	
};

}
