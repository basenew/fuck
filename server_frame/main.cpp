#include "../comm/comm.h"
#include "epoll_reactor.h"
#include "tcp_acceptor.h"
#include "server_frame.h"

#define TEST_MAIN main

using namespace comm;

class server:public server_frame
{
public:
	server():server_frame(_reactor, _acceptor){};
	~server(){};
	void send_sig(int sig)
	{
		LOG_DBG << "send_sig" << endl;
	};

	void on_start()
	{
		LOG_DBG << "on_start" << endl;
	};

	void on_stop()
	{
		LOG_DBG << "on_stop" << endl;
	};

	void on_suspend()
	{
		LOG_DBG << "on_suspend" << endl;
	};

	void on_continue()
	{
		LOG_DBG << "on_continue" << endl;
	};

	void on_reload()
	{
		LOG_DBG << "on_reload" << endl;
	};

private:
	tcp_acceptor _acceptor;
	epoll_reactor _reactor;
};

int main(int argc, char** argv)
{
	LOG_DBG<< "test server frame" << endl;
	server svr;

	return 0;
}
