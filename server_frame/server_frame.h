#pragma once

#include "../comm_include.h"
#include "reactor.h"

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
		STATUS_PRESTOP,
		STATUS_RELOAD, 
	};

	server_frame();
	~server_frame();
	int start(int argc, char** argv);
	void stop();

	virtual void send_sig(int sig);
protected:
	virtual void on_start();
	virtual void on_stop();
	virtual void on_suspend();
	virtual void on_continue();
	virtual void on_reload();

private:
	server_frame(const server_frame&);
	server_frame& operator=(const server_frame&);

private:
	reactor* _reactor;
	int _status;
	int _max_io_h;	
};

}
