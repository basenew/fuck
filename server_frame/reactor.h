#pragma once
#include "io_handler.h"

namespace comm
{

class reactor
{
public:
	enum event{
        	READ  = 0x001,
        	WRITE = 0x004,
	};
public:
	reactor();
	virtual ~reactor();
	virtual int open(int io_hdl_cnt);
	virtual void close();
	virtual int check(int msec);
	virtual int add_handler(io_handler* h, int evts);
	virtual int mod_handler(io_handler* h, int evts);
	virtual void del_handler(io_handler* h);
private:
	reactor(const reactor&);
	reactor& operator=(const reactor&);
};

}
