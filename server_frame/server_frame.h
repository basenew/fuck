#pragma once

#include "../comm/comm.h"
#include "reactor.h"
#include "io_handler.h"
#include <thread>

using namespace std;

namespace comm
{


class server_frame
{
public:
	enum 
	{
		STATUS_STOPED,
		STATUS_RUNNING,
		STATUS_SUSPEND,
		STATUS_RELOAD, 
	};

	server_frame(reactor& evt_egn, io_handler& main_io)
	:_evt_egn(evt_egn)
	,_main_io(main_io)
	,_status(STATUS_STOPED)
	{};

	virtual ~server_frame(){};
	virtual int start(int argc, char** argv);
	virtual int stop();
	virtual int pause();
	virtual int resume();
	virtual int reload();

	virtual int send_sig(int sig);
	virtual int on_start(){return OK;};
	virtual int on_stop(){return OK;};
	virtual int on_suspend(){return OK;};
	virtual int on_continue(){return OK;};
	virtual int on_reload(){return OK;};

private:
	server_frame(const server_frame&){};
	server_frame& operator=(const server_frame&){};
	static void thread_static_proc(server_frame* self);
	{
		if (self)
			self->thread_proc();
	};

	int thread_proc();

	void show_help();
	int parse_arg(int argc, char** argv);

private:
	reactor&    _evt_egn;
	io_handler& _main_io;
	thread      _thd;
	int         _status;
};

}
