#ifndef __SERVER_H__
#define __SERVER_H__
#include "typedef.h"
#include "log.h"
#include "connect.h"

namespace comm
{
class Server
{
public:
	enum SelectMode
	{
		NONE,
		EPOLL,
		SELECT,
		POLL,
	};

	enum Proto
	{
		TCP,
		UDP, 
	};

	enum Module
	{
		NONE = 0,
		ECHO = 1,
		P2P=2,
		P2G = 4,
		GROUP = 8,
		BROCAST = 16,
		STAT = 32,
	};

	struct NetConfig
	{
		Proto proto;	
		bool  block;	
		bool  delay;	
		bool  keep_alive;
		bool  echo;
		int   max_conn;
		int   max_preconn_que;
		int   max_rcv_buf_size;
		int   max_snd_buf_size;
		int   heartbeat_timeout;
	};
	
	struct MemConfig
	{
		
	};

	struct LogConfig
	{
		string path;
		string name;
	};

	struct Config
	{
		ushort       port;
		bool         demon;
		int          proc_cnt;
		SelectMode   mode;
		uint32       modules,
		NetConfig    net_conf,
		MemConfig    mem_conf,
		ThreadConfig thd_conf;
	};

	enum Err
	{
		ERR_INNER = -1,
	};
	
	using clientid int;
public:
	virtual ~Server();
	virtual void run();
	virtual void read(clientid id, pcchar buf, int size);
	virtual int  write(clientid id, pcchar buf, int size);
	virtual void client_connected(client id);
	virtual void client_close(client id);
	virtual void shut_down();
	
	static Server& ins()
	{
		static Server svr;
		return svr;
	};
private:
	Config _conf;	
	Server();
};

}
#endif
