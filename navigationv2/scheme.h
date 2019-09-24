#pragma once
#include <iostream>
#include <json/json.h>

#include "nav_error_code.h"
#include "navigation.h"
#include "task_flow.h"
#include "task.h"

using namespace std;
using namespace Json;

namespace nav{

class Scheme:public TaskFlow{
public:
	enum MODE{
		SINGLE = 1,
	    LOOP   = 2,
	    RANDOM = 3
	};
	using vct_string= vector<string>;

class NavTask:public Task{
public:
	inline NavTask(const GsNamePoint& pt, const string& path)
	:Task("NavTask_" + path + "_" + pt.name), _pt(pt), _path(path){};
	void execute(){};
private:
	GsNamePoint _pt;
	string      _path;	
};

class PTZTask:public Task{
public:
	inline PTZTask(OP op, int h, int v, int z)
	:Task("PTZTask"), _op(op), _h(h), _v(v), _z(z){}
	void execute(){};
private:
	OP _op;
	int _h, _v, _z;
};

class BrocastTask:public Task{
public:
	inline BrocastTask(OP op, MODE md, vct_string& lst)
	:_op(op), _md(md), _lst(lst){};
	void execute(){};
private:
	OP     _op;
	MODE       _md;
	vct_string _lst;
};

class LampTask:public Task{
public:
	inline LampTask(OP op, bool white, bool rb, bool alarm)
	:Task("LampTask"), _op(op), _white(white), _rb(rb), _alarm(alarm){};
	void execute(){};
private:
	OP _op;
	bool _white, _rb, _alarm;
}; 

	Scheme(const string& name)
	:TaskFlow(name), _start_time(0), _exe_times(1), _merge_paths(false){};
	virtual ~Scheme(){};

	ERR_CODE parse(Value& scheme);
private:
	ERR_CODE check_json(Value& scheme);
	ERR_CODE ptz_action();

private:
	unsigned int _start_time;	
	int          _exe_times;
	GsPos        _ret_pt;
	bool         _merge_paths;
};


}

