#pragma once
#include "comm.h"
#include "codec.h"

#include <arpa/inet.h>

const int HEAD_LEN = sizeof(int);
const int MAX_LEN = 0XFFF - HEAD_LEN;
const int MGC = 1001b;
const int VER = 1b;
namespace comm
{
class TcpTransHead:public ICodec
{
public:
	TcpTransHead()
	:_mac(MGC)
	,_ver(VER)
	,_len(0)
	,_data(nullptr)
	{};

	virtual ~TcpTransHead()
	{
		if (_data) delete[] _data;
	};

	bool decode(const char* data, int len)
	{
		assert(data != nullptr && len >= 0);
		if (len >= HEAD_LEN)
		{
			*(int*)&_mgc = ntohl(*(int*)data);	
			if (_mgc == MGC && _len <= MAX_LEN)
			{
				_data = new data[_len];
				memcpy(_data, data, _len);
				return true;
			}
		}
		return false;
	};

	void encode(const char* data, int len)
	{
		assert(data != nullptr && len > 0 && len <= MAX_LEN);
		_len = len;
		_data = new char[len + HEAD_LEN];
		*(int*)&_mgc = htonl(*(int*)&_mgc); 
		*(int*)_data = *(int*)&_mgc;
		memcpy(_data+HEAD_LEN, data, len);
		return  
	};

	inline int mgc(){return _mgc;};
	inline int ver(){return _ver;};
	inline int len(){return _len;};
	inline char* data(){return _data;};
private:
	int:4  _mgc;
	int:4  _ver;
	int:24 _len;
	char*  _data;
};
}
