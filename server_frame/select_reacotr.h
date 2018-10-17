#pragma once
#include "reactor.h"


namespace comm
{

class epoll_reactor:public reactor
{
public:
	epoll_reactor();
	virtual ~epoll_reactor();

protected:
	virtual int check(int msec);
	virtual int add_handler(io_handler* h, int evts);
	virtual int mod_handler(io_handler* h, int evts);
	virtual void del_handler(io_handler* h);

private:
	epoll_reactor(const epoll_reactor&);
	epoll_reactor& operator=(const epoll_reactor&);
};

}
