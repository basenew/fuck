#pragma once
#include "comm.h"
#include <iostream>
#include <assert.h>
#include <string.h>

using namespace std;


namespace comm
{
class DataBuf
{
public:
	DataBuf():_buf(new char[KB]), _max_size(KB), _cur_len(0){};
	virtual ~DataBuf()
	{
		if (_buf)
		  delete[] _buf;
	};

	int push(int len)
	{
		assert(len + _cur_len <= _max_size);	
		return _cur_len += len;
	};

	inline char* buf(){return _buf + _cur_len;};
	inline int	 buf_size(){return _max_size - _cur_len;};
	inline char* data(){return _buf;};
	inline int   data_len(){return _cur_len;};

	inline void pop(int len = 0)
	{
		assert(len <= _cur_len);
		if (len == 0 || len == _cur_len)
		{
			_cur_len = 0;
		}
		else
		{
			memmove(_buf, _buf + len, len);
			_cur_len -= len;
		}
	};

	char* realloc_buf(int len)
	{
		assert(len <= GB);
		_max_size = len;
		char* new_buf = new char[_max_size];
		
		if (_buf && _cur_len > 0)
		{
			memcpy(new_buf, _buf, _cur_len);
			delete[] _buf;
			_buf = new_buf;
		}
	};

private:
	char* _buf;
	int   _max_size;
	int   _cur_len;
};
}
