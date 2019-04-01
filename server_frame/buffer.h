#pragma once
#include "../comm/comm.h"

#include <string.h>

namespace comm
{

class buffer
{
public:
	buffer(int size):
	_size(size),
	_in(0),
	_out(0),
	_buf(new char[size])
	{
	};
	~buffer()
	{
		if (_buf) delete[] _buf;
	};

	inline int size()const {return _size;};
	inline int space_size()const {return _size - _in;};
	inline int data_size()const {return _in - _out;};
	inline bool empty()const {return _in >= _size;};
	inline char* space(){return _buf + _in;};
	inline char* data(){return _buf + _out;};
	inline void push(int len){_in += len;};
	inline void pop(int len){_out += len;};

	inline void adjust()
	{
		if (_buf && _size)
		{
			memmove(_buf, _buf + _out, _in - _out);
			_in -= _out;
			_out = 0;
		}	
	};

	inline void resize(int size)
	{
		if (_buf)
		{
			char* buf = new char[size];
			memcpy(buf, _buf, _size);
			delete[] _buf;
			_size = size;
			_buf = buf;
		}
	};

private:
	int _size;
	int _in;
	int _out;
	char* _buf;
};

}
