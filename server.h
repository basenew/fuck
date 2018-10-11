#ifndef __SERVER_H__
#define __SERVER_H__
#include "typedef.h"
#include "log.h"
#include "connect.h"

class Server
{
public:
	enum Err
	{
		ERR_INNER = -1,
	};
	
	using clientid int;
public:
	Server(uint16 port = 9898, int thread_count = 2, bool block = false);
	virtual ~Server();
	virtual void run();
	virtual void read(clientid id, pcchar buf, int size);
	virtual int  write(clientid id, pcchar buf, int size);
	virtual void client_connected(client id);
	virtual void client_close(client id);
	virtual void shut_down();
private:
	short _port;
	
};


#endif
