#ifndef __TRANSMITTER_H__
#define __TRANSMITTER_H__
#include "typedef.h"
#include "log.h"

namespace comm
{
struct stat
{
	uint64 pkg_cnt;
	uint64 pkg_size;
	uint32 rate;
	uint32 open_time;	
	uint32 close_time;	
};
class Transmitter
{
public:
	Transmitter();
	~Transmitter();
	int  open(const string& dst_ip, ushort port);
	void close();
private:
	ushort _src_port;
	ushort _dst_port;
	string _src_ip;
	string _dst_ip;
	pchar  _rcv_buf;
	pchar  _snd_buf;
	int    _rcv_cur;
	int    _rcv_need;
	int    _snd_cur;
	int    _snd_need;
	cid    _id;
};
}

#endif
