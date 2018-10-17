#pragma once
#include "reactor.h"
#include "../vector_list.h"

#include <sys/epoll.h>

namespace comm
{

class epoll_reactor:public reactor
{
public:
	epoll_reactor():_ep_fd(-1), _evt_cnt(0), _evts(nullptr){};
	virtual ~epoll_reactor(){::close(_ep_fd);if (_evts) delete[] _evts;};

	int open(int io_handler_cnt);
protected:
	virtual int check(int ms);
	virtual int add_handler(io_handler* h, int evts);
	virtual int mod_handler(io_handler* h, int evts);
	virtual void del_handler(io_handler* h);

private:
	epoll_reactor(const epoll_reactor&);
	epoll_reactor& operator=(const epoll_reactor&);
	int ctl_handler(io_handler* h, int evts, int op);

private:
	int _ep_fd;
	int _evt_cnt;
	epoll_event* _evts;
};

}
