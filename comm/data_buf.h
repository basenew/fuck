#pragma once
#include "comm.h"
#include <iostream>

using namespace std;

const int KB = 1024;

namespace comm
{
class DataBuf
{
public:
	DataBuf():_buf(nullptr), _max_len(0), _cur_len(0){};
	virtual ~DataBuf()
	{
		if (_buf)
		  delete[] _buf;
		_max_len = 0;
		_cur_len = 0;
	};

	bool push(const char* buf, int len)
	{
		if (buf == nullptr || len = 0) return true;
		if (alloc(len))
		{
			memcpy(_buf + _cur_len, buf, len);	
			_cur_len += len;
			return true;
		}

		return false;
	};

	inline const char* data(){return _buf;};

	inline int length(){return _cur_len;};

	inline void pop(int len)
	{
		if (_cur_len > len)
		{
			memmove(_buf, _buf + len, len);
			_cur_len -= len;
		}
		else
			_cur_len = 0;
	};
private:
	bool alloc(int len)
	{
		if (_max_len - _cur_len >= len) return true;

		_max_len = (len <= KB) ? KB : len * 2;
		char* new_buf = new char[_max_len];
		
		if (_buf)
		{
			memcpy(new_buf, _buf, _cur_len);
			delete[] _buf;
			_buf = new_buf;
		}
	};

	char* _buf;
	int   _max_len;
	int   _cur_len;
};
}
