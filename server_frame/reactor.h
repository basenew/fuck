#pragma once
#include "io_handler.h"


namespace comm
{

class reactor
{
public:
	reactor();
	virtual ~reactor();
	virtual int check(int msec);
	virtual int add_handler(io_handler* h, int evts);
	virtual int mod_handler(io_handler* h, int evts);
	virtual void del_handler(io_handler* h);

private:
	reactor(const reactor&);
	reactor& operator=(const reactor&);
};

}
