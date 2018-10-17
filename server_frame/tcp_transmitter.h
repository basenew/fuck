#pragma once
#include "../typedef.h"
#include "io_handler.h"

#include <iostream>

using namespace std;

namespace comm
{


class tcp_transmitter:public io_handler
{
public:
	tcp_transmitter();
	virtual ~tcp_transmitter();
	int open(int fd);
	int open(const string& remote_ip, ushort remote_port, int timeout_ms);
	void close();

	int id(){return _id;};
	int send(pcchar data, int size, int delay);
	virtual void handler_data(pcchar data, int size);

	virtual void on_connected(){};
	virtual void on_disconnect(int err){};
	virtual void on_close(){};
protected:
	virtual void on_read();
	virtual void on_write();
	virtual void on_error();

private:
	int _id;
};

}
