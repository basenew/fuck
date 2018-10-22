#pragma once
#include "../comm_include.h"

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
	_buf(nullptr)
	{
		_buf = new char[_size];
	};
	~buffer();

	int size()const {return _size;};
	int space_size()const {return _size - _in;};
	int data_size()const {return _in - _out;};
	bool empty()const {return _in >= _size;};
	char* space(){return _buf + _in;};
	char* data(){return _buf + _out;};
	void push(int len){_in += len;};
	void pop(int len){_out += len;};
	void adjust()
	{
		if (_buf && _size)
		{
			memmove(_buf, _buf + _out, _in - _out);
			_in -= _out;
			_out = 0;
		}	
	};
	void resize(int size)
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
