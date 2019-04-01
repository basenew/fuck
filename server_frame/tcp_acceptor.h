#pragma once
#include "io_handler.h"

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <sys/types.h>
#include <tr1/functional>

using namespace std;

namespace comm
{

//typedef std::tr1::function<int(int,sockaddr_in*,void*)> accept_cb;
typedef int (*accept_cb)(int,sockaddr_in*,void*);
class tcp_acceptor:public io_handler
{
public:
	tcp_acceptor(){};
	virtual ~tcp_acceptor(){};
	int open(const string& ip, ushort port, accept_cb cb, void* context);
	void close();

protected:
	virtual void on_read();
	virtual void on_write();
	virtual void on_error();

private:
	accept_cb _cb;	
	void* _context;
	int _fd;
};
}
