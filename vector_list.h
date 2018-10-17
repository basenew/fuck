#pragma once
#include "comm_include.h"

#include <vector>
#include <list>

using namespace std;

namespace comm
{

template<class T>
class vector_list
{
public:
	struct elem
	{
		int idx;
		T t;
		elem(int index = -1):idx(index){};
	};
public:
	vector_list():_cap(0){};

	void init(int size)
	{
		for (int i = 0; i < size; i++)
		{
			elem e(i);
			_v.push_back(e);
			_l.push_back(i);
		}
		_cap = size;
	};
	~vector_list(){};

	int used_size(){return _cap - _l.size();};
	int free_size(){return _l.size();};
	int capacity(){return _cap;};
	void resize(int size)
	{
		//todo size < _cap
		if (size > _cap)
		{
			_v.resize(size);
			for (int i = _cap; i < size; i++)
			{
				_v[i].idx = i;
				_l.push_back(i);
			}
			_cap = size;
		}
	};
	
	T& pop()
	{
		if (!_l.empty())
		{
			T& t = _v[_l.front()].t;
			_l.pop_front();
			return t;
		}
		else
			return _invalid.t;	
	};

	void push(const T& t)
	{
		if (isvalid(t))
			_l.push_back(*((int*)&t - 1));
	};

	bool isvalid(const T& t)
	{
		return (&t != &_invalid.t);
	};	
	
	void clear()
	{
		_v.clear();
		_l.clear();
		_cap = 0;
	};
private:
	vector<elem> _v;
	list<int> _l;
	elem _invalid;
	int _cap;
};

}
