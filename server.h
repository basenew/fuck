#ifndef __COMM_SERVER_H__
#define __COMM_SERVER_H__
#include "typedef.h"
#include "error.h"
#include "config.h"
#include "connect.h"

namespace comm
{
class Server
{
public:
	virtual ~Server();
	virtual void run();
	virtual void read(cid id, pcchar buf, int size);
	virtual int  write(cid id, pcchar buf, int size);
	virtual void client_connected(cid id);
	virtual void client_close(cid id);
	virtual void shut_down();
	//virtual Config get_config(){return _conf;};
	//virtual void set_config(const Config& conf){return _conf = conf;};
	//virtual NetConfig get_config(){return _conf.net_conf;};
	
	static Server& ins()
	{
		static Server svr;
		return svr;
	};
private:
	enum Status
	{
		STOPED,
		RUNNING,
		SUSPEND,
	};

	Server();
	Err create_listener();
	int create_selector();
	int _fd;
	int _selector;
	int _active_conn_cnt;
	int _conn_cnt;
	Config _conf;
	Status _status;
		
};

}
#endif
