#include "typedef.h"
#include "log.h"

namespace comm
{
	enum class SelectMode
	{
		NONE,
		EPOLL,
		SELECT,
		POLL,
	};

	enum class Proto
	{
		TCP,
		UDP, 
	};

	enum class Module
	{
		NONE    = 0,
		ECHO    = 1,
		P2P     = 2,
		P2G     = 4,
		GROUP   = 8,
		BROCAST = 16,
		STAT    = 32,
	};

	struct NetConfig
	{
		bool  block;	
		bool  delay;	
		bool  keep_alive;
		int   conn_cnt;
		int   preconn_cnt;
		int   rcv_buf_size;
		int   snd_buf_size;
		int   hbt_timeout;
		int   hbt_times;
		NetConfig():
		block(true),
		delay(true),
		keep_alive(true),
		conn_cnt(-1),
		preconn_cnt(-1),
		rcv_buf_size(-1),
		snd_buf_size(-1),
		hbt_timeout(30),
		hbt_times(30)
		{};
	};
	
	struct MemConfig
	{
		
	};

	struct ThreadConfig
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
		string       ip;
		Proto 	     proto;	
		bool         demon;
		int          proc_cnt;
		SelectMode   mode;
		uint32       modules;
		NetConfig    net_conf;
		MemConfig    mem_conf;
		ThreadConfig thd_conf;
		Config():
		port(16888),
		proto(Proto::TCP),
		demon(false),
		proc_cnt(1),
		//ip("127.0.0.1")
		ip("192.168.0.108")
		{};
	};

	
}
