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
	virtual ~reactor(){};
	virtual int open(int io_hdl_cnt) = 0;
	virtual void close() = 0;
	virtual int check(int msec) = 0;
	virtual int add_handler(io_handler* h, int evts) = 0;
	virtual int mod_handler(io_handler* h, int evts) = 0;
	virtual void del_handler(io_handler* h) = 0;
private:
	//todo reactor(const reactor&);
	//todo reactor& operator=(const reactor&);
};

}
