#include "../../comm/comm.h"
#include "../../server_frame/server_frame.h"
#include "../../server_frame/epoll_reactor.h"
#include "../../server_frame/tcp_acceptor.h"
#include "../../server_frame/tcp_transmitter.h"
#include "../../server_frame/socketop.h"
//todo #include "../../comm/vector_list.h"

#include <assert.h>
#include <list>

using namespace std;
using namespace comm;

class echo_server:public server_frame
{
public:
	echo_server():server_frame(_reactor, _acceptor){};
	~echo_server()
	{
		for (auto client:_clients)
		{
			if (client)
			  delete client;
		}
	};	


	int start(int argc, char** argv)
	{
		LOG_SYS << "echo_server start..." << endl;
		using std::placeholders::_1 ;
		using std::placeholders::_2 ;
		_acceptor.open("127.0.0.1", 9898, on_connected, this);
		return 0;
	};

	inline static int on_connected(int fd, sockaddr_in* addr, void* context)
	{
		assert(context != nullptr);
		echo_server* svr = (echo_server*)context;
		return svr->_on_connected(fd, addr);
	};


protected:
	void on_start(){};
	void on_stop(){};
	void on_suspend(){};
	void on_continue(){};
	void on_reload(){};

	inline int _on_connected(int fd, sockaddr_in* addr)
	{
		LOG_MSG << "connected:" << trans_straddr(*addr) << ":" << addr->sin_port << endl;
		tcp_transmitter* client = new tcp_transmitter;	
		client->open(fd);
		_reactor.add_handler(client, reactor::event::READ|reactor::event::WRITE);
		_clients.push_back(client);
		return _clients.size();
	};

private:
	tcp_acceptor _acceptor;
	epoll_reactor _reactor;
	
	list<io_handler*> _clients;
};

int main(int argc, char** argv)
{
	echo_server svr;
	svr.start(argc, argv);
	
	return 0;
}

