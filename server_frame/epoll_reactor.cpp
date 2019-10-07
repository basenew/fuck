#include "epoll_reactor.h"

namespace comm
{
	int epoll_reactor::open(int io_handler_cnt)
	{
		if (io_handler_cnt < 1)
		{
			LOG_ERR << "io handler count must >= 1" << endl;
			return -1;
		}
		_ep_fd = epoll_create(io_handler_cnt);	
		if (_ep_fd == -1)
		{
			LOG_ERR << get_errno_str() << endl;
			return -1;
		}
		
		_evts = new epoll_event[io_handler_cnt];
		_evt_cnt = io_handler_cnt;
		return 0;
	}

	int epoll_reactor::check(int ms)
	{
		int n = epoll_wait(_ep_fd, _evts, _evt_cnt, ms);
		for (int i = 0; i < n; i++)
		{
			epoll_event& ev = _evts[i];
			if (0 == (ev.events & (EPOLLPRI|EPOLLERR|EPOLLHUP)))
			{
				if (ev.events & EPOLLIN)	
				{
					((io_handler*)ev.data.ptr)->on_read();
				}
				if (ev.events & EPOLLOUT)	
				{
					((io_handler*)ev.data.ptr)->on_write();
				}
			}
			else
			{
				((io_handler*)ev.data.ptr)->on_error();
			}
		}

		return n;
	}

	int epoll_reactor::ctl_handler(io_handler* h, int evts, int op)
	{
		if (_ep_fd > 0)
		{
			epoll_event *pev = nullptr;
			epoll_event ev{0};
			if (op == EPOLL_CTL_DEL)
			{
				ev.events = evts;
				ev.data.fd = h->fd();
				ev.data.ptr = h;
				pev = &ev;
			}
			return epoll_ctl(_ep_fd, op, ev.data.fd, pev);
		}
		
		return -1;
	}

	int epoll_reactor::add_handler(io_handler* h, int evts)
	{
		return ctl_handler(h, evts, EPOLL_CTL_ADD);
	}

	int epoll_reactor::mod_handler(io_handler* h, int evts)
	{
		return ctl_handler(h, evts, EPOLL_CTL_MOD);
	}

	void epoll_reactor::del_handler(io_handler* h)
	{
		ctl_handler(h, 0, EPOLL_CTL_DEL);
	}

}
