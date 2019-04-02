#pragma once

#include "../comm/comm.h"
#include "reactor.h"
#include "io_handler.h"

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
		STATUS_CONTINUE,
		STATUS_PRESTOP,
		STATUS_RELOAD, 
	};

	server_frame(reactor& evt_egn, io_handler& main_io):_evt_egn(evt_egn), _main_io(main_io), _status(STATUS_STOPED){};
	virtual ~server_frame(){};
	virtual int start(int argc, char** argv);
	void stop();

	virtual void send_sig(int sig);
	virtual void on_start(){};
	virtual void on_stop(){};
	virtual void on_suspend(){};
	virtual void on_continue(){};
	virtual void on_reload(){};

private:
	server_frame(const server_frame&);
	server_frame& operator=(const server_frame&);

	void show_help();
	int parse_arg(int argc, char** argv);

private:
	reactor& _evt_egn;
	io_handler& _main_io;
	int _status;
};

}
