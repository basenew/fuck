#pragma once
#include <list>
#include <thread>
#include <chrono>
#include <iostream>
#include <functional>

#include "task.h"
#include "task_node.h"

using namespace std;
using namespace std::chrono;

namespace nav{

class TaskThread
{
public:
	inline TaskThread(const string& name = "")
	:_name("thd_"+name)
	,_thd(nullptr)
	,_running(false)
	,_exited(false)
	,_t(nullptr)
	{};
	
	inline virtual ~TaskThread(){
		if (_thd)
		{
			delete _thd;
			_thd = nullptr;
		}
	};

	TaskThread(const TaskThread&)=delete;
	void operator=(const TaskThread&)=delete;

	const string& name(){return _name;};
	void name(const string& name){_name = name;};
	bool start();

	bool stop();

	bool push(Task* t);

	inline Task* task(){return _t;};
	
	inline bool is_exited(){return _exited;};

private:
	inline static void _thread_proc_cb(TaskThread* t)
	{
		if (t) t->_thread_proc();
	};

	void _thread_proc();

private:
	bool    _running;
	bool    _exited;
	string  _name;
	thread* _thd;

	Task*   _t;
	Task*   _executed_t;

	recursive_mutex    _mt;
	condition_variable_any _cv;
};

}

